//
// Created by Daniel Walz on 12.08.24.
//

#include "LicensePanel.h"


LicensePanel::LicensePanel()
{
    addAndMakeVisible (demo);
    addAndMakeVisible (activate);
    addAndMakeVisible (refresh);
    addAndMakeVisible (manage);
}

void LicensePanel::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black.withAlpha (0.8f));
}

void LicensePanel::resized()
{
    auto buttonRow = getLocalBounds().removeFromBottom (70);

    juce::FlexBox flexBox;
    flexBox.items = {
        juce::FlexItem (demo).withFlex (1.0).withMargin (20),
        juce::FlexItem (activate).withFlex (1.0).withMargin (20),
        juce::FlexItem (refresh).withFlex (1.0).withMargin (20),
        juce::FlexItem (manage).withFlex (1.0).withMargin (20),
    };

    flexBox.performLayout (buttonRow);
}


// ================================================================================

LicensePanelHolder::LicensePanelHolder (juce::Component* parent) : parentComponent (parent) { }

void LicensePanelHolder::showLicense()
{
    if (!parentComponent)
        return;

    panel = std::make_unique<LicensePanel>();
    parentComponent->addAndMakeVisible (panel.get());

    parentComponent->resized();
}

void LicensePanelHolder::updateLayout (juce::Rectangle<int> bounds)
{
    if (panel)
        panel->setBounds (bounds);
}
