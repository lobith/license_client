/********************************************************************************
    Copyright 2024 - Daniel Walz
    Foleys Finest Audio UG (haftungsbeschraenkt)
    Lobith Audio UG (haftungsbeschraenkt)
 ********************************************************************************

    This code is provided under the ISC license

    This code is provided as is. The authors disclaim all warranties.
    For details refer to the LICENSE.md

 *******************************************************************************/


#ifndef FOLEYS_LICENSE_CLIENT_FOLEYS_LICENSE_H
#define FOLEYS_LICENSE_CLIENT_FOLEYS_LICENSE_H

#include "private/foleys_SharedObject.h"
#include "private/foleys_Observers.h"
#include "private/foleys_LicenseData.h"
#include "private/foleys_LicenseUpdater.h"

#include <atomic>
#include <optional>
#include <ctime>
#include <filesystem>

namespace foleys
{


class License : private LicenseUpdater::Observer
{
public:
    License();
    ~License() override;


    /**
     * @return the timestamp when the current license was created (server time)
     */
    static time_t getLicenseTimestamp();


    [[nodiscard]] bool shouldShowPopup();

    /**
     * Access the server for a new state
     */
    void syncLicense();

    /**
     * If the last action resulted in an error, this will return it
     * @return the error as string
     */
    [[nodiscard]] std::string getLastErrorString() const;


    /**
     * Check if the hardwareUid and the uid in the license file match
     * @return true if the hardwareUid matches the machine the license was activated to.
     */
    [[nodiscard]] bool checkHardwareUid() const;

    /**
     * Verify if a license is applicable
     * @param data the decrypted json string
     * @return true if the hardware id matches
     */
    [[nodiscard]] static bool checkHardwareUid (std::string_view data);

    /**
     * Check if the machine is activated.
     * This value is cached to be fast and thread safe, but less secure.
     *
     * @return true if this machine is checked and activated.
     */
    [[nodiscard]] bool isActivated() const;

    /**
     * Check if the machine is expired (according to server time)
     * This value is cached to be fast and thread safe, but less secure.
     *
     * @return true if the expiry date is in the past.
     */
    [[nodiscard]] bool isExpired() const;

    /**
     * This is a shorthand that either a license that is not expired or a demo is running
     * @return true if is allowed to run
     */
    [[nodiscard]] bool isAllowed() const;

    /**
     * This returns the expiry time/date or nullopt if it is a perpetual license.
     * @return The expiry date
     */
    std::optional<std::time_t> expires() const;

    /**
     * When activated this can be used to display the licensee
     * @return the email the license is licensed to
     */
    [[nodiscard]] std::string getLicenseeEmail() const;

    /**
     * Send a login request. The server will send a login link
     */
    void login (const std::string& email);

    /**
     * Request the server to activate this computer
     */
    void activate (std::initializer_list<std::pair<std::string, std::string>> data);

    [[nodiscard]] bool canDemo() const;
    [[nodiscard]] bool isDemo() const;
    [[nodiscard]] int  demoDaysLeft() const;

    /**
     * Send a demo request to the server
     */
    void startDemo();

    /**
     * Set a flag in the updater to avoid multiple times popup
     * @param wasShown
     */
    void setPopupWasShown (bool wasShown) { updater->setPopupWasShown (wasShown); }

    /**
     * This is called when a valid license was received
     */
    std::function<void()> onLicenseReceived;

private:
    std::pair<Licensing::Error, std::string> loadLicenseBlob();

    void licenseUpdated() override;

    [[nodiscard]] static std::string getContents();
    [[nodiscard]] static time_t      decodeDateTime (const std::string& timeString, const char* formatString);

    [[nodiscard]] std::pair<Licensing::Error, std::string> processData (std::string_view data);

    LicenseUpdater::Ptr updater;

    mutable std::mutex processingLock;

    std::string                licenseHardware;
    std::string                email;
    std::atomic<bool>          activatedFlag = false;
    std::atomic<bool>          demoAvailable = false;
    std::atomic<int>           demoDays      = 0;
    std::optional<std::time_t> expiryDate;
    std::time_t                checked = {};
};


}  // namespace foleys

#endif  // FOLEYS_LICENSE_CLIENT_FOLEYS_LICENSE_H
