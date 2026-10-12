/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <algorithm>

IXGrainEngineAudioProcessorEditor::~IXGrainEngineAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}
void IXGrainEngineAudioProcessorEditor::pitchToggle(juce::ToggleButton& button, float coefficient)
{
    if(button.getToggleState())
    {
        if (std::find(audioProcessor.pitchList.begin(), audioProcessor.pitchList.end(), coefficient) == audioProcessor.pitchList.end())
            audioProcessor.pitchList.push_back(coefficient);
    }
    else
    {
        audioProcessor.pitchList.erase(std::remove(audioProcessor.pitchList.begin(), audioProcessor.pitchList.end(), coefficient),audioProcessor.pitchList.end());
    }
    audioProcessor.prepareSegments();
}

IXGrainEngineAudioProcessorEditor::IXGrainEngineAudioProcessorEditor (IXGrainEngineAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Enhanced Look and Feel with modern dark theme
    customLookAndFeel.setColour(juce::Slider::thumbColourId,           juce::Colour(0xff00d4ff));  // Bright cyan
    customLookAndFeel.setColour(juce::Slider::trackColourId,           juce::Colour(0xff2a2a2a));  // Dark grey
    customLookAndFeel.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xff00d4ff));  // Bright cyan
    customLookAndFeel.setColour(juce::Slider::backgroundColourId,      juce::Colour(0xff1a1a1a));  // Very dark grey
    customLookAndFeel.setColour(juce::Label::textColourId,             juce::Colour(0xfff0f0f0));  // Light grey
    customLookAndFeel.setColour(juce::ToggleButton::textColourId,      juce::Colour(0xfff0f0f0));  // Light grey
    customLookAndFeel.setColour(juce::ToggleButton::tickColourId,      juce::Colour(0xff00d4ff));  // Bright cyan
    customLookAndFeel.setColour(juce::ToggleButton::tickDisabledColourId, juce::Colour(0xff555555)); // Medium grey
    customLookAndFeel.setColour(juce::ResizableWindow::backgroundColourId, juce::Colour(0xff1e1e1e)); // Dark background
    setLookAndFeel(&customLookAndFeel);

    // Main window setup
    setResizable(true, true);
    setSize(1200, 700);

    // Title Label
    titleLabel.setText("Pitch Harmonizer", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(28.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setColour(juce::Label::textColourId, juce::Colour(0xff00d4ff));
    addAndMakeVisible(titleLabel);

    // Intervals Group Label
    intervalsLabel.setText("Musical Intervals", juce::dontSendNotification);
    intervalsLabel.setFont(juce::Font(18.0f, juce::Font::bold));
    intervalsLabel.setJustificationType(juce::Justification::centredLeft);
    intervalsLabel.setColour(juce::Label::textColourId, juce::Colour(0xffcccccc));
    addAndMakeVisible(intervalsLabel);

    // Initialize pitch buttons with enhanced styling
    // Minor 2nd
    addAndMakeVisible(minor2);
    addAndMakeVisible(minor2.pitchLabel);
    minor2.setColour(juce::ToggleButton::textColourId, juce::Colour(0xfff0f0f0));
    minor2.setColour(juce::ToggleButton::tickColourId, juce::Colour(0xff00d4ff));
    minor2.onClick = [this]() { pitchToggle(minor2, minor2.coefficient); };

    // Major 2nd
    addAndMakeVisible(major2);
    addAndMakeVisible(major2.pitchLabel);
    major2.setColour(juce::ToggleButton::textColourId, juce::Colour(0xfff0f0f0));
    major2.setColour(juce::ToggleButton::tickColourId, juce::Colour(0xff00d4ff));
    major2.onClick = [this]() { pitchToggle(major2, major2.coefficient); };

    // Minor 3rd
    addAndMakeVisible(minor3);
    addAndMakeVisible(minor3.pitchLabel);
    minor3.setColour(juce::ToggleButton::textColourId, juce::Colour(0xfff0f0f0));
    minor3.setColour(juce::ToggleButton::tickColourId, juce::Colour(0xff00d4ff));
    minor3.onClick = [this]() { pitchToggle(minor3, minor3.coefficient); };

    // Major 3rd
    addAndMakeVisible(major3);
    addAndMakeVisible(major3.pitchLabel);
    major3.setColour(juce::ToggleButton::textColourId, juce::Colour(0xfff0f0f0));
    major3.setColour(juce::ToggleButton::tickColourId, juce::Colour(0xff00d4ff));
    major3.onClick = [this]() { pitchToggle(major3, major3.coefficient); };

    // Perfect 4th
    addAndMakeVisible(perfect4);
    addAndMakeVisible(perfect4.pitchLabel);
    perfect4.setColour(juce::ToggleButton::textColourId, juce::Colour(0xfff0f0f0));
    perfect4.setColour(juce::ToggleButton::tickColourId, juce::Colour(0xff00d4ff));
    perfect4.onClick = [this]() { pitchToggle(perfect4, perfect4.coefficient); };

    // Tritone
    addAndMakeVisible(tritone);
    addAndMakeVisible(tritone.pitchLabel);
    tritone.setColour(juce::ToggleButton::textColourId, juce::Colour(0xfff0f0f0));
    tritone.setColour(juce::ToggleButton::tickColourId, juce::Colour(0xff00d4ff));
    tritone.onClick = [this]() { pitchToggle(tritone, tritone.coefficient); };

    // Perfect 5th
    addAndMakeVisible(perfect5);
    addAndMakeVisible(perfect5.pitchLabel);
    perfect5.setColour(juce::ToggleButton::textColourId, juce::Colour(0xfff0f0f0));
    perfect5.setColour(juce::ToggleButton::tickColourId, juce::Colour(0xff00d4ff));
    perfect5.onClick = [this]() { pitchToggle(perfect5, perfect5.coefficient); };

    // Minor 6th
    addAndMakeVisible(minor6);
    addAndMakeVisible(minor6.pitchLabel);
    minor6.setColour(juce::ToggleButton::textColourId, juce::Colour(0xfff0f0f0));
    minor6.setColour(juce::ToggleButton::tickColourId, juce::Colour(0xff00d4ff));
    minor6.onClick = [this]() { pitchToggle(minor6, minor6.coefficient); };

    // Major 6th
    addAndMakeVisible(major6);
    addAndMakeVisible(major6.pitchLabel);
    major6.setColour(juce::ToggleButton::textColourId, juce::Colour(0xfff0f0f0));
    major6.setColour(juce::ToggleButton::tickColourId, juce::Colour(0xff00d4ff));
    major6.onClick = [this]() { pitchToggle(major6, major6.coefficient); };

    // Minor 7th
    addAndMakeVisible(minor7);
    addAndMakeVisible(minor7.pitchLabel);
    minor7.setColour(juce::ToggleButton::textColourId, juce::Colour(0xfff0f0f0));
    minor7.setColour(juce::ToggleButton::tickColourId, juce::Colour(0xff00d4ff));
    minor7.onClick = [this]() { pitchToggle(minor7, minor7.coefficient); };

    // Major 7th
    addAndMakeVisible(major7);
    addAndMakeVisible(major7.pitchLabel);
    major7.setColour(juce::ToggleButton::textColourId, juce::Colour(0xfff0f0f0));
    major7.setColour(juce::ToggleButton::tickColourId, juce::Colour(0xff00d4ff));
    major7.onClick = [this]() { pitchToggle(major7, major7.coefficient); };

    // Segment Size Control Group
    segmentLabel.setText("Processing", juce::dontSendNotification);
    segmentLabel.setFont(juce::Font(18.0f, juce::Font::bold));
    segmentLabel.setJustificationType(juce::Justification::centredLeft);
    segmentLabel.setColour(juce::Label::textColourId, juce::Colour(0xffcccccc));
    addAndMakeVisible(segmentLabel);

    // Enhanced Segment Size Slider
    segmentSizeSlider.setRange(128, 81920, 128);
    segmentSizeSlider.setValue(audioProcessor.segmentSize, juce::dontSendNotification);
    segmentSizeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    segmentSizeSlider.setTextValueSuffix(" samples");
    segmentSizeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    segmentSizeSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xfff0f0f0));
    segmentSizeSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff2a2a2a));
    segmentSizeSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xff555555));
    addAndMakeVisible(segmentSizeSlider);

    segmentSizeLabel.setText("Segment Size", juce::dontSendNotification);
    segmentSizeLabel.setFont(juce::Font(14.0f));
    segmentSizeLabel.setJustificationType(juce::Justification::centred);
    segmentSizeLabel.setColour(juce::Label::textColourId, juce::Colour(0xffcccccc));
    addAndMakeVisible(segmentSizeLabel);

    // Callback to update segment size
    segmentSizeSlider.onValueChange = [this]()
    {
        audioProcessor.segmentSize = (int) segmentSizeSlider.getValue();
        audioProcessor.prepareSegments();
    };

    // Parameter attachments
    segAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, SS_ID, segmentSizeSlider);
    
    min2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, min2_ID, minor2);
    maj2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, maj2_ID, major2);
    min3Attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, min3_ID, minor3);
    maj3Attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, maj3_ID, major3);
    p4Attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, p4_ID, perfect4);
    ttAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, tt_ID, tritone);
    p5Attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, p5_ID, perfect5);
    min6Attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, min6_ID, minor6);
    maj6Attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, maj6_ID, major6);
    min7Attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, min7_ID, minor7);
    maj7Attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, maj7_ID, major7);
}

