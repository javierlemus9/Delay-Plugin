/*
  ==============================================================================

    SettingsMenu.cpp
    Created: 30 Oct 2025 11:29:54am
    Author:  Javier Lemus

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SettingsMenu.h"

//==============================================================================
SettingsMenu::SettingsMenu(){

    initializeSettingsButton();

}

SettingsMenu::~SettingsMenu(){
    
}

void SettingsMenu::initializeSettingsButton(){
    
    
    juce::Path settingsGearImage;
    settingsGearImage.addStar({0, 0}, 12, 5.0f, 10.0f, 0.5f);
    
    auto normal = std::make_unique<juce::DrawablePath>();
    //auto color = getLookAndFeel().findColour(MainLookAndFeel::ColourIds::settingsIconId);
    
    normal->setPath(settingsGearImage);
    //normal->setFill(color);
    
    settingsButton.setImages(normal.get());
    addAndMakeVisible(settingsButton);
    
    settingsButton.onClick = [this]() {
        showSettingsMenu();
    };

}

void SettingsMenu::handleSettingsOption(int selectedItemId){
    
    switch (selectedItemId) {
            
        case LightTheme:
            if (onThemeChange)
                onThemeChange(selectedItemId);
            break;
        case DarkTheme:
            if (onThemeChange)
                onThemeChange(selectedItemId);
            break;
            
        case About:
            
            juce::DialogWindow::LaunchOptions dialog;
            dialog.content.setOwned(new juce::Label("About", "Delay Plugin\nVersion 0.1\nDeveloped by Javier Lemus\n\nContact:javierlemus9@gmail.com"));
            dialog.dialogTitle = "About";
            dialog.componentToCentreAround = &settingsButton;
            dialog.launchAsync();
            break;
    }
    
}

void SettingsMenu::showSettingsMenu(){
    
    juce::PopupMenu mainMenu;
    juce::PopupMenu themesMenu;
    
    themesMenu.addItem(LightTheme, "Light");
    themesMenu.addItem(DarkTheme, "Dark");
    
    mainMenu.addSubMenu("Themes", themesMenu);
    mainMenu.addItem(About, "About");
    
    mainMenu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&settingsButton),
                           [this](int selectedItemId){
        handleSettingsOption(selectedItemId); });
}


void SettingsMenu::paint (juce::Graphics& g){
    

}

void SettingsMenu::resized()
{
 
    auto bounds = getLocalBounds();
    
    settingsButton.setBounds(0, 0, bounds.getWidth(), bounds.getHeight());

}
