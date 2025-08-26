/*
  ==============================================================================

    Parameters.cpp
    Created: 28 May 2025 2:43:54pm
    Author:  Javier Lemus

  ==============================================================================
*/

#include "Parameters.h"
#include "DSP.h"

template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState& apvts,
                          const juce::ParameterID& id,
                          T& destination){
    
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination);
    
}

static float millisecondsFromString(const juce::String& text){
    
    float value = text.getFloatValue();
    
    if(!text.endsWithIgnoreCase("ms")){
        if(text.endsWithIgnoreCase("s") || value < Parameters::minDelayTime){
            return value * 1000.0f;
        }
    }
    
    return value;
    
}

static juce::String stringFromMilliseconds(float value, int){
 
    if (value < 10.0f){
        return juce::String(value, 2) + " ms";
    } else if (value < 100.0f){
        return juce::String(value, 1) + " ms";
    } else if (value < 1000.0f){
        return juce::String(int(value)) + " ms";
    } else {
        return juce::String(value * 0.001f, 2) + " s";
    }
}

static  juce::String stringFromDecibels(float value, int){
    
    return juce::String(value, 1) + " dB";
}

static juce::String stringFromPercent(float value, int){
    
    return juce::String(int(value)) + " %";
}

static juce::String stringFromHz(float value, int){
    
    if (value < 1000.0f){
        return juce::String(int(value)) + "Hz";
    } else if (value < 10000.0f){
        return juce::String(value / 1000.0f, 2) + "kHz";
    } else {
        return juce::String(value / 1000.0f, 1) + "kHz";
    }
}

static float hzFromString(const juce::String& str){
    
    float value = str.getFloatValue();
    if ( value < 20.0f) {
        return value * 1000.0f;
    }
    return value;
}

Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts){
    
    castParameter(apvts, gainParamID, gainParam);
    castParameter(apvts, delayTimeParamID, delayTimeParam);
    castParameter(apvts, mixParamID, mixParam);
    castParameter(apvts, feedbackParamID, feedbackParam);
    castParameter(apvts, stereoWidthParamID, stereoWidthParam);
    castParameter(apvts, lowCutParamID, lowCutParam);
    castParameter(apvts, highCutParamID, highCutParam);
    castParameter(apvts, tempoSyncParamID, tempoSyncParam);
    castParameter(apvts, delayNoteParamID, delayNoteParam);
    castParameter(apvts, bypassParamID, bypassParam);
    
}

juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout(){
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                          gainParamID,
                                                          "Output Gain",
                                                          juce::NormalisableRange<float>{-12.0f,12.0f},
                                                          0.0f,
                                                           juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromDecibels)
                                                           ));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                           delayTimeParamID,
                                                           "Delay Time",
                                                           juce::NormalisableRange<float> {minDelayTime,maxDelayTime, 0.001f, 0.25f},
                                                           100.0f,
                                                           juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromMilliseconds).withValueFromStringFunction(millisecondsFromString)
                                                           ));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                           mixParamID,
                                                           "Mix",
                                                           juce::NormalisableRange<float> {0.0f , 100.0f, 1.0f},
                                                           100.0f,
                                                           juce::AudioParameterFloatAttributes()
                                                               .withStringFromValueFunction(stringFromPercent)
                                                           ));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                          feedbackParamID,
                                                          "Feedback",
                                                          juce::NormalisableRange<float> {-100.0f , 100.0f, 1.0f},
                                                          0.0f,
                                                          juce::AudioParameterFloatAttributes()
                                                          .withStringFromValueFunction(stringFromPercent)
                                                          ));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                           stereoWidthParamID,
                                                           "Stereo Width",
                                                           juce::NormalisableRange<float> (-100.f, 100.0f, 1.0f),
                                                           0.0f,
                                                           juce::AudioParameterFloatAttributes()
                                                           .withStringFromValueFunction(stringFromPercent)
                                                           ));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                           lowCutParamID,
                                                           "Low Cut",
                                                           juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.3f),
                                                           20.0f,
                                                           juce::AudioParameterFloatAttributes()
                                                           .withStringFromValueFunction(stringFromHz)
                                                           .withValueFromStringFunction(hzFromString)
                                                           ));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                           highCutParamID,
                                                           "High Cut",
                                                           juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.3f),
                                                           20000.0f,
                                                           juce::AudioParameterFloatAttributes()
                                                           .withStringFromValueFunction(stringFromHz)
                                                           .withValueFromStringFunction(hzFromString)
                                                           ));
    
    layout.add(std::make_unique<juce::AudioParameterBool>(tempoSyncParamID, "Tempo Sync", false));
    
    layout.add(std::make_unique<juce::AudioParameterBool>(bypassParamID, "Bypass", false));
    
    juce::StringArray noteLengths = {
        "1/32",
        "1/16 triplet",
        "1/32 dotted",
        "1/16",
        "1/8 triplet",
        "1/16 dotted",
        "1/8",
        "1/4 triplet",
        "1/8 dotted",
        "1/4",
        "1/2 triplet",
        "1/4 dotted",
        "1/2",
        "1/1 triplet",
        "1/2 dotted",
        "1/1"
    };
    
    layout.add(std::make_unique<juce::AudioParameterChoice>(delayNoteParamID, "DelayNote", noteLengths, 9));
    
    return layout;
}

