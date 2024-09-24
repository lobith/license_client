//
// Created by Daniel Walz on 20.09.24.
//

#ifndef FOLEYS_LICENSING_CLIENT_FOLEYS_POPUPHOLDER_H
#define FOLEYS_LICENSING_CLIENT_FOLEYS_POPUPHOLDER_H

#include <juce_gui_basics/juce_gui_basics.h>

namespace foleys
{

class Popup : public juce::Component
{
public:
    using juce::Component::Component;

    std::function<void()> onCloseRequest;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Popup)
};


class PopupHolder : private juce::ComponentListener
{
public:
    explicit PopupHolder (juce::Component* parent) : parentComponent (parent) { parentComponent->addComponentListener (this); }

    ~PopupHolder() override
    {
        if (parentComponent)
            parentComponent->removeComponentListener (this);
    }

    void showPopup (std::unique_ptr<Popup> popupToShow)
    {
        if (!parentComponent)
            return;

        popup = std::move (popupToShow);
        parentComponent->addAndMakeVisible (popup.get());

        popup->setBounds (parentComponent->getLocalBounds());
        popup->onCloseRequest = [this] { closePopup(); };
    }

    void closePopup() { popup.reset(); }

    void componentMovedOrResized (juce::Component& component, [[maybe_unused]] bool wasMoved, [[maybe_unused]] bool wasResized) override
    {
        if (popup && parentComponent)
            popup->setBounds (component.getLocalBounds());
    }

private:
    juce::Component::SafePointer<juce::Component> parentComponent;
    std::unique_ptr<Popup>                        popup;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PopupHolder)
};


}  // namespace foleys

#endif  // FOLEYS_LICENSING_CLIENT_FOLEYS_POPUPHOLDER_H
