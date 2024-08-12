//
// Created by Daniel Walz on 12.08.24.
//

#ifndef FOLEYS_LICENSING_CLIENT_LICENSINGDEMOEDITOR_H
#define FOLEYS_LICENSING_CLIENT_LICENSINGDEMOEDITOR_H


#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

class LicensingDemoProcessor;

class LicensingDemoEditor : public juce::AudioProcessorEditor
{
public:
    LicensingDemoEditor (LicensingDemoProcessor& processor);

    void paint (juce::Graphics& g) override;


private:
    LicensingDemoProcessor& processor;
};

#endif  // FOLEYS_LICENSING_CLIENT_LICENSINGDEMOEDITOR_H
