//
// Created by Daniel Walz on 12.08.24.
//

#include "LicensingDemoProcessor.h"
#include "LicensingDemoEditor.h"
#include "juce/foleys_JuceUtilities.h"

namespace foleys::Licensing
{
// This block sets up some static data for all instances of this plugin.
const std::filesystem::path localStorage = createLicensePath ("Manufacturer", LicenseData::productName);
const std::string           hardwareUid  = juce::SystemStats::getUniqueDeviceID().toRawUTF8();
}  // namespace foleys::Licensing


LicensingDemoProcessor::LicensingDemoProcessor() = default;

void LicensingDemoProcessor::prepareToPlay ([[maybe_unused]] double sampleRate, [[maybe_unused]] int expectedNumSamples) { }

void LicensingDemoProcessor::processBlock (juce::AudioBuffer<float>& buffer, [[maybe_unused]] juce::MidiBuffer& midi)
{
    if (!license.isAllowed())
    {
        buffer.clear();
        return;
    }

    for (int c = getTotalNumInputChannels(); c < getTotalNumOutputChannels(); ++c)
        buffer.clear (c, 0, buffer.getNumSamples());
}

void LicensingDemoProcessor::releaseResources() { }

juce::AudioProcessorEditor* LicensingDemoProcessor::createEditor()
{
    return new LicensingDemoEditor (*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LicensingDemoProcessor();
}
