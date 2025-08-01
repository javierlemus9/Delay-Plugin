/*
  ==============================================================================

    Parameters.cpp
    Created: 28 May 2025 2:43:54pm
    Author:  Javier Lemus

  ==============================================================================
*/

#include "Parameters.h"

template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState& apvts,
                          const juce::ParameterID& id,
                          T& destination){
    
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination);
    
}

Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts){
    
    castParameter(apvts, gainParamID, gainParam);
    
}

juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout(){
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                          gainParamID,
                                                          "Output Gain",
                                                          juce::NormalisableRange<float>{-12.0f,12.0f},
                                                          0.0f));
    return layout;
}

void Parameters::update() noexcept{

    gainSmoother.setTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    
}

void Parameters::prepareToPlay(double sampleRate) noexcept{
    
    double duration = 0.02;
    gainSmoother.reset(sampleRate, duration);
}

void Parameters::reset() noexcept{
    
    gain = 0.0f;
    
    gainSmoother.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
}

void Parameters::smoothen() noexcept{
    
    gain = gainSmoother.getNextValue();
}
