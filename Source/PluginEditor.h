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

    // Changed Value Area
    void sliderValueChanged(juce::Slider*) override;

    AMT6001AudioProcessor& audioProcessor;

    //Slider Defs
    juce::Slider midiVolume;

    //adsr
    juce::Slider attack_Dial;
    juce::Slider decay_Dial;
    juce::Slider sustain_Dial;
    juce::Slider release_Dial;

    //harmonics 
    juce::Slider fund;
    juce::Slider harmonic2;

    //labels
    juce::Label attack_label;
    juce::Label decay_label;
    juce::Label sustain_label;
    juce::Label release_label;
    juce::Label fund_label;
    juce::Label harmonic2_label;


    //midi keyboard
    juce::MidiKeyboardComponent virtualKeyboard{ audioProcessor.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard };


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AMT6001AudioProcessorEditor)
};
