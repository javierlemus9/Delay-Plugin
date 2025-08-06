/*
  ==============================================================================

    DSP.h
    Created: 5 Aug 2025 3:21:09pm
    Author:  Javier Lemus

  ==============================================================================
*/
#pragma once

#include <cmath>

inline void panningEqualPowerLaw(float panning, float& left, float& right){
    
    float x = 0.7853981633974483f * (panning + 1.0f);
    left = std::cos(x);
    right = std::sin(x);
}
