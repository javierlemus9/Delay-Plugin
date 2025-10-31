/*
  ==============================================================================

    LookAndFeel.cpp
    Created: 9 Jun 2025 6:39:56pm
    Author:  Javier Lemus

  ==============================================================================
*/

#include "LookAndFeel.h"

const juce::Typeface::Ptr Fonts::typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::LatoMedium_ttf, BinaryData::LatoMedium_ttfSize);

juce::Font Fonts::getFont(float height) {
    
    return juce::FontOptions(typeface)
                    .withMetricsKind(juce::TypefaceMetricsKind::legacy)
                    .withHeight(height);
}

juce::Font MainLookAndFeel::getLabelFont([[maybe_unused]] juce::Label& label) {
    return Fonts::getFont(label.getLocalBounds().getHeight());
}

void MainLookAndFeel::drawGroupComponentOutline(juce::Graphics& g,
                                                int width,
                                                int height,
                                                [[maybe_unused]] const juce::String& text,
                                                const juce::Justification& position,
                                                juce::GroupComponent& group){
    
    auto bounds = juce::Rectangle<int>(width, height).toFloat();
    auto cornerSize = 10.0f;

    g.setColour (group.findColour(groupOutlineId));
    g.fillRoundedRectangle(bounds, cornerSize);
}

void MainLookAndFeel::drawRotarySlider(juce::Graphics& g,
                                             int x,
                                             int y,
                                             int width,
                                             [[maybe_unused]] int height,
                                             float sliderPos,
                                             float rotaryStartAngle,
                                             float rotaryEndAngle,
                                             juce::Slider& slider){
    
    auto bounds = juce::Rectangle<int>(x, y, width, width).toFloat();
    auto knobRect = bounds.reduced(10.0f, 10.0f);
    
    auto path = juce::Path();
    path.addEllipse(knobRect);
    knobDropShadow.drawForPath(g, path);
    
    g.setColour(findColour(knobOutlineId));
    g.fillEllipse(knobRect);
    
    auto innerRect = knobRect.reduced(2.0f, 2.0f);
    auto gradient = juce::ColourGradient::vertical(findColour(knobGradientTopId), 0.0f, findColour(knobGradientBottomId), innerRect.getHeight());
    
    g.setGradientFill(gradient);
    g.fillEllipse(innerRect);
    
    auto center = bounds.getCentre();
    auto radius = bounds.getWidth() / 2;
    auto lineWidth = 3.0f;
    auto arcRadius = radius - lineWidth / 2.0f;
    
    juce::Path backgroundArc;
    backgroundArc.addCentredArc(center.x,
                                center.y,
                                arcRadius,
                                arcRadius,
                                0.0f,
                                rotaryStartAngle,
                                rotaryEndAngle,
                                true);
    
    auto strokeType = juce::PathStrokeType(lineWidth,
                                           juce::PathStrokeType::curved,
                                           juce::PathStrokeType::rounded);
    
    g.setColour(findColour(knobTrackBackgroundId));
    g.strokePath(backgroundArc, strokeType);
    
    auto dialRadius = innerRect.getHeight() / 2.0f - lineWidth;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    
    juce::Point<float> dialStart(center.x + 10.0f * std::sin(toAngle),
                                 center.y - 10.0f * std::cos(toAngle));
    
    juce::Point<float> dialEnd(center.x + dialRadius * std::sin(toAngle),
                               center.y - dialRadius * std::cos(toAngle));
    
    juce::Path dialPath;
    dialPath.startNewSubPath(dialStart);
    dialPath.lineTo(dialEnd);
    g.setColour(findColour(knobDialId));
    g.strokePath(dialPath, strokeType);
    
    if (slider.isEnabled()) {
        float fromAngle = rotaryStartAngle;
        if (slider.getProperties()["drawFromMiddle"]) {
            fromAngle += (rotaryEndAngle - rotaryStartAngle) / 2.0f;
        }
        
        juce::Path valueArc;
        valueArc.addCentredArc(center.x,
                               center.y,
                               arcRadius,
                               arcRadius,
                               0.0f,
                               fromAngle,
                               toAngle,
                               true);
        
        g.setColour(slider.findColour(juce::Slider::rotarySliderFillColourId));
        g.strokePath(valueArc, strokeType);
    }
}

