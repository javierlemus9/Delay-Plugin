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
        
        //knob
        knobTrackBackgroundId = 0x2000300,
        knobTrackActiveId = 0x2000301,
        knobOutlineId = 0x2000302,
        knobGradientTopId = 0x2000303,
        knobGradientBottomId = 0x2000304,
        knobDialId = 0x2000305,
        knobDropShadowId = 0x2000306,
        knobLabelId = 0x2000307,
        knobTextBoxBackgroundId = 0x2000308,
        knobValueId = 0x2000309,
        knobCaretId = 0x2000310,
        
        //button
        buttonTextId = 0x2000400,
        buttonTextToggledId = 0x2000401,
        buttonBackgroundId = 0x2000402,
        buttonBackgroundToggledId = 0x2000403,
        buttonOutlineId = 0x2000404,
        buttonGradientTopId = 0x2000405,
        buttonGradientBottomId = 0x2000406,
        buttonDropShadowId = 0x2000407,
        
        //level meter
        levelMeterBackgroundId = 0x2000500,
        levelMeterTickLineId = 0x2000501,
        levelMeterTickLabelId = 0x2000502,
        levelMeterTooLoudId = 0x2000503,
        levelMeterLevelOKId = 0x2000504
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
