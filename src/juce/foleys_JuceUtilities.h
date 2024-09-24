//
// Created by Daniel Walz on 24.09.24.
//

#ifndef FOLEYS_LICENSING_CLIENT_FOLEYS_JUCEUTILITIES_H
#define FOLEYS_LICENSING_CLIENT_FOLEYS_JUCEUTILITIES_H

#include "private/foleys_LicenseData.h"

#include <juce_core/juce_core.h>
#include <filesystem>

namespace foleys
{

static inline std::filesystem::path createLicensePath (const char* manufacturer, const char* productName)
{
    auto appFolder = juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory);
#if JUCE_MAC
    appFolder = appFolder.getChildFile ("Application Support");
#endif
    return appFolder.getChildFile (manufacturer).getChildFile (productName).withFileExtension (".lic").getFullPathName().toRawUTF8();
}


}  // namespace foleys


#endif  // FOLEYS_LICENSING_CLIENT_FOLEYS_JUCEUTILITIES_H
