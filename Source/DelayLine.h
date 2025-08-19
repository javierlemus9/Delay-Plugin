/*
  ==============================================================================

    DelayLine.h
    Created: 16 Aug 2025 1:59:54pm
    Author:  Javier Lemus

  ==============================================================================
*/

#pragma once

#include <memory>

class DelayLine{
    
    public:
    
    void setMaximumDelayInSamples(int maxlengthInSamples);
    
    void reset() noexcept;
    
    void write(float input) noexcept;
    
    float read(float delayInSamples) const noexcept;
    
    int getBufferlength() const noexcept{
        
        return bufferLength;
    }
    
    
    private:
    
    std::unique_ptr<float[]> buffer;
    int bufferLength = 0;
    int writeIndex = 0;
    
};