void MainLookAndFeel::fillTextEditorBackground(juce::Graphics& g,
                                                    [[maybe_unused]] int width,
                                                     [[maybe_unused]] int height,
                                                     juce::TextEditor& textEditor){
    
    g.setColour(findColour(knobTextBoxBackgroundId));
    g.fillRoundedRectangle(textEditor.getLocalBounds().reduced(4, 0).toFloat(),
                           4.0f);
    
}

class RotaryKnobLabel : public juce::Label {
  
    public:
    RotaryKnobLabel() : juce::Label() {}
    
    void mouseWheelMove(const juce::MouseEvent&,
                        const juce::MouseWheelDetails&) override {}
    
    std::unique_ptr<juce::AccessibilityHandler> createAccessibilityHandler() override {
        
        return createIgnoredAccessibilityHandler(*this);
    }
    
    juce::TextEditor* createEditorComponent() override {
        
        auto* editor = new juce::TextEditor(getName());
        editor->applyFontToAllText(getLookAndFeel().getLabelFont(*this));
        copyAllExplicitColoursTo(*editor);
        
        editor->setBorder(juce::BorderSize<int>());
        editor->setIndents(2, 1);
        editor->setJustification(juce::Justification::centredTop);
        
        editor->setPopupMenuEnabled(false);
        editor->setInputRestrictions(8);
        
        return editor;
    }
};

juce::Label* MainLookAndFeel::createSliderTextBox(juce::Slider& slider) {
    
    auto label = new RotaryKnobLabel();
    
    label->setJustificationType(juce::Justification::centred);
    label->setKeyboardType(juce::TextInputTarget::decimalKeyboard);
    
    label->setColour(juce::Label::textColourId,
                     slider.findColour(juce::Slider::textBoxTextColourId));
    
    label->setColour(juce::TextEditor::textColourId, findColour(knobValueId));
    label->setColour(juce::TextEditor::highlightedTextColourId,
                     findColour(knobValueId));
    
    label->setColour(juce::TextEditor::highlightColourId,
                     slider.findColour(juce::Slider::rotarySliderFillColourId));
    
    label->setColour(juce::TextEditor::backgroundColourId,
                     findColour(knobTextBoxBackgroundId));
    
    return label;
}

MainLookAndFeel::MainLookAndFeel() {
    
    setTheme(getCurrentTheme());
}

