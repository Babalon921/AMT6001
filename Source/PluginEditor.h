/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ADSRGraph.h"
#include "Osc.h"

//=============================================================================
/**
*/
class AMT6001AudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Slider::Listener, private juce::Button::Listener
{
public:
    AMT6001AudioProcessorEditor (AMT6001AudioProcessor&);
    ~AMT6001AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    ADSRGraph adsrGraph;
    Oscilloscope oscilloscope;

private:

    //OSC
   
    // Changed Value Area
    void sliderValueChanged(juce::Slider*) override;
    void buttonClicked(juce::Button*) override;

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
    juce::Slider harmonic2Detune_Dial;


    juce::Slider distortion_Dial;

    //labels
    juce::Label attack_label;
    juce::Label decay_label;
    juce::Label sustain_label;
    juce::Label release_label;
    juce::Label fund_label;
    juce::Label harmonic2_label;
    juce::Label distortion_label;
    juce::Label harmonic2Detune_label;
    //text button
    juce::TextButton turboButton;

    //midi keyboard
    juce::MidiKeyboardComponent virtualKeyboard{ audioProcessor.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard };


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AMT6001AudioProcessorEditor)
};
