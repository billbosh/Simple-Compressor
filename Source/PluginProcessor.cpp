/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleCompressorAudioProcessor::SimpleCompressorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    parameters(*this, nullptr, "simplecomp", {
        std::make_unique<AudioParameterFloat>(thresholdParamID, "Threshold", NormalisableRange<float> {-70.f, 6.f, 0.01f}, 0.f),
        std::make_unique<AudioParameterFloat>(ratioParamID, "Ratio", NormalisableRange<float> {1.f, 20.f, 0.01f}, 1.f),
        std::make_unique<AudioParameterFloat>(attackParamID, "Attack", NormalisableRange<float> {0.01f, 1000.f, 0.01f, 0.11925f}, 1.f),
        std::make_unique<AudioParameterFloat>(releaseParamID, "Release", NormalisableRange<float> {1.f, 3000.f, 0.01f, 0.38649f}, 30.f),
        std::make_unique<AudioParameterFloat>(outputGainParamID, "Output Gain", NormalisableRange<float> {-36.f, 36.f, 0.01f}, 0.f)
    })
#endif
{
    parameters.addParameterListener(thresholdParamID, this);
    parameters.addParameterListener(ratioParamID, this);
    parameters.addParameterListener(attackParamID, this);
    parameters.addParameterListener(releaseParamID, this);
    parameters.addParameterListener(outputGainParamID, this);
}

SimpleCompressorAudioProcessor::~SimpleCompressorAudioProcessor()
{
    parameters.removeParameterListener(thresholdParamID, this);
    parameters.removeParameterListener(ratioParamID, this);
    parameters.removeParameterListener(attackParamID, this);
    parameters.removeParameterListener(releaseParamID, this);
    parameters.removeParameterListener(outputGainParamID, this);

}

//==============================================================================
const juce::String SimpleCompressorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleCompressorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SimpleCompressorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SimpleCompressorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SimpleCompressorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleCompressorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleCompressorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleCompressorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SimpleCompressorAudioProcessor::getProgramName (int index)
{
    return {};
}

void SimpleCompressorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SimpleCompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    const dsp::ProcessSpec spec {sampleRate, static_cast<uint32>(samplesPerBlock), static_cast<uint32>(getTotalNumInputChannels())};
    compressorProcessor.prepare(spec);
    makeUpGainProcessor.prepare(spec);
    
    // set compressor parameters
    compressorProcessor.setThreshold(parameters.getRawParameterValue(thresholdParamID)->load());
    compressorProcessor.setRatio(parameters.getRawParameterValue(ratioParamID)->load());
    compressorProcessor.setAttack(parameters.getRawParameterValue(attackParamID)->load());
    compressorProcessor.setRelease(parameters.getRawParameterValue(releaseParamID)->load());

    // set make up gain parameters
    makeUpGainProcessor.setGainDecibels(parameters.getRawParameterValue(outputGainParamID)->load());
    makeUpGainProcessor.setRampDurationSeconds(0.001f);
}

void SimpleCompressorAudioProcessor::releaseResources()
{
//    mainProcessor -> releaseResources();
    compressorProcessor.reset();
    makeUpGainProcessor.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimpleCompressorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void SimpleCompressorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    dsp::AudioBlock<float> audioBlock {buffer};
    dsp::ProcessContextReplacing<float> context {audioBlock};
    compressorProcessor.process(context);
    makeUpGainProcessor.process(context);
}

//==============================================================================
bool SimpleCompressorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SimpleCompressorAudioProcessor::createEditor()
{
    return new SimpleCompressorAudioProcessorEditor (*this);
}

//==============================================================================
void SimpleCompressorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SimpleCompressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void SimpleCompressorAudioProcessor::parameterChanged(const String &parameterID, float newValue)
{
    if (parameterID.equalsIgnoreCase(thresholdParamID))
    {
        compressorProcessor.setThreshold(newValue);
    }
    else if (parameterID.equalsIgnoreCase(ratioParamID))
    {
        compressorProcessor.setRatio(newValue);
    }
    else if (parameterID.equalsIgnoreCase(attackParamID))
    {
        compressorProcessor.setAttack(newValue);
    }
    else if (parameterID.equalsIgnoreCase(releaseParamID))
    {
        compressorProcessor.setRelease(newValue);
    }
    else if (parameterID.equalsIgnoreCase(outputGainParamID))
    {
        makeUpGainProcessor.setGainDecibels(newValue);
    }
}

AudioProcessorValueTreeState &SimpleCompressorAudioProcessor::getApvts() 
{
    return parameters;
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleCompressorAudioProcessor();
}
