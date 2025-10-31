/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayAudioProcessorEditor::DelayAudioProcessorEditor (DelayAudioProcessor& p) : AudioProcessorEditor (&p), audioProcessor (p), levelMeter(p.levelL, p.levelR){
    
    delayGroup.setText("Delay");
    delayGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    delayGroup.addAndMakeVisible(delayTimeKnob);
    delayGroup.addChildComponent(delayNoteKnob);
    
    tempoSyncButton.setButtonText("Sync");
    tempoSyncButton.setClickingTogglesState(true);
    delayGroup.addAndMakeVisible(tempoSyncButton);
    
    addAndMakeVisible(delayGroup);
    
    feedbackGroup.setText("Feedback");
    feedbackGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    feedbackGroup.addAndMakeVisible(feedbackKnob);
    feedbackGroup.addAndMakeVisible(stereoWidthKnob);
    feedbackGroup.addAndMakeVisible(lowCutKnob);
    feedbackGroup.addAndMakeVisible(highCutKnob);
    addAndMakeVisible(feedbackGroup);
    
    outputGroup.setText("Output");
    outputGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    outputGroup.addAndMakeVisible(gainKnob);
    outputGroup.addAndMakeVisible(mixKnob);
    outputGroup.addAndMakeVisible(levelMeter);
    addAndMakeVisible(outputGroup);
    

          
    auto bypassIcon = juce::ImageCache::getFromMemory(BinaryData::Bypass_png, BinaryData::Bypass_pngSize);
    
    bypassButton.setClickingTogglesState(true);
    bypassButton.setImages(false,
                           true,
                           true,
                           bypassIcon,
                           1.0f,
                           juce::Colours::white,
                           bypassIcon,
                           1.0f,
                           juce::Colours::white,
                           bypassIcon,
                           1.0f,
                           juce::Colours::grey,
                           0.0f);
    
    addAndMakeVisible(bypassButton);
    
    addAndMakeVisible(settingsMenu);
    
    setLookAndFeel(&mainLF);
    mainLF.setTheme(Theme::createDarkTheme());
    
    settingsMenu.onThemeChange = [this](int themeId){
        
        if (themeId == SettingsMenu::MenuItemIDs::LightTheme) {
            mainLF.setTheme(Theme::createLightTheme());
        } else if (themeId == SettingsMenu::MenuItemIDs::DarkTheme) {
            mainLF.setTheme(Theme::createDarkTheme());
        }
        
        repaint();
    };
        

    
    setResizable(true, true);
    getConstrainer()->setFixedAspectRatio(1.51);
    setResizeLimits(500, 330, 750, 495);
    
    setSize (500, 330);
    
    updateDelayKnobs(audioProcessor.params.tempoSyncParam->get());
    audioProcessor.params.tempoSyncParam->addListener(this);
}

DelayAudioProcessorEditor::~DelayAudioProcessorEditor(){
    
    audioProcessor.params.tempoSyncParam->removeListener(this);
    setLookAndFeel(nullptr);
}

//==============================================================================
void DelayAudioProcessorEditor::paint (juce::Graphics& g){
    
    auto noise = juce::ImageCache::getFromMemory(BinaryData::Noise_png, BinaryData::Noise_pngSize);
    auto fillType = juce::FillType(noise, juce::AffineTransform::scale(0.5f));
    g.setFillType(fillType);
    g.fillRect(getLocalBounds());
    
    auto backgroundGradient = juce::ColourGradient::vertical(findColour(mainLF.backgroundGradientTopId), 0.0f, findColour(mainLF.backgroundGradientBottomId), getHeight() / 2);
    
    g.setGradientFill(backgroundGradient);
    g.fillRect(getLocalBounds());
    
    auto rectangleHeader = getLocalBounds().withHeight(40);
    g.setColour(findColour(mainLF.headerId));
    g.fillRect(rectangleHeader);
    
    auto image = juce::ImageCache::getFromMemory(BinaryData::Logo_png, BinaryData::Logo_pngSize);
    
    int destWidth = image.getWidth() / 2;
    int destHeight = image.getHeight() / 2;
    g.drawImage(image,
                getWidth() / 2 - destWidth / 2,
                0,
                destWidth,
                destHeight,
                0,
                0,
                image.getWidth(),
                image.getHeight());
    
}

