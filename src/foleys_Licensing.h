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
    ServerError,
    CouldNotSave,
    CouldNotRead,
    HardwareMismatch,
};

enum class Status
{
    // Make sure to keep synchronised with the server!
    Unchecked        = 0,  //< Initial state
    Activated        = 1,  //< Activated and can be run
    NotActivated     = 2,  //< But can be activated
    Expired          = 3,  //< License expired
    Deactivated      = 4,  //< License was manually deactivated
    NoActivationLeft = 5,  //< tried to activate but no activations left
};

struct Activation
{
    size_t      index = 0;
    std::string computer;
    std::string user;
};

}  // namespace foleys::Licensing


#endif  // FOLEYS_LICENSING_CLIENT_FOLEYS_LICENSING_H
