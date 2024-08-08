//
// Created by Daniel Walz on 07.08.24.
//

#include "foleys_License.h"

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


}  // namespace foleys
