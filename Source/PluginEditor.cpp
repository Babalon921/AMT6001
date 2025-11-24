/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AMT6001AudioProcessorEditor::AMT6001AudioProcessorEditor (AMT6001AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 300);
    // these define the parameters of our slider object
    midiVolume.setSliderStyle(juce::Slider::LinearBarVertical);
    midiVolume.setRange(0.0, 127.0, 1.0);
    midiVolume.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    midiVolume.setColour(juce::Slider::trackColourId, juce::Colours::red);
    midiVolume.setPopupDisplayEnabled(true, false, this);
    midiVolume.setTextValueSuffix(" Volume");
    midiVolume.setValue(1.0);
    // this function adds the slider to the editor
    addAndMakeVisible(&midiVolume);
    addAndMakeVisible(virtualKeyboard);
}

AMT6001AudioProcessorEditor::~AMT6001AudioProcessorEditor()
{
}

//==============================================================================
void AMT6001AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    // set the current drawing colour to black
    g.setColour(juce::Colours::white);
    // set the font size and draw text to the screen
    g.setFont(15.0f);
    g.drawFittedText("Midi Volume", 0, 0, getWidth(), 30, juce::Justification::centred, 1);

}

void AMT6001AudioProcessorEditor::resized()
{
    midiVolume.setBounds(40, 30, 20, getHeight() - 60);
    virtualKeyboard.setBounds(80, getHeight() - 100, getWidth() - 100, 80);
}
