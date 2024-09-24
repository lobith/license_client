//
// Created by Daniel Walz on 15.08.24.
//

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
