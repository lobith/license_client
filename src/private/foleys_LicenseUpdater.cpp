//
// Created by Daniel Walz on 23.09.24.
//

#include "foleys_LicenseUpdater.h"
#include "foleys_LicenseData.h"
#include "foleys_License.h"
#include "foleys_Crypto.h"

#include <private/choc_Base64.h>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>


namespace foleys
{

LicenseUpdater::LicenseUpdater()
{
    fetchIfNecessary();
}

void LicenseUpdater::fetchIfNecessary (int hours)
{
    auto timestamp = License::getLicenseTimestamp();
    auto seconds   = std::difftime (std::time (nullptr), timestamp);

    if (seconds > 3600 * hours)
        fetchLicenseData();
}


void LicenseUpdater::fetchLicenseData (std::string_view action, std::initializer_list<std::pair<std::string, std::string>> data)
{
    nlohmann::json request;
    request["product"]  = LicenseData::productUid;
    request["version"]  = LicenseData::version;
    request["hardware"] = Licensing::hardwareUid;
    request["os"]       = Licensing::os;
    request["host"]     = Licensing::host;

    if (!action.empty())
        request["action"] = action;

    for (const auto& pair: data)
        request[pair.first] = pair.second;

    cpr::Response r = cpr::Post (cpr::Url (LicenseData::authServerUrl) + "auth/", cpr::Body (request.dump()));

    auto answer = Crypto::decrypt (r.text);

    if (answer.empty())
    {
        lastError       = Licensing::Error::ServerAnswerInvalid;
        lastErrorString = "Server Error";
        sendUpdateSignal();
        return;
    }

    if (License::checkHardwareUid (answer))
    {
        std::filesystem::create_directories (Licensing::localStorage.parent_path());
        std::ofstream output (Licensing::localStorage);
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
    else
    {
        lastError       = Licensing::Error::ServerAnswerInvalid;
        lastErrorString = "License not applicable";
    }

    sendUpdateSignal();
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
