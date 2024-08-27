//
// Created by Daniel Walz on 12.08.24.
//

#ifndef FOLEYS_LICENSING_CLIENT_LICENSEPANEL_H
#define FOLEYS_LICENSING_CLIENT_LICENSEPANEL_H

#include <juce_gui_basics/juce_gui_basics.h>
#include <foleys_License.h>

class LicensePanelHolder;

class LicensePanel
  : public juce::Component
  , public foleys::Licensing::Observer
{
public:
    explicit LicensePanel (LicensePanelHolder& holder);
    ~LicensePanel() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

    void update();

    void licenseLoaded() override { update(); }
    void licenseFetched() override { update(); }

private:
    LicensePanelHolder& panelHolder;
    juce::TextEditor    email;
    juce::Label         status;
    juce::TextButton    closeButton { TRANS ("Close"), TRANS ("Close panel") };
    juce::TextButton    demo { TRANS ("Start Demo"), TRANS ("Start your 14 days free trial period") };
    juce::TextButton    activate { TRANS ("Activate"), TRANS ("Request an activation from the server (need to be online)") };
    juce::TextButton    refresh { TRANS ("Refresh"), TRANS ("Check activation status again") };
    juce::TextButton    manage { TRANS ("Manage"), TRANS ("Open the license management website") };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LicensePanel)
};

class LicensePanelHolder
{
public:
    explicit LicensePanelHolder (juce::Component* parent);

    void showLicense();
    void closePanel();
    void updateLayout (juce::Rectangle<int> bounds);

private:
    juce::Component::SafePointer<juce::Component> parentComponent;
    std::unique_ptr<LicensePanel>                 panel;
};


#endif  // FOLEYS_LICENSING_CLIENT_LICENSEPANEL_H
