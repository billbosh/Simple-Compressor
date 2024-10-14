/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleCompressorAudioProcessorEditor::SimpleCompressorAudioProcessorEditor (SimpleCompressorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    thresholdAttachment(p.getApvts(), p.thresholdParamID, thresholdSlider),
    ratioAttachment(p.getApvts(), p.ratioParamID, ratioSlider),
    attackAttachment(p.getApvts(), p.attackParamID, attackSlider),
    releaseAttachment(p.getApvts(), p.releaseParamID, releaseSlider),
    outputGainAttchment(p.getApvts(), p.outputGainParamID, outputGainSlider)
{
    setLookAndFeel(new CustomLookAndFeel);
    
    thresholdSlider.setTextValueSuffix("dB");
    ratioSlider.setTextValueSuffix(":1");
    attackSlider.setTextValueSuffix("ms");
    releaseSlider.setTextValueSuffix("ms");
    outputGainSlider.setTextValueSuffix("dB");

    
    addAndMakeVisible(thresholdSlider);
    addAndMakeVisible(ratioSlider);
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(releaseSlider);
    addAndMakeVisible(outputGainSlider);
    addAndMakeVisible(thresholdLabel);
    addAndMakeVisible(ratioLabel);
    addAndMakeVisible(attackLabel);
    addAndMakeVisible(releaseLabel);
    addAndMakeVisible(outputGainLabel);

    thresholdLabel.setJustificationType(Justification::centred);
    ratioLabel.setJustificationType(Justification::centred);
    attackLabel.setJustificationType(Justification::centred);
    releaseLabel.setJustificationType(Justification::centred);
    outputGainLabel.setJustificationType(Justification::centred);

    thresholdLabel.attachToComponent(&thresholdSlider, false);
    ratioLabel.attachToComponent(&ratioSlider, false);
    attackLabel.attachToComponent(&attackSlider, false);
    releaseLabel.attachToComponent(&releaseSlider, false);
    outputGainLabel.attachToComponent(&outputGainSlider, false);

    
    setSize (400, 300);
}

SimpleCompressorAudioProcessorEditor::~SimpleCompressorAudioProcessorEditor()
{
}

//==============================================================================
void SimpleCompressorAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void SimpleCompressorAudioProcessorEditor::resized()
{
    auto bounds = getBounds().reduced(30);
    auto sliderWidth = bounds.getWidth() / 5;
    
    thresholdSlider.setBounds(bounds.removeFromLeft(sliderWidth).reduced(2));
    ratioSlider.setBounds(bounds.removeFromLeft(sliderWidth).reduced(2));
    attackSlider.setBounds(bounds.removeFromLeft(sliderWidth).reduced(2));
    releaseSlider.setBounds(bounds.removeFromLeft(sliderWidth).reduced(2));
    outputGainSlider.setBounds(bounds.removeFromLeft(sliderWidth).reduced(2));
}
