/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JuceAllpassFilterPluginAudioProcessor::JuceAllpassFilterPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
            .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
#endif
            .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
            ),
#else
    :
#endif
    parameters(*this, nullptr, juce::Identifier("JuceAllpassFilterPlugin"),
        { std::make_unique<juce::AudioParameterFloat>(
                "cutoff_frequency", // in hz, range 20 to 20k with step=0.1
                "Cutoff Frequency",
                juce::NormalisableRange{20.f,
                                        20000.f,
                                        0.1f,
                                        0.2f, //skewFactor=0.2 for log scaling
                                        false}, 
                500.f),
           std::make_unique<juce::AudioParameterBool>("highpass", "Highpass", false) //false for lowpass, convert float to bool
        }){ //end init list
        cutoffFrequencyParameter = parameters.getRawParameterValue("cutoff_frequency");
        highpassParameter = parameters.getRawParameterValue("highpass");
    }

JuceAllpassFilterPluginAudioProcessor::~JuceAllpassFilterPluginAudioProcessor()
{
}

//==============================================================================
const juce::String JuceAllpassFilterPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JuceAllpassFilterPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JuceAllpassFilterPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JuceAllpassFilterPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JuceAllpassFilterPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JuceAllpassFilterPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JuceAllpassFilterPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JuceAllpassFilterPluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String JuceAllpassFilterPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void JuceAllpassFilterPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void JuceAllpassFilterPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    //CUSTOM
    filter.setSamplingRate(static_cast<float>(sampleRate)); //AllpassFilter setup
}

void JuceAllpassFilterPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JuceAllpassFilterPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

//main audio proc method
// - clears unused channels, retrieve plugin params, set them, do filtering
// - params are set at audio rate, not control rate
void JuceAllpassFilterPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //DEFAULT ABOVE, CUSTOM BELOW HERE
    const auto cutoffFrequency = cutoffFrequencyParameter->load();
    const auto highpass = *highpassParameter < 0.5f ? false : true; //equivalent to load
    filter.setCutoffFrequency(cutoffFrequency);
    filter.setHighpass(highpass);
    filter.processBlock(buffer, midiMessages); //do filtering
}

//==============================================================================
bool JuceAllpassFilterPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* JuceAllpassFilterPluginAudioProcessor::createEditor()
{
    return new JuceAllpassFilterPluginAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void JuceAllpassFilterPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void JuceAllpassFilterPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JuceAllpassFilterPluginAudioProcessor();
}
