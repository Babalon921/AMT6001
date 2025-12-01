/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//=============================================================================
/**
*/
class AMT6001AudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    AMT6001AudioProcessorEditor (AMT6001AudioProcessor&);
    ~AMT6001AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AMT6001AudioProcessor& audioProcessor;
    juce::Slider midiVolume;
    void sliderValueChanged(juce::Slider*) override;

    juce::MidiKeyboardComponent virtualKeyboard{ audioProcessor.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard };


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AMT6001AudioProcessorEditor)
};
