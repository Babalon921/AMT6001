/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "BassNote.h"
#include "BassSound.h"

//==============================================================================
AMT6001AudioProcessor::AMT6001AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
#endif
{
    synth.clearVoices();

    for (int i = 0; i < 8; i++)
        synth.addVoice(new BassNote());

    synth.clearSounds();
    synth.addSound(new BassSound());
}

AMT6001AudioProcessor::~AMT6001AudioProcessor()
{
}

//==============================================================================
const juce::String AMT6001AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AMT6001AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AMT6001AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AMT6001AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AMT6001AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AMT6001AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AMT6001AudioProcessor::getCurrentProgram()
{
    return 0;
}

void AMT6001AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AMT6001AudioProcessor::getProgramName (int index)
{
    return {};
}

void AMT6001AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AMT6001AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
synth.setCurrentPlaybackSampleRate(sampleRate);
}

void AMT6001AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AMT6001AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AMT6001AudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
    
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<BassNote*>(synth.getVoice(i)))
        {
            voice->setADSRParameters(attack, decay, sustain, release);
            voice->setHarmonics(fundamentalAmp, harmonic2Amp);
            voice->setDistortion(distortionAmount);
            voice->setHarmonic2Detune(harmonic2Detune);
        }
    }

    
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    if (auto* editor = dynamic_cast<AMT6001AudioProcessorEditor*>(getActiveEditor()))
    {
        editor->oscilloscope.pushBuffer(buffer);
    }

    buffer.applyGain(masterVolume);
}


//==============================================================================
bool AMT6001AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AMT6001AudioProcessor::createEditor()
{
    return new AMT6001AudioProcessorEditor(*this);
}

//==============================================================================
void AMT6001AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AMT6001AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AMT6001AudioProcessor();
}
