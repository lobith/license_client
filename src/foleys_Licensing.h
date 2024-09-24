//
// Created by Daniel Walz on 23.09.24.
//

#ifndef FOLEYS_LICENSING_CLIENT_FOLEYS_LICENSING_H
#define FOLEYS_LICENSING_CLIENT_FOLEYS_LICENSING_H

#include <filesystem>

namespace foleys::Licensing
{

extern const std::filesystem::path localStorage;
extern const std::string           hardwareUid;

enum class Error
{
    NoError = 0,
    ServerNotAvailable,
    ServerAnswerInvalid,
    CouldNotSave,
    CouldNotRead,
    HardwareMismatch,
};


}  // namespace foleys::Licensing


#endif  // FOLEYS_LICENSING_CLIENT_FOLEYS_LICENSING_H
