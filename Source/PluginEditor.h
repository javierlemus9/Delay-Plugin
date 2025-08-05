/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters.h"
#include "RotaryKnob.h"
#include "LookAndFeel.h"

//==============================================================================
/**
*/
class DelayAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DelayAudioProcessorEditor (DelayAudioProcessor&);
    ~DelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
  
    DelayAudioProcessor& audioProcessor;
    
    RotaryKnob gainKnob { "Gain" , audioProcessor.apvts, gainParamID, true};
    RotaryKnob mixKnob { "Mix" , audioProcessor.apvts, mixParamID};
    RotaryKnob delayTimeKnob { "Time", audioProcessor.apvts, delayTimeParamID};
    RotaryKnob feedbackKnob { "Feedback", audioProcessor.apvts, feedbackParamID, true};
    
    juce::GroupComponent delayGroup, feedbackGroup, outputGroup;
    
    MainLookAndFeel mainLF;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayAudioProcessorEditor)
};
