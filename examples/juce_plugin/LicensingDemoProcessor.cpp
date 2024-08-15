//
// Created by Daniel Walz on 12.08.24.
//

#include "LicensingDemoProcessor.h"
#include "LicensingDemoEditor.h"

LicensingDemoProcessor::LicensingDemoProcessor()
{
    // TODO: do this in the module itself
    licensing->setHardwareUid (juce::SystemStats::getUniqueDeviceID().toRawUTF8());

    auto licFile = juce::File::getSpecialLocation (juce::File::commonApplicationDataDirectory).getChildFile ("Manufacturer").getChildFile ("LicensingDemoPlugin.lic");
    licensing->setLocalStorage (licFile.getFullPathName().toRawUTF8());
}


void LicensingDemoProcessor::prepareToPlay (double sampleRate, int expectedNumSamples) { }

void LicensingDemoProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    if (!licensing->activated())
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
