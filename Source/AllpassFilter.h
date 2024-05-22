#pragma once
#include <vector>
#include "JuceHeader.h"

class AllpassFilter
{
// setters for params of filtering
// processing block fn for filtering algo

public:
	void setHighpass(bool highpass); //false for lowpass
	void setCutoffFrequency(float cutoffFrequency);
	void setSamplingRate(float samplingRate);

	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&); //filter logic



private:
	bool highpass;
	float cutoffFrequency;
	float samplingRate;
	std::vector<float> dnBuffer; //filter buffers for each channel=1
};
