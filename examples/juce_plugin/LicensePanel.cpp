//
// Created by Daniel Walz on 12.08.24.
//

#include "LicensePanel.h"

#include <foleys_License.h>
#include <juce_audio_processors/juce_audio_processors.h>


LicensePanel::LicensePanel (LicensePanelHolder& holder) : panelHolder (holder)
{
    foleys::Licensing::Ptr()->syncLicense();

    addAndMakeVisible (email);
    addAndMakeVisible (status);
    addAndMakeVisible (closeButton);
    addAndMakeVisible (demo);
    addAndMakeVisible (activate);
    addAndMakeVisible (refresh);
    addAndMakeVisible (manage);

    email.setTextToShowWhenEmpty ("email for activation", juce::Colours::grey);

    closeButton.onClick = [this]
    {
        if (foleys::Licensing::Ptr()->isAllowed())
            panelHolder.closePanel();
    };

    refresh.onClick = [] { foleys::Licensing::Ptr()->syncLicense(); };

    demo.onClick = [] { foleys::Licensing::Ptr()->startDemo(); };

    activate.onClick = [this]
    {
        if (email.isEmpty())
        {
            status.setText ("Please enter the email you used to purchase the license", juce::sendNotification);
            return;
        }

        foleys::Licensing::Ptr licensing;
        licensing->activate ({ { "computer", juce::SystemStats::getComputerName().toRawUTF8() },
                               { "user", juce::SystemStats::getFullUserName().toRawUTF8() },
                               { "os", juce::SystemStats::getOperatingSystemName().toRawUTF8() },
                               { "host", juce::PluginHostType().getHostDescription() },
                               { "email", email.getText().toLowerCase().toRawUTF8() } });
    };

    update();
}

void LicensePanel::update()
{
    foleys::Licensing::Ptr licensing;
    demo.setEnabled (licensing->canDemo());

    closeButton.setVisible (licensing->isAllowed());

    if (!licensing->getLastError().empty())
        status.setText (licensing->getLastError(), juce::sendNotification);
    else if (licensing->expired())
    {
        char dateString[64];
        auto expiryDate = *licensing->expires();
        std::strftime (dateString, 64, "%d. %m %Y", std::localtime (&expiryDate));
        status.setText ("Your license expired on " + juce::String (dateString), juce::sendNotification);
    }
    else if (licensing->activated())
        status.setText ("Plugin activated to " + licensing->getLicenseeEmail(), juce::sendNotification);
    else if (licensing->isDemo())
        status.setText ("Your demo will expire in " + juce::String (licensing->demoDaysLeft()) + " days", juce::sendNotification);
    else if (licensing->canDemo())
        status.setText ("Hit the Demo button to start your free " + juce::String (licensing->demoDaysLeft()) + " days trial", juce::sendNotification);
    else
        status.setText ("If you bought a license enter your email and hit Activate", juce::sendNotification);
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
    closeButton.setBounds (getWidth() - 80, 0, 80, 20);

    auto area = getLocalBounds();
    status.setBounds (area.removeFromBottom (40).reduced (20, 5));
    auto buttonRow = area.removeFromBottom (70);

    juce::FlexBox flexBox;
    flexBox.items = {
        juce::FlexItem (demo).withFlex (1.0).withMargin (20),
        juce::FlexItem (activate).withFlex (1.0).withMargin (20),
        juce::FlexItem (refresh).withFlex (1.0).withMargin (20),
        juce::FlexItem (manage).withFlex (1.0).withMargin (20),
    };

    flexBox.performLayout (buttonRow);

    email.setBounds (area.removeFromBottom (70).reduced (20));
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
