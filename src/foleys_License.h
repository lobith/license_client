//
// Created by Daniel Walz on 07.08.24.
//

#ifndef FOLEYS_LICENSE_CLIENT_FOLEYS_LICENSE_H
#define FOLEYS_LICENSE_CLIENT_FOLEYS_LICENSE_H

#include "foleys_SharedObject.h"

#include <atomic>

namespace foleys
{

class Licensing
{
public:
    Licensing() = default;

    bool activated() const;
    bool expired() const;

    using Ptr = SharedObject<Licensing>;

private:
    std::atomic<bool> activatedFlag { false };
};


};  // namespace foleys

#endif  // FOLEYS_LICENSE_CLIENT_FOLEYS_LICENSE_H
