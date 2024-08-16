//
// Created by Daniel Walz on 12.08.24.
//

#include "LicensePanel.h"

#include <foleys_License.h>


LicensePanel::LicensePanel (LicensePanelHolder& holder) : panelHolder (holder)
{
    foleys::Licensing::Ptr licensing;

    addAndMakeVisible (demo);
    addAndMakeVisible (activate);
    addAndMakeVisible (refresh);
    addAndMakeVisible (manage);

    refresh.onClick = [this]
    {
        foleys::Licensing::Ptr licensing;
        licensing->syncLicense();
    };

    demo.setEnabled (licensing->canDemo());
    demo.onClick = [this]
    {
        foleys::Licensing::Ptr licensing;
        licensing->startDemo();
    };

    activate.onClick = [this]
    {
        // TODO: check if close is allowed
        panelHolder.closePanel();
    };
}

void LicensePanel::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black.withAlpha (0.8f));

    foleys::Licensing::Ptr licensing;
    if (licensing->isDemo())
    {
        g.setColour (juce::Colours::silver);
        g.drawFittedText ("Your demo will expire in " + juce::String (licensing->demoDaysLeft()) + " days", getLocalBounds(), juce::Justification::centred, 1);
    }
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

    panel = std::make_unique<LicensePanel> (*this);
    parentComponent->addAndMakeVisible (panel.get());

    parentComponent->resized();
}

void LicensePanelHolder::closePanel()
{
    panel.reset();
}

void LicensePanelHolder::updateLayout (juce::Rectangle<int> bounds)
{
    if (panel)
        panel->setBounds (bounds);
}
