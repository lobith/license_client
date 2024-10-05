/********************************************************************************
Copyright 2024 - Daniel Walz
Foleys Finest Audio UG (haftungsbeschraenkt)
Lobith Audio UG (haftungsbeschraenkt)
********************************************************************************

This code is provided under the ISC license

This code is provided as is. The authors disclaim all warranties.
For details refer to the LICENSE.md

*******************************************************************************/


#include "foleys_LicenseUpdater.h"
#include "foleys_LicenseData.h"
#include "foleys_License.h"
#include "foleys_Crypto.h"
#include "foleys_Checks.h"

#include <nlohmann/json.hpp>
#include <cpr/cpr.h>


namespace foleys
{

LicenseUpdater::LicenseUpdater()
{
    fetchIfNecessary();
}

void LicenseUpdater::setupLicenseData (const std::filesystem::path& licenseFile, std::string_view hwUID, std::initializer_list<std::pair<std::string, std::string>> data)
{
    localStorage = licenseFile;
    hardwareUid  = hwUID;
    defaultData  = data;

    fetchIfNecessary();
}

void LicenseUpdater::fetchIfNecessary (int hours)
{
    if (hardwareUid.empty())
        return;

    auto contents = getContents();
    if (contents.empty())
    {
        fetchLicenseData();
        return;
    }

    auto json = nlohmann::json::parse (getContents(), nullptr, false);
    if (json.is_discarded())
    {
        fetchLicenseData();
        return;
    }

    auto timestamp = License::decodeDateTime (json[LicenseID::checked], "%Y-%m-%dT%H:%M:%S");
    auto seconds   = std::difftime (std::time (nullptr), timestamp);

    if (seconds > 3600 * hours)
        fetchLicenseData();
}


void LicenseUpdater::fetchLicenseData (std::string_view action, std::initializer_list<std::pair<std::string, std::string>> data)
{
    if (hardwareUid.empty())
        return;

    nlohmann::json request;
    if (!action.empty())
        request[LicenseID::action] = action;

    request[LicenseID::product]  = LicenseData::productUid;
    request[LicenseID::hardware] = hardwareUid;

    for (const auto& pair: defaultData)
        request[pair.first] = pair.second;

    for (const auto& pair: data)
        request[pair.first] = pair.second;

#if FOLEYS_LICENSE_ENCRYPT_REQUEST
    auto          cipher = Crypto::encrypt (request.dump());
    cpr::Response r      = cpr::Post (cpr::Url (LicenseData::authServerUrl) + "auth/" + LicenseData::productUid + "/", cpr::Body (cipher));
#else
    cpr::Response r = cpr::Post (cpr::Url (LicenseData::authServerUrl) + "auth/", cpr::Body (request.dump()));
#endif

    if (r.status_code >= 400)
    {
        lastError       = Licensing::Error::ServerAnswerInvalid;
        lastErrorString = "Server error (http status code: " + std::to_string (r.status_code) + ")";
        sendUpdateSignal();
        return;
    }

    auto answer = Crypto::decrypt (r.text);

    if (answer.empty())
    {
        lastError       = Licensing::Error::ServerAnswerInvalid;
        lastErrorString = "Server Error (could not decrypt)";
        sendUpdateSignal();
        return;
    }

    auto json = nlohmann::json::parse (answer, nullptr, false);
    if (json.is_discarded())
    {
        lastError       = Licensing::Error::ServerAnswerInvalid;
        lastErrorString = "Server Error (invalid json)";
        sendUpdateSignal();
        return;
    }

    if (Checks::getHardwareUID (json) != hardwareUid)
    {
        lastError       = Licensing::Error::ServerAnswerInvalid;
        lastErrorString = "License not applicable";
        sendUpdateSignal();
        return;
    }

    {
        const std::scoped_lock mutex (storageMutex);

        if (localStorage.empty())
        {
            lastError       = Licensing::Error::CouldNotSave;
            lastErrorString = "Could not open license file for writing";
            sendUpdateSignal();
            return;
        }

        std::filesystem::create_directories (localStorage.parent_path());
        std::ofstream output (localStorage);
        if (output.is_open())
        {
            output << r.text;
        }
        else
        {
            lastError       = Licensing::Error::CouldNotSave;
            lastErrorString = "Could not open license file for writing";
        }
    }

    sendUpdateSignal();
}

std::string LicenseUpdater::getContents()
{
    if (localStorage.empty())
        return {};

    const std::scoped_lock mutex (storageMutex);

    std::ifstream input (localStorage);
    std::string   cipher (std::istreambuf_iterator<char> { input }, {});

    if (cipher.empty())
        return {};

    return Crypto::decrypt (cipher);
}

void LicenseUpdater::sendUpdateSignal()
{
    observerList.call ([] (auto& l) { l.licenseUpdated(); });
}

void LicenseUpdater::addObserver (Observer* observer)
{
    observerList.addObserver (observer);
}

void LicenseUpdater::removeObserver (Observer* observer)
{
    observerList.removeObserver (observer);
}

}  // namespace foleys
