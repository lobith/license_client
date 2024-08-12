//
// Created by Daniel Walz on 07.08.24.
//

#include "foleys_License.h"

#include <iostream>

namespace foleys
{

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

std::string Licensing::getLicenseeEmail() const { }

int Licensing::getAvailableActivations() const { }

int Licensing::getTotalActivations() const { }

void Licensing::setLocalStorage (const std::filesystem::path& path)
{
    localStorage = path;
    if (std::filesystem::exists (localStorage))
    {
        loadLicenseBlob();
    }
}

// ================================================================================

void Licensing::loadLicenseBlob()
{

}


}  // namespace foleys
