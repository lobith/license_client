//
// Created by Daniel Walz on 12.08.24.
//

#include "LicensePanel.h"
#include "BinaryData.h"

#include <foleys_License.h>
#include <juce_audio_processors/juce_audio_processors.h>


LicensePanel::LicensePanel()
{
    const juce::Colour buttonColour { 55, 69, 129 };
    const juce::Colour labelColour { 40, 41, 46 };

    addAndMakeVisible (title);
    addAndMakeVisible (enterSerial);
    addAndMakeVisible (code);
    addAndMakeVisible (submit);
    addAndMakeVisible (status);
    addAndMakeVisible (demo);
    addAndMakeVisible (manualButton);
    addAndMakeVisible (websiteButton);
    addAndMakeVisible (copyright);

    title.setJustificationType (juce::Justification::centred);
    title.setColour (juce::Label::textColourId, juce::Colours::silver);
    title.setText (LicenseData::productName, juce::dontSendNotification);
    title.setFont (juce::Font (24.0f));

    code.setColour (juce::TextEditor::backgroundColourId, labelColour);
    code.setTextToShowWhenEmpty ("XXXX-YYYY-ZZZZ-WWWW", juce::Colours::grey);
    code.setJustification (juce::Justification::centred);

    submit.setColour (juce::TextButton::buttonColourId, buttonColour);
    submit.setColour (juce::TextButton::textColourOffId, juce::Colours::silver);
    demo.setColour (juce::TextButton::buttonColourId, buttonColour);
    demo.setColour (juce::TextButton::textColourOffId, juce::Colours::silver);

    enterSerial.setJustificationType (juce::Justification::centred);
    enterSerial.setColour (juce::Label::textColourId, juce::Colours::silver);
    enterSerial.setFont (juce::Font (18.0f));

    status.setJustificationType (juce::Justification::centred);
    status.setColour (juce::Label::textColourId, juce::Colours::silver);

    copyright.setJustificationType (juce::Justification::centred);
    copyright.setColour (juce::Label::textColourId, juce::Colours::silver);
    copyright.setText (LicenseData::copyright, juce::dontSendNotification);
    copyright.setFont (juce::Font (12.0f));

    auto manualImage = juce::DrawableComposite::createFromImageData (BinaryData::pdficon_svg, BinaryData::pdficon_svgSize);
    manualImage->replaceColour (juce::Colours::black, juce::Colours::silver);
    manualButton.setImages (manualImage.get());
    manualButton.setColour (juce::DrawableButton::backgroundColourId, juce::Colours::darkgrey);
    manualButton.onClick = [] { juce::URL (LicenseData::manualUrl).launchInDefaultBrowser(); };

    auto websiteImage = juce::DrawableComposite::createFromImageData (BinaryData::wwwicon_svg, BinaryData::wwwicon_svgSize);
    websiteImage->replaceColour (juce::Colours::black, juce::Colours::silver);
    websiteButton.setImages (websiteImage.get());
    websiteButton.setColour (juce::DrawableButton::backgroundColourId, juce::Colours::darkgrey);
    websiteButton.onClick = [] { juce::URL (LicenseData::buyUrl).launchInDefaultBrowser(); };

    juce::Component::SafePointer<LicensePanel> safePointer (this);
    license.onLicenseReceived = [safePointer]
    {
        juce::MessageManager::callAsync (
          [safePointer]
          {
              if (safePointer)
                  safePointer->update();
          });
    };

    demo.onClick = [this]
    {
        if (license.isAllowed())
        {
            if (onCloseRequest)
                onCloseRequest();
        }
        else if (license.canDemo())
        {
            license.startDemo();
        }
    };

    submit.onClick = [this]
    {
        if (!code.isEmpty())
        {
            license.activate ({ { "computer", juce::SystemStats::getComputerName().toRawUTF8() },
                                { "user", juce::SystemStats::getFullUserName().toRawUTF8() },
                                { "os", juce::SystemStats::getOperatingSystemName().toRawUTF8() },
                                { "host", juce::PluginHostType().getHostDescription() },
                                { "serial", code.getText().toRawUTF8() } });
        }
    };

    update();
}

void LicensePanel::update()
{
    demo.setEnabled (license.canDemo() || license.isAllowed());

    if (license.isActivated())
        demo.setButtonText (TRANS ("Plugin activated"));
    else if (license.isDemo())
        demo.setButtonText (TRANS ("Days to evaluate: ") + juce::String (license.demoDaysLeft()));
    else if (!license.canDemo())
        demo.setButtonText (TRANS ("Demo expired, please buy a license"));

    juce::String lastError (license.getLastErrorString());
    if (lastError.isNotEmpty())
        status.setText (lastError, juce::dontSendNotification);
    else if (license.isExpired())
    {
        char dateString[64];
        auto expiryDate = *license.expires();
        std::strftime (dateString, 64, "%d. %m %Y", std::localtime (&expiryDate));
        status.setText ("Your license expired on " + juce::String (dateString), juce::dontSendNotification);
    }
    else if (license.isActivated())
        status.setText ("", juce::sendNotification);
    else if (license.isDemo())
        status.setText ({}, juce::dontSendNotification);
    else if (license.canDemo())
        status.setText ("Hit the Demo button to start your free " + juce::String (license.demoDaysLeft()) + " days trial", juce::dontSendNotification);
    else
        status.setText ("If you bought a license enter your email and hit Activate", juce::dontSendNotification);
}

void LicensePanel::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black.withAlpha (0.8f));
}

void LicensePanel::resized()
{
    auto area = getLocalBounds().reduced (40);
    area      = area.withSizeKeepingCentre (std::min (area.getWidth(), 400), area.getHeight());

    title.setBounds (area.removeFromTop (40).reduced (80, 0));
    copyright.setBounds (area.removeFromBottom (30).withTrimmedTop (10));

    auto buttonArea = area.removeFromBottom (area.getHeight() / 4);
    manualButton.setBounds (buttonArea.removeFromLeft (buttonArea.getWidth() / 2).withTrimmedRight (10));
    websiteButton.setBounds (buttonArea.withTrimmedLeft (10));

    demo.setBounds (area.removeFromBottom (60).withSizeKeepingCentre (std::min (area.getWidth(), 250), 30));

    auto third = area.getHeight() / 3;
    enterSerial.setBounds (area.removeFromTop (third));
    status.setBounds (area.removeFromBottom (third));

    area.reduce (30, 0);
    submit.setBounds (area.removeFromRight (area.getWidth() / 4).reduced (10));
    code.setBounds (area.reduced (10));
}
