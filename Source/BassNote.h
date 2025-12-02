#pragma once
#include <JuceHeader.h>

class BassNote : public juce::SynthesiserVoice {
public:
	BassNote() 
	{
		adsr.setParameters(adsrparameters);
	}

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

		adsr.setSampleRate(getSampleRate());
		adsr.setParameters(adsrparameters);
		adsr.noteOn();
		isActive = true;

	}
	void stopNote(float velocity, bool allowTailOff) override
	{
		adsr.noteOff();
		if (!allowTailOff || !adsr.isActive()) {
			clearCurrentNote();
			isActive = false;
		}
	}

	void pitchWheelMoved(int newPitchWheelValue) override{}
	void controllerMoved(int controllerNumber, int newControllerValue) override {}

	void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
	{
		if (!isActive)
			return;
	
		adsr.setSampleRate(getSampleRate());

		while (numSamples > 0) {

			double phase = std::fmod(currentAngle, juce::MathConstants<double>::twoPi);

			float sample = std::sin(phase) * fundamentalAmp + std::sin(phase * 2.0) * harmonic2Amp;

			sample *= adsr.getNextSample();
			sample *= level;

			for (int chan = 0; chan < outputBuffer.getNumChannels(); ++chan)
				outputBuffer.addSample(chan, startSample, sample);

			currentAngle += angleDelta;
			while (currentAngle >= juce::MathConstants<double>::twoPi)
				currentAngle -= juce::MathConstants<double>::twoPi;

			startSample++;
			numSamples--;
		}
		if (!adsr.isActive())
		{
			clearCurrentNote();
			isActive = false;
		}
			
	}

	void setADSRParameters(float attack, float decay, float sustain, float release)
	{
		adsrparameters.attack = attack;
		adsrparameters.decay = decay;
		adsrparameters.sustain = sustain;
		adsrparameters.release = release;
		adsr.setParameters(adsrparameters);
	}

	void setHarmonics(float fund, float harm2)
	{
		fundamentalAmp = fund;
		harmonic2Amp = harm2;
	}


private:
	float level = 0.0f;
	double currentAngle = 0.0;
	double angleDelta = 0.0;
	double freq = 0.0;
	bool isActive = false;

	float fundamentalAmp = 0.06f;
	float harmonic2Amp = 0.2f;

	juce::ADSR adsr;
	juce::ADSR::Parameters adsrparameters{ 0.05f, 0.1f, 0.7f, 0.1f };

};
