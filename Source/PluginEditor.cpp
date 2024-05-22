/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JuceAllpassFilterPluginAudioProcessorEditor::JuceAllpassFilterPluginAudioProcessorEditor (
    JuceAllpassFilterPluginAudioProcessor& p,
    juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(cutoffFrequencySlider);
    cutoffFrequencySlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    cutoffFrequencyAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        vts, "cutoff_frequency", cutoffFrequencySlider));

    addAndMakeVisible(cutoffFrequencyLabel);
    cutoffFrequencyLabel.setText("Cutoff Frequency", juce::dontSendNotification);

    addAndMakeVisible(highpassButton);
    highpassAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(
        vts, "highpass", highpassButton));

    addAndMakeVisible(highpassButtonLabel);
    highpassButtonLabel.setText("Highpass", juce::dontSendNotification);

    setSize(200, 400); 
}

JuceAllpassFilterPluginAudioProcessorEditor::~JuceAllpassFilterPluginAudioProcessorEditor()
{
}

//==============================================================================
void JuceAllpassFilterPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillAll (juce::Colours::darkseagreen);

    //g.setColour (juce::Colours::slateblue);
    //g.setFont (15.0f);
    //g.drawFittedText ("AllpassFilter Test", getLocalBounds(), juce::Justification::centred, 1);
}

void JuceAllpassFilterPluginAudioProcessorEditor::resized()
{
    cutoffFrequencySlider.setBounds({ 15, 35, 100, 300 });
    cutoffFrequencyLabel.setBounds({ cutoffFrequencySlider.getX() + 30,
                                    cutoffFrequencySlider.getY() - 30,
                                    200, 50 });
    highpassButton.setBounds({ cutoffFrequencySlider.getX(),
                                cutoffFrequencySlider.getY() + cutoffFrequencySlider.getHeight() + 15,
                                30, 
                                50 });
    highpassButtonLabel.setBounds({ cutoffFrequencySlider.getX() + highpassButton.getWidth() + 15,
                                highpassButton.getY(), 
                                cutoffFrequencySlider.getWidth() - highpassButton.getWidth(),
                                highpassButton.getHeight() });

}
