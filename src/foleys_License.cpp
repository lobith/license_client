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

Licensing::Licensing() { }

void Licensing::reload()
{
    loadLicenseBlob();

    if (lastCheckExpired())
        syncLicense();
}

void Licensing::syncLicense()
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

void Licensing::activate()
{
    fetchLicenseData ("activate");
}

bool Licensing::canDemo()
{
    return demoAvailable;
}

bool Licensing::isDemo()
{
    return !demoAvailable && demoDays > 0;
}

int Licensing::demoDaysLeft()
{
    return demoDays;
}

void Licensing::startDemo()
{
    fetchLicenseData ("demo");
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
    if (!action.empty())
        request["action"] = action;

    cpr::Response r = cpr::Post (cpr::Url (LicenseData::authServerUrl), cpr::Body (request.dump()));

    if (processData (r.text))
    {
        std::filesystem::create_directories (localStorage.parent_path());
        std::ofstream output (localStorage);
        output << r.text;
    }
}

bool Licensing::processData (std::string_view data)
{
    auto response = nlohmann::json::parse (data, nullptr, false);
    if (response.is_discarded())
        return false;

    demoAvailable = response["demo_available"];
    demoDays      = response["demo_days"];

    return true;
}

void Licensing::loadLicenseBlob()
{
    std::ifstream input (localStorage);
    std::string   text (std::istreambuf_iterator<char> { input }, {});

    if (!processData (text))
        fetchLicenseData();
}

bool Licensing::lastCheckExpired() const
{
    return true;
}


}  // namespace foleys
