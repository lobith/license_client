/********************************************************************************
Copyright 2024 - Daniel Walz
Foleys Finest Audio UG (haftungsbeschraenkt)
Lobith Audio UG (haftungsbeschraenkt)
********************************************************************************

This code is provided under the ISC license

This code is provided as is. The authors disclaim all warranties.
For details refer to the LICENSE.md

*******************************************************************************/

#include "foleys_License.h"

#include "private/foleys_LicenseData.h"
#include "private/foleys_Crypto.h"

#include <nlohmann/json.hpp>
#include <string>
#include <sstream>
#include <fstream>

namespace foleys
{

License::License()
{
    updater->addObserver (this);
    updater->fetchIfNecessary();
    loadLicenseBlob();
}

License::~License()
{
    updater->removeObserver (this);
}

void License::syncLicense()
{
    updater->fetchLicenseData();
}

bool License::isActivated() const
{
    return activatedFlag.load();
}

bool License::isExpired() const
{
    if (expiryDate)
        return *expiryDate < std::time (nullptr);

    return false;
}

bool License::isAllowed() const
{
    return checkHardwareUid() && ((isActivated() && !isExpired()) || isDemo());
}

bool License::checkHardwareUid() const
{
    return Licensing::hardwareUid == licenseHardware;
}

// static
bool License::checkHardwareUid (std::string_view data)
{
    auto response = nlohmann::json::parse (data, nullptr, false);
    if (response.is_discarded())
        return false;

    if (response.contains ("hardware"))
        return Licensing::hardwareUid == response["hardware"];

    return false;
}

std::string License::getLastErrorString() const
{
    return updater->getLastErrorString();
}

std::optional<std::time_t> License::expires() const
{
    return expiryDate;
}

std::string License::getLicenseeEmail() const
{
    std::scoped_lock guard (processingLock);
    return email;
}

void License::login (const std::string& login_email)
{
    updater->fetchLicenseData ("login", { { "login_email", login_email } });
}

void License::activate (std::initializer_list<std::pair<std::string, std::string>> data)
{
    updater->fetchLicenseData ("activate", data);
}

bool License::canDemo() const
{
    return demoAvailable;
}

bool License::isDemo() const
{
    return demoDays > 0 && !demoAvailable;
}

int License::demoDaysLeft() const
{
    return demoDays;
}

void License::startDemo()
{
    updater->fetchLicenseData ("demo");
}

time_t License::getLicenseTimestamp()
{
    auto response = nlohmann::json::parse (getContents(), nullptr, false);
    if (response.is_discarded())
        return {};

    return decodeDateTime (response["checked"], "%Y-%m-%dT%H:%M:%S");
}

bool License::shouldShowPopup()
{
    return !isAllowed() || (!updater->wasPopupShown() && !isActivated());
}


// ================================================================================


std::pair<Licensing::Error, std::string> License::processData (std::string_view message)
{
    auto response = nlohmann::json::parse (message, nullptr, false);
    if (response.is_discarded())
        return { Licensing::Error::ServerAnswerInvalid, "Got invalid license data (bad json)" };

    std::scoped_lock guard (processingLock);

    checked       = decodeDateTime (response["checked"], "%Y-%m-%dT%H:%M:%S");
    activatedFlag = response["activated"];
    email         = response.contains ("licensee_email") ? response["licensee_email"] : "";

    licenseHardware = response["hardware"];

    if (!checkHardwareUid())
    {
        activatedFlag = false;
        return { Licensing::Error::HardwareMismatch, "Hardware mismatch" };
    }

    if (response.contains ("license_expires"))
        expiryDate = decodeDateTime (response["license_expires"], "%Y-%m-%d");
    else
        expiryDate = std::nullopt;

    if (response.contains ("demo_available"))
    {
        demoAvailable = response["demo_available"];
        demoDays      = response["demo_days"];
        if (response.contains ("demo_ends"))
        {
            auto ends          = decodeDateTime (response["demo_ends"], "%Y-%m-%d");
            auto localDemoDays = static_cast<int> (1 + difftime (ends, std::time (nullptr)) / (24 * 3600));
            demoDays           = std::min (demoDays.load(), localDemoDays);
        }
    }
    else
    {
        demoAvailable = false;
        demoDays      = 0;
    }

    if (response.contains ("error"))
    {
        return { Licensing::Error::HardwareMismatch, response["error"] };
    }

    return { Licensing::Error::NoError, {} };
}

std::string License::getContents()
{
    std::ifstream input (Licensing::localStorage);
    std::string   cipher (std::istreambuf_iterator<char> { input }, {});

    if (cipher.empty())
        return {};

    return Crypto::decrypt (cipher);
}

time_t License::decodeDateTime (const std::string& timeString, const char* formatString)
{
    std::tm            tm {};
    std::istringstream ss (timeString);
    ss >> std::get_time (&tm, formatString);
    return std::mktime (&tm);
}

std::pair<Licensing::Error, std::string> License::loadLicenseBlob()
{
    auto text = getContents();

    if (text.empty())
        return { Licensing::Error::CouldNotRead, "No local license file" };

    return processData (text);
}

void License::licenseUpdated()
{
    loadLicenseBlob();

    if (onLicenseReceived)
        onLicenseReceived();
}


}  // namespace foleys
