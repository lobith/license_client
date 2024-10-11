//
// Created by Daniel Walz on 11.10.24.
//

#include "LicenseDeactivate.h"


LicenseDeactivate::LicenseDeactivate()
{
    activations = license.getActivations();
    setRowHeight (48);
    setModel (this);
}

LicenseDeactivate::~LicenseDeactivate()
{
    setModel (nullptr);
}

int LicenseDeactivate::getNumRows()
{
    return static_cast<int> (activations.size());
}

class DeactivateItem : public juce::Component
{
public:
    DeactivateItem (LicenseDeactivate& ownerToUse) : owner (ownerToUse) { addAndMakeVisible (deactivate); }

    void setActivation (const foleys::Licensing::Activation& activationToUse)
    {
        activation         = activationToUse;
        deactivate.onClick = [this]
        {
            if (owner.onDeactivate)
                owner.onDeactivate (activation.index);
        };
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::black);
        g.setColour (juce::Colours::silver);
        g.setFont (20.0f);
        g.drawFittedText (activation.computer, 10, 4, getWidth() - 110, 24, juce::Justification::left, 1);
        g.setFont (14.0f);
        g.drawFittedText (activation.user, 10, 28, getWidth() - 110, 18, juce::Justification::left, 1);
    }

    void resized() override { deactivate.setBounds (getRight() - 90, 6, 80, 24); }

private:
    LicenseDeactivate&            owner;
    foleys::Licensing::Activation activation;
    juce::TextButton              deactivate { "deactivate", "Deactivate" };
};

juce::Component* LicenseDeactivate::refreshComponentForRow (int rowNumber, bool isRowSelected, juce::Component* existingComponentToUpdate)
{
    if (auto* comp = dynamic_cast<DeactivateItem*> (existingComponentToUpdate))
    {
        comp->setActivation (activations[size_t (rowNumber)]);
        return comp;
    }

    if (existingComponentToUpdate)
        delete (existingComponentToUpdate);

    auto* comp = new DeactivateItem (*this);
    comp->setActivation (activations[size_t (rowNumber)]);
    return comp;
}
