//
// Created by Daniel Walz on 08.08.24.
//

#include <foleys_License.h>


namespace foleys::Licensing
{
const std::filesystem::path localStorage = "~/Library/Application Support/Manufacturer/TestStandalone.lic";
const std::string           hardwareUid  = "Hardware TODO";
}  // namespace foleys::Licensing

int main (int, char**)
{

    foleys::License license;


    return 0;
}
