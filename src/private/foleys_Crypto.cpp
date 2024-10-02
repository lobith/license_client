/********************************************************************************
Copyright 2024 - Daniel Walz
Foleys Finest Audio UG (haftungsbeschraenkt)
Lobith Audio UG (haftungsbeschraenkt)
********************************************************************************

This code is provided under the ISC license

This code is provided as is. The authors disclaim all warranties.
For details refer to the LICENSE.md

*******************************************************************************/

#include "foleys_Crypto.h"
#include "foleys_LicenseData.h"
#include "foleys_Licensing.h"

#include <private/choc_Base64.h>
#include <cpr/cpr.h>
#include <sodium.h>

namespace foleys
{

std::string Crypto::decrypt (std::string_view data)
{
    std::vector<unsigned char> binary;
    if (!choc::base64::decodeToContainer (binary, data))
        return {};

    std::vector<unsigned char> message (binary.size());
    if (crypto_box_open_easy (message.data(), binary.data() + crypto_box_noncebytes(), binary.size() - crypto_box_noncebytes(), binary.data(),
                              LicenseData::publicKey, LicenseData::privateKey)
        != 0)
    {
        return {};
    }

    return { message.begin(), message.end() };
}

std::string Crypto::encrypt (std::string_view message)
{
    const auto                 numBytes = message.size() + crypto_box_noncebytes();
    std::vector<unsigned char> plain { message.begin(), message.end() };
    std::vector<unsigned char> cipher (numBytes);

    randombytes_buf (cipher.data(), crypto_box_noncebytes());
    auto cipherText = cipher.data() + crypto_box_noncebytes();

    if (crypto_box_easy (cipherText, plain.data(), message.size(), cipher.data(), LicenseData::publicKey, LicenseData::privateKey) != 0)
    {
        return {};
    }

    return choc::base64::encodeToString (cipher.data(), cipher.size());
}

}  // namespace foleys
