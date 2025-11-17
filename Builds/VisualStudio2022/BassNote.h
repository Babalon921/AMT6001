#pragma once
#include <JuceHeader.h>

class BassNote : public juce::SynthesiserVoice {
public:
	bool canPlaySound(juce::SynthesiserSound* sound) override
	{
		return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
	}
	void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int pitchWheel) override
	{
		level = velocity * 0.5f;
		freq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		currentAngle = 0.0;
		angleDelta = freq * 2.0 * juce::MathConstants<double>::pi / getSampleRate();
		isActive = true;
		adsr.noteOn();
	}
	void stopNote(float velocity, bool allowTailOff) override
	{
		adsr.noteOff();
		if (!allowTailOff || adsr.isActive()) {
			isActive = false;
		}
	}
	void pitchWheelMoved(int, int) {}
	void controllerMoved(int, int) {}
	void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
	{
		if (!isActive)
			return;
	
		adsr.setSampleRate(getSampleRate());

		while (numSamples > 0) {
			float sample = std::sin(currentAngle) * 0.06f + std::sin(currentAngle * 2.0) * 0.2f;

			sample *= adsr.getNextSample();
			sample *= level;

			for (int chan = 0; chan < outputBuffer.getNumChannels(); ++chan)
				outputBuffer.addSample(chan, startSample, sample);

			currentAngle += angleDelta;
			startSample++;
			numSamples--;

		}
		if (!adsr.isActive())
			isActive = false;

	}




private:
	float level = 0.0f;
	double currentAngle = 0.0;
	double angleDelta = 0.0;
	double freq = 0.0;
	bool isActive = false;

	juce::ADSR adsr;
	juce::ADSR::Parameters adsrparameters { 0.01f, 0.1f, 0.7f, 0.1f };

};
