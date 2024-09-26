//
// Created by Daniel Walz on 12.08.24.
//

#ifndef FOLEYS_LICENSING_CLIENT_LICENSEPANEL_H
#define FOLEYS_LICENSING_CLIENT_LICENSEPANEL_H

#include "juce/foleys_PopupHolder.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <foleys_License.h>

class LicensePanelHolder;

class LicensePanel : public foleys::Popup
{
public:
    explicit LicensePanel();

    void paint (juce::Graphics& g) override;
    void resized() override;

    void update();

private:
    foleys::License license;

    juce::Label          title;
    juce::Label          enterSerial { {}, TRANS ("ENTER SERIAL") };
    juce::TextEditor     code;
    juce::TextButton     submit { TRANS ("ENTER"), TRANS ("Submit code") };
    juce::Label          status;
    juce::TextButton     demo { TRANS ("Start Demo"), TRANS ("Start your 14 days free trial period") };
    juce::DrawableButton manualButton { "Manual Guide", juce::DrawableButton::ButtonStyle::ImageAboveTextLabel };
    juce::DrawableButton homeButton { "My Licenses", juce::DrawableButton::ButtonStyle::ImageAboveTextLabel };
    juce::DrawableButton websiteButton { "Buy License", juce::DrawableButton::ButtonStyle::ImageAboveTextLabel };
    juce::Label          copyright;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LicensePanel)
};


#endif  // FOLEYS_LICENSING_CLIENT_LICENSEPANEL_H
