/*
  ==============================================================================

    LookAndFeel.h
    Created: 9 Jun 2025 6:39:56pm
    Author:  Javier Lemus

  ==============================================================================
*/
#include <JuceHeader.h>
#include "Theme.h"

#pragma once

class Fonts {
    
    public:
    static juce::Font getFont(float height = 16.0f);
    
    private:
    static const juce::Typeface::Ptr typeface;
    
    Fonts() = delete;
};

class MainLookAndFeel : public juce::LookAndFeel_V4 {
  
    public:
    
    enum ColourIds {
        
        //main window
        backgroundGradientTopId = 0x2000100,
        backgroundGradientBottomId = 0x2000101,
        headerId = 0x2000102,
        
        // group component
        groupLabelId = 0x2000200,
        groupOutlineId = 0x2000201,
        
        //settings menu
        settingsIconId = 0x2000300,
        
        //knob
        knobTrackBackgroundId = 0x2000400,
        knobTrackActiveId = 0x2000401,
        knobOutlineId = 0x2000402,
        knobGradientTopId = 0x2000403,
        knobGradientBottomId = 0x2000404,
        knobDialId = 0x2000405,
        knobDropShadowId = 0x2000406,
        knobLabelId = 0x2000407,
        knobTextBoxBackgroundId = 0x2000408,
        knobValueId = 0x2000409,
        knobCaretId = 0x2000410,
        
        //button
        buttonTextId = 0x2000500,
        buttonTextToggledId = 0x2000501,
        buttonBackgroundId = 0x2000502,
        buttonBackgroundToggledId = 0x2000503,
        buttonOutlineId = 0x2000504,
        buttonGradientTopId = 0x2000505,
        buttonGradientBottomId = 0x2000506,
        buttonDropShadowId = 0x2000507,
        
        //level meter
        levelMeterBackgroundId = 0x2000600,
        levelMeterTickLineId = 0x2000601,
        levelMeterTickLabelId = 0x2000602,
        levelMeterTooLoudId = 0x2000603,
        levelMeterLevelOKId = 0x2000604
    };
    
    MainLookAndFeel();
    
    Theme getCurrentTheme() const {
        return currentTheme;
    }
    
    void setTheme(const Theme &theme);
    
    juce::Font getLabelFont(juce::Label&) override;
    
    //group methods
    void drawGroupComponentOutline(juce::Graphics& g,
                                   int width,
                                   int height,
                                   const juce::String& text,
                                   const juce::Justification& position,
                                   juce::GroupComponent& group) override;
    
    //knob methods
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos,float rotaryStartAngle,
                          float rotaryEndAngle, juce::Slider& slider) override;
    
    juce::Label* createSliderTextBox(juce::Slider&) override;
    
    void drawTextEditorOutline(juce::Graphics&, int, int, juce::TextEditor&) override {}
    
    void fillTextEditorBackground(juce::Graphics&, int width, int height,
                                  juce::TextEditor&) override;
    
    //button methods
    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
                              const juce::Colour& backgroundColour,
                              bool shouldDrawButtonAsHighlighted,
                              bool shouldDrawButtonAsDown) override;
    
    void drawButtonText (juce::Graphics& g, juce::TextButton& button,
                         bool shouldDrawButtonAsHighlighted,
                         bool shouldDrawButtonAsDown) override;
    
    private:
    
    Theme currentTheme;
    
    juce::DropShadow knobDropShadow {currentTheme.palette.knobDropShadowColor, 6, {0, 3} };
    
    juce::DropShadow buttonDropShadow {currentTheme.palette.buttonDropShadowColor, 6, {0, 3} };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainLookAndFeel)
};
