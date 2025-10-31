/*
  ==============================================================================

    SettingsMenu.h
    Created: 30 Oct 2025 11:29:54am
    Author:  Javier Lemus

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LookAndFeel.h"

//==============================================================================
/*
*/
class SettingsMenu  : public juce::Component
{
public:
    
    SettingsMenu();
    
    ~SettingsMenu() override;

    void paint (juce::Graphics&) override;
    
    void resized() override;
    
    std::function<void(int)> onThemeChange;
    
    enum MenuItemIDs {
        
        ThemesMenu = 1000,
        DarkTheme = 1001,
        LightTheme = 1002,
        About = 2000
    };

private:
    
    void showSettingsMenu();
    
    void initializeSettingsButton();
    
    void handleSettingsOption(int selectedItemId);
    
    juce::DrawableButton settingsButton {"Settings", juce::DrawableButton::ImageFitted};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingsMenu)
};
