/*
  ==============================================================================

    LevelMeter.h
    Created: 18 Aug 2025 9:35:40pm
    Author:  Javier Lemus

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Measurement.h"

//==============================================================================
/*
*/
class LevelMeter  : public juce::Component, private juce::Timer
{
public:
    
    LevelMeter(Measurement& measurementL, Measurement& measurementR);
    
    ~LevelMeter() override;

    void paint (juce::Graphics&) override;
    
    void resized() override;

private:
    
    Measurement& measurementL;
    Measurement& measurementR;
    
    static constexpr float maxdB = 6.0f;
    static constexpr float mindB = -60.0f;
    static constexpr float stepdB = 6.0f;
    
    float maxPosition = 0.0f;
    float minPosition = 0.0f;
    
    static constexpr float clampdB = -120.0f;
    static constexpr float clampLevel = 0.000001f; // -120 dB
    
    float dbLevelL;
    float dbLevelR;
    
    static constexpr int refreshRate = 60;
    
    float decay = 0.0f;
    float levelL = clampLevel;
    float levelR = clampLevel;
    
    void timerCallback() override;
    
    int positionForLevel(float dbLevel) const noexcept{
        
        return int(std::round(juce::jmap(dbLevel, mindB, maxdB, minPosition, maxPosition)));
    }
    
    void drawlevel(juce::Graphics& g, float level, int x, int width);
    
    void updateLevel(float newLevel, float& smoothedLevel, float& leveldB) const;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeter)
};
