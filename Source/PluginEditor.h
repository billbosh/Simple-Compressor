/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"

//==============================================================================
/**
*/
class SimpleCompressorAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SimpleCompressorAudioProcessorEditor (SimpleCompressorAudioProcessor&);
    ~SimpleCompressorAudioProcessorEditor() override;
    
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
    SimpleCompressorAudioProcessor& audioProcessor;
    
    Slider thresholdSlider{Slider::SliderStyle::LinearVertical, Slider::TextBoxBelow};
    Slider ratioSlider{Slider::SliderStyle::LinearVertical, Slider::TextBoxBelow};
    Slider attackSlider{Slider::SliderStyle::LinearVertical, Slider::TextBoxBelow};
    Slider releaseSlider{Slider::SliderStyle::LinearVertical, Slider::TextBoxBelow};
    Slider outputGainSlider{Slider::SliderStyle::LinearVertical, Slider::TextBoxBelow};
    
    SliderAttachment thresholdAttachment, ratioAttachment, attackAttachment, releaseAttachment, outputGainAttchment;
    
    Label thresholdLabel{"thresholdLabel", "Threshold"};
    Label ratioLabel{"ratioLabel", "Ratio"};
    Label attackLabel{"attackLabel", "Attack"};
    Label releaseLabel{"releaseLabel", "Release"};
    Label outputGainLabel{"outputGainLabel", "Output"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleCompressorAudioProcessorEditor)
};
