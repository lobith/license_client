//
// Created by Daniel Walz on 07.08.24.
//

#include "foleys_License.h"

#include "private/foleys_LicenseData.h"

#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

#include <iostream>

namespace foleys
{

Licensing::Licensing()
{
    loadLicenseBlob();

    if (lastCheckExpired())
        fetchLicenseData();
}

void Licensing::reload()
{
    fetchLicenseData();
}

bool Licensing::activated() const
{
    return activatedFlag.load();
}

bool Licensing::expired() const
{
    return false;
}

std::optional<std::time_t> Licensing::expires() const
{
    return {};
}

std::string Licensing::getLicenseeEmail() const
{
    return "unknown";
}

int Licensing::getAvailableActivations() const
{
    return 0;
}

int Licensing::getTotalActivations() const
{
    return 0;
}

void Licensing::setLocalStorage (const std::filesystem::path& path)
{
    localStorage = path;
    if (std::filesystem::exists (localStorage))
    {
        loadLicenseBlob();
    }
}

void Licensing::setHardwareUid (std::string_view uid)
{
    hardwareUid = uid;
}

// ================================================================================

void Licensing::fetchLicenseData (std::string_view action)
{
    nlohmann::json request;
    request["product"]  = LicenseData::productUid;
    request["hardware"] = hardwareUid;

    cpr::Response r = cpr::Post (cpr::Url (LicenseData::authServerUrl), cpr::Body (request.dump()));
    std::cout << "Status: " << r.status_code << std::endl;
    std::cout << r.text << std::endl;
}

void Licensing::loadLicenseBlob() { }

bool Licensing::lastCheckExpired() const
{
    return true;
}


}  // namespace foleys