void MainLookAndFeel::setTheme(const Theme &theme) {
    
    // main window
    setColour(backgroundGradientTopId, theme.palette.backgroundGradientTopColor);
    setColour(backgroundGradientBottomId, theme.palette.backgroundGradientBottomColor);
    setColour(headerId,  theme.palette.headerColor);
    
    // group component
    setColour(groupLabelId, theme.palette.groupLabelColor);
    setColour(groupOutlineId, theme.palette.groupBackgroundColor);
    
    // settings
    setColour(settingsIconId, theme.palette.settingsIconColor);
    
    // knob
    setColour(knobTrackBackgroundId, theme.palette.knobTrackBackgroundColor);
    setColour(knobTrackActiveId, theme.palette.knobTrackActiveColor);
    setColour(knobOutlineId, theme.palette.knobOutlineColor);
    setColour(knobGradientTopId, theme.palette.knobGradientTopColor);
    setColour(knobGradientBottomId, theme.palette.knobGradientBottomColor);
    setColour(knobDialId, theme.palette.knobDialColor);
    setColour(knobDropShadowId, theme.palette.knobDropShadowColor);
    setColour(knobLabelId, theme.palette.knobLabelColor);
    setColour(knobTextBoxBackgroundId, theme.palette.knobTextBoxBackgroundColor);
    setColour(knobValueId, theme.palette.knobValueColor);
    setColour(knobCaretId, theme.palette.knobCaretColor);
    
    // button
    setColour(buttonTextId, theme.palette.buttonTextColor);
    setColour(buttonTextToggledId, theme.palette.buttonTextToggledColor);
    setColour(buttonBackgroundId, theme.palette.buttonBackgroundColor);
    setColour(buttonBackgroundToggledId, theme.palette.buttonBackgroundToggledColor);
    setColour(buttonOutlineId, theme.palette.buttonOutlineColor);
    setColour(buttonGradientTopId, theme.palette.buttonGradientTopColor);
    setColour(buttonGradientBottomId, theme.palette.buttonGradientBottomColor);
    setColour(buttonDropShadowId, theme.palette.buttonDropShadowColor);
    
    // level meter
    setColour(levelMeterBackgroundId, theme.palette.levelMeterBackgroundColor);
    setColour(levelMeterTickLineId, theme.palette.levelMeterTickLineColor);
    setColour(levelMeterTickLabelId, theme.palette.levelMeterTickLabelColor);
    setColour(levelMeterTooLoudId, theme.palette.levelMeterTooLoudColor);
    setColour(levelMeterLevelOKId, theme.palette.levelMeterLevelOKColor);
                                             
    currentTheme = theme;
    
}

void MainLookAndFeel::drawButtonBackground(
                                           juce::Graphics &g,
                                           juce::Button &button,
                                           [[maybe_unused]]const juce::Colour &backgroundColour,
                                           [[maybe_unused]]bool shouldDrawButtonAsHighlighted,
                                           bool shouldDrawButtonAsDown){
    
    auto bounds = button.getLocalBounds().toFloat();
    auto cornerSize = bounds.getHeight() * 0.25f;
    auto buttonRect = bounds.reduced(1.0f, 1.0f).withTrimmedBottom(1.0f);
    
    auto path = juce::Path();
    path.addRoundedRectangle(buttonRect, cornerSize);
    buttonDropShadow.drawForPath(g, path);
    
    auto innerRect = buttonRect.reduced(2.0f, 2.0f);
    auto gradient = juce::ColourGradient(
                                         findColour(buttonGradientTopId),
                                         0.0f,
                                         innerRect.getY(),
                                         findColour(buttonGradientBottomId),
                                         0.0f,
                                         innerRect.getBottom(),
                                         false);
    
    g.setGradientFill(gradient);
    g.fillRect(innerRect);
    
    if (shouldDrawButtonAsDown) {
        buttonRect.translate(0.0f, 1.0f);
    }
    
    g.setColour(findColour(buttonBackgroundId));
    g.fillRoundedRectangle(buttonRect, cornerSize);
    
    g.setColour(findColour(buttonOutlineId));
    g.drawRoundedRectangle(buttonRect, cornerSize, 2.0f);
    

}

void MainLookAndFeel::drawButtonText(
                                       juce::Graphics &g,
                                       juce::TextButton &button,
                                       [[maybe_unused]]bool shouldDrawButtonAsHighlighted,
                                       bool shouldDrawButtonAsDown){
    
    auto bounds = button.getLocalBounds().toFloat();
    auto buttonRect = bounds.reduced(1.0f, 1.0f).withTrimmedBottom(1.0f);
    
    if (shouldDrawButtonAsDown) {
        buttonRect.translate(0.0f, 1.0f);
    }
    
    if (!button.getToggleState()) {
        g.setColour(button.findColour(buttonTextId));
    } else {
        g.setColour(button.findColour(buttonTextToggledId));
    }
    
    g.setFont(Fonts::getFont());
    g.drawText(button.getButtonText(), buttonRect, juce::Justification::centred);
}