// Remove the generic helper function since each button needs individual setup
// Instead, we'll set up each button individually in the constructor

void IXGrainEngineAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Gradient background
    juce::ColourGradient gradient(juce::Colour(0xff1a1a1a), 0, 0,
                                  juce::Colour(0xff2a2a2a), 0, getHeight(),
                                  false);
    g.setGradientFill(gradient);
    g.fillAll();

    // Draw decorative elements
    g.setColour(juce::Colour(0xff333333));
    g.drawRect(20, 60, getWidth() - 40, 200, 2);  // Intervals section border
    g.drawRect(20, 300, getWidth() - 40, 150, 2); // Processing section border

    // Draw section backgrounds
    g.setColour(juce::Colour(0x22ffffff));
    g.fillRect(22, 62, getWidth() - 44, 196);     // Intervals section background
    g.fillRect(22, 302, getWidth() - 44, 146);    // Processing section background
}

void IXGrainEngineAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    
    // Title
    titleLabel.setBounds(area.removeFromTop(50).reduced(20, 10));
    
    // Intervals section
    area.removeFromTop(10); // Spacing
    intervalsLabel.setBounds(area.removeFromTop(30).reduced(30, 5));
    
    auto intervalsArea = area.removeFromTop(180).reduced(40, 20);
    
    // Arrange pitch buttons in a grid layout
    const int buttonsPerRow = 6;
    const int buttonWidth = 80;
    const int buttonHeight = 60;
    const int spacing = 10;
    
    int totalWidth = buttonsPerRow * buttonWidth + (buttonsPerRow - 1) * spacing;
    int startX = (intervalsArea.getWidth() - totalWidth) / 2;
    
    // Array of button pointers and their labels for easier iteration
    std::vector<std::pair<juce::Component*, juce::Component*>> buttonPairs = {
        {&minor2, &minor2.pitchLabel}, {&major2, &major2.pitchLabel},
        {&minor3, &minor3.pitchLabel}, {&major3, &major3.pitchLabel},
        {&perfect4, &perfect4.pitchLabel}, {&tritone, &tritone.pitchLabel},
        {&perfect5, &perfect5.pitchLabel}, {&minor6, &minor6.pitchLabel},
        {&major6, &major6.pitchLabel}, {&minor7, &minor7.pitchLabel},
        {&major7, &major7.pitchLabel}
    };
    
    for (int i = 0; i < buttonPairs.size(); ++i)
    {
        int row = i / buttonsPerRow;
        int col = i % buttonsPerRow;
        
        int x = startX + col * (buttonWidth + spacing);
        int y = row * (buttonHeight + spacing);
        
        // Set button bounds
        buttonPairs[i].first->setBounds(intervalsArea.getX() + x, intervalsArea.getY() + y,
                                       buttonWidth, buttonHeight - 20);
        
        // Set label bounds below the button
        buttonPairs[i].second->setBounds(intervalsArea.getX() + x, intervalsArea.getY() + y + buttonHeight - 20,
                                        buttonWidth, 20);
    }
    
    // Processing section
    area.removeFromTop(30); // Spacing
    segmentLabel.setBounds(area.removeFromTop(30).reduced(30, 5));
    
    auto processingArea = area.removeFromTop(120).reduced(40, 20);
    
    // Center the segment size control
    int sliderSize = 100;
    int centerX = processingArea.getCentreX() - sliderSize / 2;
    int centerY = processingArea.getCentreY() - sliderSize / 2;
    
    segmentSizeSlider.setBounds(centerX, centerY - 15, sliderSize, sliderSize);
    segmentSizeLabel.setBounds(centerX - 20, centerY + sliderSize - 10, sliderSize + 40, 20);
}
