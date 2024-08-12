//
// Created by Daniel Walz on 12.08.24.
//

#include "LicensingDemoEditor.h"
#include "LicensingDemoProcessor.h"


LicensingDemoEditor::LicensingDemoEditor (LicensingDemoProcessor& p) : juce::AudioProcessorEditor (&p), processor (p)
{
    setSize (640, 480);
}

void LicensingDemoEditor::paint (juce::Graphics& g)
{
    g.fillAll (findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::silver);
    g.drawFittedText ("Hello user!", getLocalBounds(), juce::Justification::centred, 1);
}
