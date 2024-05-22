#include "AllpassFilter.h"

void AllpassFilter::setHighpass(bool highpass) {
	this->highpass = highpass;
}
void AllpassFilter::setCutoffFrequency(float cutoffFrequency) {
	this->cutoffFrequency = cutoffFrequency;
}
void AllpassFilter::setSamplingRate(float samplingRate) {
	this->samplingRate = samplingRate;
}

void AllpassFilter::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) {
	//unused MidiBuffer arg
	constexpr auto PI = 3.14159265359f;

	dnBuffer.resize(buffer.getNumChannels(), 0.f); //resize buf to num of channels=1, each new entry set to 0

	const auto sign = highpass ? -1.f : 1.f; //if highpass, invert phase

	const auto tan = std::tan(PI * cutoffFrequency / samplingRate); //helper var for allpass coeff
	const auto a1 = (tan - 1.f) / (tan + 1.f); //allpass coeff constant within block proc

	for (auto channel = 0; channel < buffer.getNumChannels(); ++channel) {
		auto channelSamples = buffer.getWritePointer(channel); //write pointer to modify samples
		for (auto i = 0; i < buffer.getNumChannels(); ++i) { //for each sample in channel
			const auto inputSample = channelSamples[i];
			const auto allpassFilteredSample = a1 * inputSample + dnBuffer[channel]; //do allpass filtering
			dnBuffer[channel] = inputSample - a1 * allpassFilteredSample;

			//final filtering and scale by 0.5 for clip protection
			const auto filterOutput = 0.5f * (inputSample + sign * allpassFilteredSample);

			channelSamples[i] = filterOutput; // set as output
		}
	}
}