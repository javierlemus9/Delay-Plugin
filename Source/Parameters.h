/*
  ==============================================================================

    Parameters.h
    Created: 28 May 2025 2:43:54pm
    Author:  Javier Lemus

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

const juce::ParameterID gainParamID { "gain", 1};
const juce::ParameterID delayTimeParamID { "delayTime", 1};
const juce::ParameterID mixParamID { "mix" , 1};
const juce::ParameterID feedbackParamID { "feedback", 1 };

class Parameters {
  
    public:
    Parameters(juce::AudioProcessorValueTreeState& apvts);
    
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    void update() noexcept;
    
    void prepareToPlay(double sampleRate) noexcept;
    
    void reset() noexcept;
    
    void smoothen() noexcept;
    
    float gain = 0.0f;
    
    float delayTime = 0.0f;
    
    float mix = 1.0f;
    
    float feedback = 0.0f;
    
    static constexpr float minDelayTime = 5.0f;
    static constexpr float maxDelayTime = 5000.0f;
    
    private:
    
    juce::AudioParameterFloat* gainParam;
    juce::AudioParameterFloat* delayTimeParam;
    juce::AudioParameterFloat* mixParam;
    juce::AudioParameterFloat* feedbackParam;
    
    juce::LinearSmoothedValue<float> gainSmoother;
    juce::LinearSmoothedValue<float> mixSmoother;
    juce::LinearSmoothedValue<float> feedbackSmoother;
    
    float targetDelayTime = 0.0f;
    float onePoleSmoothingCoefficient = 0.0f;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Parameters)

    
};
