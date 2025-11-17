#pragma once
#include <JuceHeader.h>

class BaseNote : public juce::SynthesiserVoice {
public:
	bool canPlaySound(juce::SynthesiserSound* sound) override
	{
		return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
	}
	void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int pitchWheel) override
	{
		level = velocity * 0.5f;

	}


private:
	float level = 0.0f;
	double currentAngle = 0.0;
	double angleDelta = 0.0;
	double frequency = 0.0;
	bool isActive = false;


};
