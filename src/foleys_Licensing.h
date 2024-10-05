//
// Created by Daniel Walz on 23.09.24.
//

#ifndef FOLEYS_LICENSING_CLIENT_FOLEYS_LICENSING_H
#define FOLEYS_LICENSING_CLIENT_FOLEYS_LICENSING_H

#include <filesystem>

namespace foleys::Licensing
{

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