void DelayAudioProcessorEditor::resized(){

    auto editorArea = getLocalBounds();
    
    auto headerArea = editorArea.removeFromTop(40);
    bypassButton.setBounds(headerArea.removeFromRight(40).reduced(5));
    settingsMenu.setBounds(headerArea.removeFromRight(100).reduced(5));
    
    auto mainArea = editorArea.reduced(10);
    auto delayGroupArea = mainArea.removeFromLeft(int(mainArea.getWidth() * 0.25f)).reduced(5);
    auto feedbackGroupArea = mainArea.removeFromLeft(int(mainArea.getWidth() * 0.55f)).reduced(5);
    auto outputGroupArea = mainArea.reduced(5);
    
    delayGroup.setBounds(delayGroupArea);
    feedbackGroup.setBounds(feedbackGroupArea);
    outputGroup.setBounds(outputGroupArea);
    
    // delay group
    
    int leftMarginDelayGroup = int(delayGroupArea.getWidth() * 0.20f);
    int topMarginDelayGroup = int(delayGroupArea.getHeight() * 0.07f);
    int knobWidth = int(delayGroupArea.getWidth() * 0.6f);
    int knobHeight = int(delayGroupArea.getHeight() * 0.35f);
    int buttonWidth = int(delayGroupArea.getWidth() * 0.7f);
    int buttonHeight = int(delayGroupArea.getHeight() * 0.1f);
    
    delayTimeKnob.setBounds(leftMarginDelayGroup, topMarginDelayGroup, knobWidth, knobHeight);
    tempoSyncButton.setBounds(delayTimeKnob.getX() - 8, delayTimeKnob.getBottom() + 10, buttonWidth,buttonHeight);
    delayNoteKnob.setBounds(leftMarginDelayGroup, topMarginDelayGroup, knobWidth, knobHeight);
    
    // feedback group
    
    int leftMarginFeedbackGroup = int(feedbackGroupArea.getWidth() * 0.11f);
    int topMarginFeedbackGroup = int(feedbackGroupArea.getHeight() * 0.07f);
    
    feedbackKnob.setBounds(leftMarginFeedbackGroup, topMarginFeedbackGroup, knobWidth, knobHeight);
    stereoWidthKnob.setBounds(feedbackKnob.getRight() + 20, topMarginFeedbackGroup, knobWidth, knobHeight);
    lowCutKnob.setBounds(feedbackKnob.getX(), feedbackKnob.getBottom() + 30, knobWidth, knobHeight);
    highCutKnob.setBounds(stereoWidthKnob.getX(), stereoWidthKnob.getBottom() + 30, knobWidth, knobHeight);
    
    // output group
    
    int leftMarginOutputGroup = int(outputGroupArea.getWidth() * 0.11f);
    int topMarginOutputGroup = int(outputGroupArea.getHeight() * 0.07f);
    
    mixKnob.setBounds(leftMarginOutputGroup, topMarginOutputGroup, knobWidth, knobHeight);
    gainKnob.setBounds(mixKnob.getX(), mixKnob.getBottom() + 30, knobWidth, knobHeight);
    levelMeter.setBounds(mixKnob.getRight() + 20, topMarginOutputGroup, outputGroupArea.getWidth() / 4, int(outputGroupArea.getHeight() * 0.9f));

}

void DelayAudioProcessorEditor::parameterValueChanged(int, float value){
    
    if (juce::MessageManager::getInstance()->isThisTheMessageThread()) {
        updateDelayKnobs(value != 0.0f);
    } else {
        juce::MessageManager::callAsync([this, value]{
            updateDelayKnobs(value != 0.0f);
        });
    }
            
}

void DelayAudioProcessorEditor::updateDelayKnobs(bool tempoSyncActive){
    
    delayTimeKnob.setVisible(!tempoSyncActive);
    delayNoteKnob.setVisible(tempoSyncActive);
}
