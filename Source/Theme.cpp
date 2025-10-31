/*
  ==============================================================================

    Theme.cpp
    Created: 17 Oct 2025 10:31:52am
    Author:  Javier Lemus

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Theme.h"


Theme Theme::createLightTheme(){
    
    Theme theme;
    theme.name = "Light";
    theme.palette.backgroundGradientTopColor = juce::Colour(0xFFD7FF79);
    theme.palette.backgroundGradientBottomColor = juce::Colour(0xFFFFFFFF);
    theme.palette.headerColor = juce::Colour(0x80C6DD90);
    
    theme.palette.groupLabelColor = juce::Colour(160, 155, 150);
    theme.palette.groupBackgroundColor = juce::Colour(0xFFC6DD90);
    
    theme.palette.settingsIconColor = juce::Colour(0xFF000000);
    
    theme.palette.knobTrackBackgroundColor = juce::Colour(205, 200, 195);
    theme.palette.knobTrackActiveColor = juce::Colour(177, 101, 135);
    theme.palette.knobOutlineColor = juce::Colour(0x80C6DD90);
    theme.palette.knobGradientTopColor = juce::Colour(0xFFC6DD90);
    theme.palette.knobGradientBottomColor = juce::Colour(0xFFFFFFFF);
    theme.palette.knobDialColor = juce::Colour(0xFF000000);
    theme.palette.knobDropShadowColor = juce::Colour(195, 190, 185);
    theme.palette.knobLabelColor = juce::Colour(0xFF000000);
    theme.palette.knobTextBoxBackgroundColor = juce::Colour(80, 80, 80);
    theme.palette.knobValueColor = juce::Colour(0xFF000000);
    theme.palette.knobCaretColor = juce::Colour(255, 255, 255);
    
    theme.palette.buttonTextColor = juce::Colour(0xFFFF0000);
    theme.palette.buttonTextToggledColor = juce::Colour(0xFF41CE58);
    theme.palette.buttonBackgroundColor = juce::Colour(0xFFFFFFFF);
    theme.palette.buttonBackgroundToggledColor = juce::Colour(255, 250, 245);
    theme.palette.buttonOutlineColor = juce::Colour(235, 230, 225);
    theme.palette.buttonGradientTopColor = juce::Colour(250, 245, 240);
    theme.palette.buttonGradientBottomColor = juce::Colour(240, 235, 230);
    theme.palette.buttonDropShadowColor = juce::Colour(195, 190, 185);
    
    theme.palette.levelMeterBackgroundColor = juce::Colour(0xFFC6DD90);
    theme.palette.levelMeterTickLineColor = juce::Colour(0xFF000000);
    theme.palette.levelMeterTickLabelColor = juce::Colour(0xFF000000);
    theme.palette.levelMeterTooLoudColor = juce::Colour(226, 74, 81);
    theme.palette.levelMeterLevelOKColor = juce::Colour(65, 206, 88);
    
    return theme;
}

Theme Theme::createDarkTheme(){
    
    Theme theme;
    theme.name = "Dark";
    
    theme.palette.backgroundGradientTopColor = juce::Colour(0xFF324701);
    theme.palette.backgroundGradientBottomColor = juce::Colour(0xFF000000);
    theme.palette.headerColor = juce::Colour(0x8095A375);
    
    theme.palette.groupLabelColor = juce::Colour(0xFFFFFFFF);
    theme.palette.groupBackgroundColor = juce::Colour(0xFF324701);
    
    theme.palette.settingsIconColor = juce::Colour(0xFFFFFFFF);
    
    theme.palette.knobTrackBackgroundColor = juce::Colour(205, 200, 195);
    theme.palette.knobTrackActiveColor = juce::Colour(177, 101, 135);
    theme.palette.knobOutlineColor = juce::Colour(0x8095A375);
    theme.palette.knobGradientTopColor = juce::Colour(0xFF324701);
    theme.palette.knobGradientBottomColor = juce::Colour(0xFF000000);
    theme.palette.knobDialColor = juce::Colour(0xFFFFFFFF);
    theme.palette.knobDropShadowColor = juce::Colour(195, 190, 185);
    theme.palette.knobLabelColor = juce::Colour(0xFFFFFFFF);
    theme.palette.knobTextBoxBackgroundColor = juce::Colour(80, 80, 80);
    theme.palette.knobValueColor = juce::Colour(0xFFFFFFFF);
    theme.palette.knobCaretColor = juce::Colour(255, 255, 255);
    
    theme.palette.buttonTextColor = juce::Colour(0xFFFF0000);
    theme.palette.buttonTextToggledColor = juce::Colour(0xFF41CE58);
    theme.palette.buttonBackgroundColor = juce::Colour(0xFF000000);
    theme.palette.buttonBackgroundToggledColor = juce::Colour(255, 250, 245);
    theme.palette.buttonOutlineColor = juce::Colour(235, 230, 225);
    theme.palette.buttonGradientTopColor = juce::Colour(250, 245, 240);
    theme.palette.buttonGradientBottomColor = juce::Colour(240, 235, 230);
    theme.palette.buttonDropShadowColor = juce::Colour(195, 190, 185);
    
    theme.palette.levelMeterBackgroundColor = juce::Colour(0xFF324701);
    theme.palette.levelMeterTickLineColor = juce::Colour(0xFFFFFFFF);
    theme.palette.levelMeterTickLabelColor = juce::Colour(0xFFFFFFFF);
    theme.palette.levelMeterTooLoudColor = juce::Colour(226, 74, 81);
    theme.palette.levelMeterLevelOKColor = juce::Colour(65, 206, 88);
    
    return theme;
}
