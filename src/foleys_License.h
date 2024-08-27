//
// Created by Daniel Walz on 07.08.24.
//

#ifndef FOLEYS_LICENSE_CLIENT_FOLEYS_LICENSE_H
#define FOLEYS_LICENSE_CLIENT_FOLEYS_LICENSE_H

#include "private/foleys_SharedObject.h"
#include "private/foleys_Observers.h"

#include <atomic>
#include <optional>
#include <ctime>
#include <filesystem>

namespace foleys
{

class Licensing
{
public:
    Licensing();

    void setLocalStorage (const std::filesystem::path& path);
    void setHardwareUid (std::string_view uid);

    /**
     * Access the server for a new state
     */
    void syncLicense();

    /**
     * Load the state from disk and if necessary fetch a new license
     */
    void reload();

    /**
     * If the last action resulted in an error, this will return it
     * @return the error as string
     */
    std::string getLastError() const;

    /**
     * Check if the machine is activated.
     * This value is cached to be fast and thread safe, but less secure.
     *
     * @return true if this machine is checked and activated.
     */
    [[nodiscard]] bool activated() const;

    /**
     * Check if the machine is expired (according to server time)
     * This value is cached to be fast and thread safe, but less secure.
     *
     * @return true if the expiry date is in the past.
     */
    [[nodiscard]] bool expired() const;

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
    std::string getLicenseeEmail() const;

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
     * Tries to get new license data from the server.
     * @param action an optional action. Allowed values: 'demo' or 'activate'. Anything else just gets the status
     */
    void fetchLicenseData (std::string_view action = {}, std::initializer_list<std::pair<std::string, std::string>> data = {});
    void loadLicenseBlob();

    bool lastCheckExpired() const;

    using Ptr = SharedObject<Licensing>;

    struct Observer
    {
        virtual ~Observer()           = default;
        virtual void licenseLoaded()  = 0;
        virtual void licenseFetched() = 0;
    };

    /**
     * Register an observer when a new license answer is received or loaded. Make sure to removeObserver before you destroy the observer.
     * @param observer
     */
    void addObserver (Observer* observer);

    /**
     * Deregister an observer
     * @param observer
     */
    void removeObserver (Observer* observer);

private:
    [[nodiscard]] bool processData (std::string_view data);

    std::filesystem::path      localStorage;
    std::string                hardwareUid;
    std::string                email;
    std::string                lastError;
    std::atomic<bool>          activatedFlag = false;
    std::atomic<bool>          demoAvailable = false;
    std::atomic<int>           demoDays      = 0;
    std::optional<std::time_t> expiryDate;
    std::time_t                checked = {};
    ObserverList<Observer>     observerList;
};


}  // namespace foleys

#endif  // FOLEYS_LICENSE_CLIENT_FOLEYS_LICENSE_H
