//
// Created by Daniel Walz on 12.08.24.
//

#include "LicensingDemoProcessor.h"
#include "LicensingDemoEditor.h"

LicensingDemoProcessor::LicensingDemoProcessor()
{
    // TODO: do this in the module itself
    licensing->setHardwareUid (juce::SystemStats::getUniqueDeviceID().toRawUTF8());


    auto appFolder = juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory);
#if JUCE_MAC
    appFolder = appFolder.getChildFile("Application Support");
#endif
    auto licFile = appFolder.getChildFile ("Manufacturer").getChildFile ("LicensingDemoPlugin.lic");
    licensing->setLocalStorage (licFile.getFullPathName().toRawUTF8());
    licensing->reload();
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
