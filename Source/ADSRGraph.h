/*
  ==============================================================================

    ADSRGraph.h
    Created: 2 Dec 2025 8:54:20pm
    Author:  Haza

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class ADSRGraph : public juce::Component
{
public:
    ADSRGraph()
    {
        setSize(300, 100);
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colour(0xFF1a1a1a)); //#1a1a1a grey color
        g.setColour(juce::Colours::darkgrey);

        g.drawRect(getLocalBounds(), 1);

        auto bounds = getLocalBounds().reduced(5);
        float width = bounds.getWidth();
        float height = bounds.getHeight();

        float totalTime = attackTime + decayTime + 0.3f + releaseTime; 
        float attackX = (attackTime / totalTime) * width;
        float decayX = attackX + (decayTime / totalTime) * width;
        float sustainX = decayX + (0.3f / totalTime) * width;
        float releaseX = width;

        juce::Path envelopePath;

        envelopePath.startNewSubPath(bounds.getX(), bounds.getBottom());
        envelopePath.lineTo(bounds.getX() + attackX, bounds.getY());
        float sustainY = bounds.getY() + (1.0f - sustainLevel) * height;
        envelopePath.lineTo(bounds.getX() + decayX, sustainY);
        envelopePath.lineTo(bounds.getX() + sustainX, sustainY);
        envelopePath.lineTo(bounds.getX() + releaseX, bounds.getBottom());

        juce::ColourGradient gradient(
            juce::Colour(0xFFeb4034), bounds.getX(), bounds.getCentreY(),
            juce::Colour(0xFFffff00), bounds.getRight(), bounds.getCentreY(),
            false
        );
        g.setGradientFill(gradient);
        g.strokePath(envelopePath, juce::PathStrokeType(2.0f));
    }

    void setAttack(float value)
    {
        attackTime = value;
        repaint();
    }

    void setDecay(float value)
    {
        decayTime = value;
        repaint();
    }

    void setSustain(float value)
    {
        sustainLevel = value;
        repaint();
    }

    void setRelease(float value)
    {
        releaseTime = value;
        repaint();
    }

private:
    float attackTime = 0.05f;
    float decayTime = 0.1f;
    float sustainLevel = 0.7f;
    float releaseTime = 0.1f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSRGraph)
};