void Parameters::update() noexcept{

    gainSmoother.setTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    
    targetDelayTime = delayTimeParam->get();
    if (delayTime == 0.0f){
        delayTime = targetDelayTime;
    }
    
    mixSmoother.setTargetValue(mixParam->get() * 0.01f);
    feedbackSmoother.setTargetValue(feedbackParam->get() * 0.01f);
    stereoWidthSmoother.setTargetValue(stereoWidthParam->get() * 0.01f);
    lowCutSmoother.setTargetValue(lowCutParam->get());
    highCutSmoother.setTargetValue(highCutParam->get());
    
    delayNote = delayNoteParam->getIndex();
    tempoSync = tempoSyncParam->get();
    bypassed = bypassParam->get();
    
}

void Parameters::prepareToPlay(double sampleRate) noexcept{
    
    double duration = 0.02;
    gainSmoother.reset(sampleRate, duration);
    mixSmoother.reset(sampleRate, duration);
    feedbackSmoother.reset(sampleRate, duration);
    stereoWidthSmoother.reset(sampleRate, duration);
    lowCutSmoother.reset(sampleRate, duration);
    highCutSmoother.reset(sampleRate, duration);
    onePoleSmoothingCoefficient = 1.0f - std::exp(-1.0f / (1.0f * float(sampleRate)));
    
}

void Parameters::reset() noexcept{
    
    gain = 0.0f;
    delayTime = 0.0f;
    mix = 1.0f;
    feedback = 0.0f;
    panL = 0.0f;
    panR = 1.0f;
    lowCut = 20.0f;
    highCut = 20000.0f;
    
    gainSmoother.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    mixSmoother.setCurrentAndTargetValue(mixParam->get() * 0.01f);
    feedbackSmoother.setCurrentAndTargetValue(feedbackParam->get() * 0.01f);
    stereoWidthSmoother.setCurrentAndTargetValue(stereoWidthParam->get() * 0.01f);
    lowCutSmoother.setCurrentAndTargetValue(lowCutParam->get());
    highCutSmoother.setCurrentAndTargetValue(highCutParam->get());
}

void Parameters::smoothen() noexcept{
    
    gain = gainSmoother.getNextValue();
    delayTime = targetDelayTime;
    mix = mixSmoother.getNextValue();
    feedback = feedbackSmoother.getNextValue();
    panningEqualPowerLaw(stereoWidthSmoother.getNextValue(), panL, panR);
    lowCut = lowCutSmoother.getNextValue();
    highCut = highCutSmoother.getNextValue();
}
