/*
  ==============================================================================

    Parameters.h
    Created: 28 May 2025 2:43:54pm
    Author:  Javier Lemus

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

const juce::ParameterID gainParamID {"gain", 1};
const juce::ParameterID delayTimeParamID {"delayTime", 1};
const juce::ParameterID mixParamID {"mix" , 1};
const juce::ParameterID feedbackParamID {"feedback", 1 };
const juce::ParameterID stereoWidthParamID {"stereo width", 1};
const juce::ParameterID lowCutParamID {"lowCut", 1};
const juce::ParameterID highCutParamID {"highCut", 1};
const juce::ParameterID tempoSyncParamID {"tempoSync", 1};
const juce::ParameterID delayNoteParamID {"delayNote", 1};
const juce::ParameterID bypassParamID {"bypass" , 1};

class Parameters {
  
    public:

    float gain = 0.0f;
    
    float delayTime = 0.0f;
    
    float mix = 1.0f;
    
    float feedback = 0.0f;
    
    float panL = 0.0f;
    
    float panR = 1.0f;
    
    float lowCut = 20.0f;
    
    float highCut = 20000.0f;
    
    int delayNote = 0;
    
    bool tempoSync = false;
    
    juce::AudioParameterBool* tempoSyncParam;
    juce::AudioParameterBool* bypassParam;
    
    bool bypassed = false;
    
    static constexpr float minDelayTime = 5.0f;
    static constexpr float maxDelayTime = 5000.0f;
    
    Parameters(juce::AudioProcessorValueTreeState& apvts);
    
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    void update() noexcept;
    
    void prepareToPlay(double sampleRate) noexcept;
    
    void reset() noexcept;
    
    void smoothen() noexcept;
    
    private:
    
    juce::AudioParameterFloat* gainParam;
    juce::AudioParameterFloat* delayTimeParam;
    juce::AudioParameterFloat* mixParam;
    juce::AudioParameterFloat* feedbackParam;
    juce::AudioParameterFloat* stereoWidthParam;
    juce::AudioParameterFloat* lowCutParam;
    juce::AudioParameterFloat* highCutParam;
    juce::AudioParameterChoice* delayNoteParam;
    
    juce::LinearSmoothedValue<float> gainSmoother;
    juce::LinearSmoothedValue<float> mixSmoother;
    juce::LinearSmoothedValue<float> feedbackSmoother;
    juce::LinearSmoothedValue<float> stereoWidthSmoother;
    juce::LinearSmoothedValue<float> lowCutSmoother;
    juce::LinearSmoothedValue<float> highCutSmoother;
    
    float targetDelayTime = 0.0f;
    float onePoleSmoothingCoefficient = 0.0f;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Parameters)

    
};
