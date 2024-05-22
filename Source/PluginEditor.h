/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class JuceAllpassFilterPluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    JuceAllpassFilterPluginAudioProcessorEditor (JuceAllpassFilterPluginAudioProcessor&, 
        juce::AudioProcessorValueTreeState& vts);
    ~JuceAllpassFilterPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JuceAllpassFilterPluginAudioProcessor& audioProcessor;

    //custom members
    juce::Slider cutoffFrequencySlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        cutoffFrequencyAttachment; //connect active value to UI element
    juce::Label cutoffFrequencyLabel; //ui element

    juce::ToggleButton highpassButton; //bool checkbox
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>
        highpassAttachment; //link to value again
    juce::Label highpassButtonLabel; //ui element

    //default below
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JuceAllpassFilterPluginAudioProcessorEditor)
};
