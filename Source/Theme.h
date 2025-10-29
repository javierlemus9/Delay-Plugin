/*
  ==============================================================================

    Theme.h
    Created: 17 Oct 2025 10:31:52am
    Author:  Javier Lemus

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct Palette {

        // main window
        juce::Colour backgroundGradientTopColor;
        juce::Colour backgroundGradientBottomColor;
        juce::Colour headerColor;
            
        // group component
        juce::Colour groupLabelColor;
        juce::Colour groupBackgroundColor;
    
        // knob
        juce::Colour knobTrackBackgroundColor;
        juce::Colour knobTrackActiveColor;
        juce::Colour knobOutlineColor;
        juce::Colour knobGradientTopColor;
        juce::Colour knobGradientBottomColor;
        juce::Colour knobDialColor;
        juce::Colour knobDropShadowColor;
        juce::Colour knobLabelColor;
        juce::Colour knobTextBoxBackgroundColor;
        juce::Colour knobValueColor;
        juce::Colour knobCaretColor;
    
        // button
        juce::Colour buttonTextColor;
        juce::Colour buttonTextToggledColor;
        juce::Colour buttonBackgroundColor;
        juce::Colour buttonBackgroundToggledColor;
        juce::Colour buttonOutlineColor;
        juce::Colour buttonGradientTopColor;
        juce::Colour buttonGradientBottomColor;
        juce::Colour buttonDropShadowColor;
        
        // level meter
        juce::Colour levelMeterBackgroundColor;
        juce::Colour levelMeterTickLineColor;
        juce::Colour levelMeterTickLabelColor;
        juce::Colour levelMeterTooLoudColor;
        juce::Colour levelMeterLevelOKColor;
    
};

struct Theme {
    
    juce::String name;
    Palette palette;
    
    static Theme createLightTheme();
    
    static Theme createDarkTheme();
    
    static Theme createHighConstrastTheme();

};

