//
// Created by Daniel Walz on 12.08.24.
//

#ifndef FOLEYS_LICENSING_CLIENT_LICENSINGDEMOEDITOR_H
#define FOLEYS_LICENSING_CLIENT_LICENSINGDEMOEDITOR_H


#include "LicensePanel.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

class LicensingDemoProcessor;

class LicensingDemoEditor : public juce::AudioProcessorEditor
{
public:
    LicensingDemoEditor (LicensingDemoProcessor& processor);

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    LicensingDemoProcessor& processor;
    LicensePanelHolder      licensingPanel { this };
};

#endif  // FOLEYS_LICENSING_CLIENT_LICENSINGDEMOEDITOR_H
