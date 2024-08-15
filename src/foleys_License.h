//
// Created by Daniel Walz on 07.08.24.
//

#ifndef FOLEYS_LICENSE_CLIENT_FOLEYS_LICENSE_H
#define FOLEYS_LICENSE_CLIENT_FOLEYS_LICENSE_H

#include "private/foleys_SharedObject.h"

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
    void reload();

    /**
     * Check if the machine is activated.
     * This value is cached to be fast and thread safe, but less secure.
     *
     * @return true if this machine is checked and activated.
     */
    bool activated() const;

    /**
     * Check if the machine is expired (according to server time)
     * This value is cached to be fast and thread safe, but less secure.
     *
     * @return true if the expiry date is in the past.
     */
    bool expired() const;

    /**
     * This returns the expiry time/date or nullopt if it is a perpetual license.
     * @return The expiry date
     */
    std::optional<std::time_t> expires() const;

    std::string getLicenseeEmail() const;
    int         getAvailableActivations() const;
    int         getTotalActivations() const;

    /**
     * Tries to get new license data from the server.
     * @param action an optional action. Allowed values: 'demo' or 'activate'. Anything else just gets the status
     */
    void fetchLicenseData (std::string_view action = {});
    void loadLicenseBlob();

    bool lastCheckExpired() const;

    using Ptr = SharedObject<Licensing>;

private:
    std::filesystem::path localStorage;
    std::string           hardwareUid;
    std::atomic<bool>     activatedFlag;
};


};  // namespace foleys

#endif  // FOLEYS_LICENSE_CLIENT_FOLEYS_LICENSE_H
