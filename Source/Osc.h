/*
  ==============================================================================

    Osc.h
    Created: 8 Dec 2025 9:41:19pm
    Author:  Haza

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Oscilloscope : public juce::Component, public juce::Timer
{
public:
    Oscilloscope()
    {
        setSize(300, 200);
        audioPoints.resize(512, 0.0f); //pre allocate due to error
        startTimerHz(60); 
    }

    ~Oscilloscope() override
    {
        stopTimer();
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colour(0xFF1a1a1a));
        
        g.setColour(juce::Colours::darkgrey);
        g.drawRect(getLocalBounds(), 1);

        auto bounds = getLocalBounds().reduced(10);
        float width = bounds.getWidth();
        float height = bounds.getHeight();
        float centerY = bounds.getCentreY();

        g.setColour(juce::Colour(0xFF2a2a2a));
        g.drawLine(bounds.getX(), centerY, bounds.getRight(), centerY, 1.0f);

        if (audioPoints.size() > 1)
        {
            juce::Path waveformPath;
            
            juce::ColourGradient gradient(
                juce::Colour(0xFFeb4034), bounds.getX(), centerY,
                juce::Colour(0xFFffff00), bounds.getRight(), centerY,
                false
            );
            g.setGradientFill(gradient);

            bool started = false;
            for (int i = 0; i < audioPoints.size(); ++i)
            {
                int index = (writePosition + i) % audioPoints.size();
                float x = bounds.getX() + (i / (float)audioPoints.size()) * width;
                float y = centerY - (audioPoints[index] * (height / 2.0f) * 0.9f);

                if (!started)
                {
                    waveformPath.startNewSubPath(x, y);
                    started = true;
                }
                else
                {
                    waveformPath.lineTo(x, y);
                }
            }

            g.strokePath(waveformPath, juce::PathStrokeType(2.0f));
        }

        g.setColour(juce::Colours::darkgrey);
        g.setFont(10.0f);
        g.drawText("OSC", bounds.getX(), bounds.getY() - 8, 50, 10, juce::Justification::left);
    }

    void timerCallback() override
    {
        repaint();
    }

    void pushBuffer(const juce::AudioBuffer<float>& buffer)
    {
        if (buffer.getNumChannels() == 0 || buffer.getNumSamples() == 0)
            return;

        const int numSamples = buffer.getNumSamples();
        const float* channelData = buffer.getReadPointer(0);

        const int displayPoints = 512;
        const int samplesPerPoint = juce::jmax(1, numSamples / displayPoints);

        juce::ScopedLock lock(audioBufferLock);

        for (int i = 0; i < numSamples; i += samplesPerPoint)
        {
            if (i < numSamples)
            {
                audioPoints[writePosition] = channelData[i];
                writePosition = (writePosition + 1) % audioPoints.size();
            }
        }
    }
    void clear()
    {
        juce::ScopedLock lock(audioBufferLock);
        std::fill(audioPoints.begin(), audioPoints.end(), 0.0f);
        writePosition = 0;
    }

private:
        std::vector<float> audioPoints;
    int writePosition = 0;
    juce::CriticalSection audioBufferLock;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Oscilloscope)
};



//bug fixed:
// VECTOR ERROR ==> PREALOCATED NO DYNAMIC SIZING
// WRITE POSTION ==> WRAPS INSTEAD OF ERASES
// WRITE POSITION ==> PRVENTS OUT OF ACCESS VECTOR
// BUG FIXED BY CONTAING VECTTOR