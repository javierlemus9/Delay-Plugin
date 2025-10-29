/*
  ==============================================================================

    RotaryKnob.cpp
    Created: 5 Jun 2025 5:33:20pm
    Author:  Javier Lemus

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RotaryKnob.h"
#include "LookAndFeel.h"

//==============================================================================
RotaryKnob::RotaryKnob(const juce::String& text,
                       juce::AudioProcessorValueTreeState& apvts,
                       const juce::ParameterID& parameterID,
                       bool drawFromMiddle) : attachment(apvts,parameterID.getParamID(), slider){
    
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    addAndMakeVisible(slider);
    
    label.setText(text, juce::NotificationType::dontSendNotification);
    label.setJustificationType(juce::Justification::horizontallyCentred);
    label.setBorderSize(juce::BorderSize<int>{0, 0, 2, 0});
    addAndMakeVisible(label);
    
    float pi = juce::MathConstants<float>::pi;
    slider.setRotaryParameters(1.25f * pi, 2.75f * pi, true);

    slider.getProperties().set("drawFromMiddle", drawFromMiddle);

}

RotaryKnob::~RotaryKnob(){
}

void RotaryKnob::paint(juce::Graphics& g){
    
    //g.setColour(juce::Colours::black);
    //g.drawRect(getLocalBounds());
    
}

void RotaryKnob::resized(){
    
    auto bounds = getLocalBounds();
    
    const int labelHeight = juce::roundToInt(juce::jmax(14.0f, label.getFont().getHeight() + 2.0f));
    auto sliderArea = bounds.removeFromBottom(juce::jmax(0, bounds.getHeight() - labelHeight));
    
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, getWidth(), getHeight() / 6);
    label.setBounds(bounds);
    slider.setBounds(sliderArea);
    

    
}
