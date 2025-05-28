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

class Parameters {
  
    public:
    Parameters(juce::AudioProcessorValueTreeState& apvts);
    
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    void update() noexcept;
    
    void prepareToPlay(double sampleRate) noexcept;
    
    void reset() noexcept;
    
    void smoothen() noexcept;
    
    float gain = 0.0f;
    
    private:
    
    juce::AudioParameterFloat* gainParam;
    juce::LinearSmoothedValue<float> gainSmoother;

    
};
