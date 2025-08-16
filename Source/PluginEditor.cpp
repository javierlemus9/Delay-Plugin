/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayAudioProcessorEditor::DelayAudioProcessorEditor (DelayAudioProcessor& p): AudioProcessorEditor (&p), audioProcessor (p){
    
    delayGroup.setText("Delay");
    delayGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    delayGroup.addAndMakeVisible(delayTimeKnob);
    delayGroup.addChildComponent(delayNoteKnob);
    
    tempoSyncButton.setButtonText("Sync");
    tempoSyncButton.setClickingTogglesState(true);
    tempoSyncButton.setBounds(0, 0, 70, 27);
    tempoSyncButton.setLookAndFeel(ButtonLookAndFeel::get());
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
    addAndMakeVisible(outputGroup);
    
    setLookAndFeel(&mainLF);
    
    
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
    
    auto rectangleHeader = getLocalBounds().withHeight(40);
    g.setColour(Colors::header);
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
    
    auto bounds = getLocalBounds();
    int y = 50;
    int height = bounds.getHeight() - 60;
    
    delayGroup.setBounds(10, y, 110, height);
    outputGroup.setBounds(bounds.getWidth()  - 160, y, 150, height);
    feedbackGroup.setBounds(delayGroup.getRight() + 10, y,
                            outputGroup.getX() - delayGroup.getRight() - 20, height);
    
    
    delayTimeKnob.setTopLeftPosition(20, 20);
    tempoSyncButton.setTopLeftPosition(20, delayTimeKnob.getBottom() + 10);
    delayNoteKnob.setTopLeftPosition(delayTimeKnob.getX(), delayTimeKnob.getY());
    
    mixKnob.setTopLeftPosition(20, 20);
    gainKnob.setTopLeftPosition(20, mixKnob.getBottom() + 10);
    
    feedbackKnob.setTopLeftPosition(20, 20);
    stereoWidthKnob.setTopLeftPosition(feedbackKnob.getRight() + 20, 20);
    lowCutKnob.setTopLeftPosition(feedbackKnob.getX(), feedbackKnob.getBottom() + 10);
    highCutKnob.setTopLeftPosition(lowCutKnob.getRight() + 20, lowCutKnob.getY());
    

}

void DelayAudioProcessorEditor::parameterValueChanged(int, float value){
    
    if (juce::MessageManager::getInstance()->isThisTheMessageThread()) {
        updateDelayKnobs(value != 0.0f);
    } else {
        juce::MessageManager::callAsync([this, value]{
            updateDelayKnobs(value != 0.0f);
        });
    }
            
    
    DBG("parameter changed: " << value);
}

void DelayAudioProcessorEditor::updateDelayKnobs(bool tempoSyncActive){
    
    delayTimeKnob.setVisible(!tempoSyncActive);
    delayNoteKnob.setVisible(tempoSyncActive);
}
