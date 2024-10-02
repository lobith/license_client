/********************************************************************************
Copyright 2024 - Daniel Walz
Foleys Finest Audio UG (haftungsbeschraenkt)
Lobith Audio UG (haftungsbeschraenkt)
********************************************************************************

This code is provided under the ISC license

This code is provided as is. The authors disclaim all warranties.
For details refer to the LICENSE.md

*******************************************************************************/


#ifndef FOLEYS_LICENSING_CLIENT_FOLEYS_LICENSEDATA_H
#define FOLEYS_LICENSING_CLIENT_FOLEYS_LICENSEDATA_H

namespace LicenseData
{

extern const char* productName;
extern const char* copyright;
extern const char* version;
extern const char* authServerUrl; //< license check backend
extern const char* buyUrl;
extern const char* manualUrl;
extern const char* productUid;
extern const unsigned char* publicKey;     //< server's public
extern const unsigned char* privateKey;    //< client's private

}


#endif  // FOLEYS_LICENSING_CLIENT_FOLEYS_LICENSEDATA_H
