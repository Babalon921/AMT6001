/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AMT6001AudioProcessorEditor::AMT6001AudioProcessorEditor(AMT6001AudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(600, 300);

    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::darkred);
    getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colours::crimson);
    getLookAndFeel().setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::darkred);
    getLookAndFeel().setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
    getLookAndFeel().setColour(juce::Label::textColourId, juce::Colours::darkgrey);

    // volume slidera
    midiVolume.setSliderStyle(juce::Slider::LinearBarVertical);
    midiVolume.setRange(0.0, 127.0, 1.0);
    midiVolume.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    midiVolume.setColour(juce::Slider::trackColourId, juce::Colours::red);
    midiVolume.setPopupDisplayEnabled(true, false, this);
    midiVolume.setTextValueSuffix(" Volume");
    midiVolume.setValue(100.0);
    midiVolume.addListener(this);
    // this function adds the slider to the editor
    addAndMakeVisible(&midiVolume);

    //attack dial
    attack_Dial.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    attack_Dial.setRange(0.001, 2.0, 0.001);
    attack_Dial.setValue(0.05);
    attack_Dial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 30, 10);
    attack_Dial.setPopupDisplayEnabled(true, false, this);
    attack_Dial.setTextValueSuffix(" s");
    attack_Dial.setNumDecimalPlacesToDisplay(2);
    attack_Dial.addListener(this);
    addAndMakeVisible(&attack_Dial);

    attack_label.setText("Attack", juce::dontSendNotification);
    attack_label.setJustificationType(juce::Justification::centred);
    attack_label.attachToComponent(&attack_Dial, false);
    addAndMakeVisible(&attack_label);

    //decay dial
    decay_Dial.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    decay_Dial.setRange(0.001, 2.0, 0.001);
    decay_Dial.setValue(0.1);
    decay_Dial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 30, 10);
    decay_Dial.setPopupDisplayEnabled(true, false, this);
    decay_Dial.setTextValueSuffix(" s");
    decay_Dial.setNumDecimalPlacesToDisplay(2);

    decay_Dial.addListener(this);
    addAndMakeVisible(&decay_Dial);

    decay_label.setText("Decay", juce::dontSendNotification);
    decay_label.setJustificationType(juce::Justification::centred);
    decay_label.attachToComponent(&decay_Dial, false);
    addAndMakeVisible(&decay_label);

    //sustain
    sustain_Dial.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    sustain_Dial.setRange(0.0, 1.0, 0.01);
    sustain_Dial.setValue(0.7);
    sustain_Dial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 30, 10);
    sustain_Dial.setPopupDisplayEnabled(true, false, this);
    sustain_Dial.setNumDecimalPlacesToDisplay(2);
    sustain_Dial.addListener(this);
    addAndMakeVisible(&sustain_Dial);

    sustain_label.setText("Sustain", juce::dontSendNotification);
    sustain_label.setJustificationType(juce::Justification::centred);
    sustain_label.attachToComponent(&sustain_Dial, false);
    addAndMakeVisible(&sustain_label);

    //release
    release_Dial.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    release_Dial.setRange(0.001, 5.0, 0.001);
    release_Dial.setValue(0.1);
    release_Dial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 30, 10);
    release_Dial.setPopupDisplayEnabled(true, false, this);
    release_Dial.setTextValueSuffix(" s");
    release_Dial.setNumDecimalPlacesToDisplay(2);
    release_Dial.addListener(this);
    addAndMakeVisible(&release_Dial);

    release_label.setText("Release", juce::dontSendNotification);
    release_label.setJustificationType(juce::Justification::centred);
    release_label.attachToComponent(&release_Dial, false);
    addAndMakeVisible(&release_label);

    //fundemental
    fund.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    fund.setRange(0.0, 1.0, 0.01);
    fund.setValue(0.06);
    fund.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 30, 10);
    fund.setPopupDisplayEnabled(true, false, this);
    fund.setNumDecimalPlacesToDisplay(2);
    fund.addListener(this);
    addAndMakeVisible(&fund);

    fund_label.setText("Fundamental", juce::dontSendNotification);
    fund_label.setJustificationType(juce::Justification::centred);
    fund_label.attachToComponent(&fund, false);
    addAndMakeVisible(&fund_label);

    //harmonics2 
    harmonic2.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    harmonic2.setRange(0.0, 1.0, 0.01);
    harmonic2.setValue(0.2);
    harmonic2.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 30, 10);
    harmonic2.setPopupDisplayEnabled(true, false, this);
    harmonic2.setNumDecimalPlacesToDisplay(2);
    harmonic2.addListener(this);
    addAndMakeVisible(&harmonic2);

    harmonic2_label.setText("2nd Harmonic", juce::dontSendNotification);
    harmonic2_label.setJustificationType(juce::Justification::centred);
    harmonic2_label.attachToComponent(&harmonic2, false);
    addAndMakeVisible(&harmonic2_label);

    //distortion
    distortion_Dial.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    distortion_Dial.setRange(0.0, 1.0, 0.01);
    distortion_Dial.setValue(0.0);
    distortion_Dial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 30, 10);
    distortion_Dial.setPopupDisplayEnabled(true, false, this);
    distortion_Dial.setNumDecimalPlacesToDisplay(2);
    distortion_Dial.addListener(this);
    addAndMakeVisible(&distortion_Dial);

    distortion_label.setText("Distortion", juce::dontSendNotification);
    distortion_label.setJustificationType(juce::Justification::centred);
    distortion_label.attachToComponent(&distortion_Dial, false);
    addAndMakeVisible(&distortion_label);

    //adsr graph
    addAndMakeVisible(adsrGraph);
    //osc
    addAndMakeVisible(oscilloscope);
    //keyboard
    addAndMakeVisible(virtualKeyboard);
}

AMT6001AudioProcessorEditor::~AMT6001AudioProcessorEditor()
{
    midiVolume.removeListener(this);
    attack_Dial.removeListener(this);
    decay_Dial.removeListener(this);
    sustain_Dial.removeListener(this);
    release_Dial.removeListener(this);
    fund.removeListener(this);
    harmonic2.removeListener(this);
    distortion_Dial.removeListener(this);
}

//==============================================================================
void AMT6001AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(18.0f);
    g.drawFittedText("Volume", 5, 10, getWidth(), 10, juce::Justification::left, 1);

    g.setFont(14.0f);
    g.setColour(juce::Colours::red);
    g.drawText("HGRAY-10783813", 480, 0, 200, 20, juce::Justification::left);



}

void AMT6001AudioProcessorEditor::resized()
{
    midiVolume.setBounds(20, 30, 20, getHeight() - 60); 

    int dialY = 30;
    int dialSize = 55;

    attack_Dial.setBounds(80, dialY, dialSize, dialSize);
    decay_Dial.setBounds(140, dialY, dialSize, dialSize);
    sustain_Dial.setBounds(200, dialY, dialSize, dialSize);
    release_Dial.setBounds(260, dialY, dialSize, dialSize);

    fund.setBounds(320, dialY, dialSize, dialSize);
    harmonic2.setBounds(380, dialY, dialSize, dialSize);
    //460, dialY + 110, dialSize, dialSize << for reverb later
    distortion_Dial.setBounds(520, dialY + 110, dialSize, dialSize);
    
    adsrGraph.setBounds(80, dialY + 65, 350, 100);

    oscilloscope.setBounds(450, 15, 140, 100);

    virtualKeyboard.setBounds(80, getHeight() - 100, getWidth() - 100, 80);
}

void AMT6001AudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &midiVolume)
    {
        audioProcessor.setMasterVolume(slider->getValue() / 127.0f);
    }
    else if (slider == &attack_Dial)
    {
        audioProcessor.setAttack(slider->getValue());
        adsrGraph.setAttack(slider->getValue());
    }
    else if (slider == &decay_Dial)
    {
        audioProcessor.setDecay(slider->getValue());
        adsrGraph.setDecay(slider->getValue());
    }
    else if (slider == &sustain_Dial)
    {
        audioProcessor.setSustain(slider->getValue());
        adsrGraph.setSustain(slider->getValue());
    }
    else if (slider == &release_Dial)
    {
        audioProcessor.setRelease(slider->getValue());
        adsrGraph.setRelease(slider->getValue());
    }
    else if (slider == &fund)
    {
        audioProcessor.setFundamental(slider->getValue());
    }
    else if (slider == &harmonic2)
    {
        audioProcessor.setHarmonic2(slider->getValue());
    }
    else if (slider == &distortion_Dial)
    {
        audioProcessor.setDistortion(slider->getValue());
    }
}