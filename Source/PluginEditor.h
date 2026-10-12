/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


////==============================================================================
//struct PowerButton : juce::ToggleButton { };
//
//struct AnalyzerButton : juce::ToggleButton
//{
//    void resized() override
//    {
//        auto bounds = getLocalBounds();
//        auto insetRect = bounds.reduced(4);
//        
//        randomPath.clear();
//        
//        juce::Random r;
//        
//        randomPath.startNewSubPath(insetRect.getX(),
//                                   insetRect.getY() + insetRect.getHeight() * r.nextFloat());
//        
//        for( auto x = insetRect.getX() + 1; x < insetRect.getRight(); x += 2 )
//        {
//            randomPath.lineTo(x,
//                              insetRect.getY() + insetRect.getHeight() * r.nextFloat());
//        }
//    }
//    
//    juce::Path randomPath;
//};
/**
*/
class pitchButton : public juce::ToggleButton
{
public:
    float coefficient = 1;
    juce::Label pitchLabel;
    pitchButton(const juce::String name, float theCoefficient)
    {
        setButtonText("");
        pitchLabel.setText(name, juce::NotificationType::dontSendNotification);
        pitchLabel.setJustificationType(juce::Justification::centred);
        pitchLabel.setColour(juce::Label::textColourId, juce::Colours::white);
        pitchLabel.setSize(20.0, 40.0);
        coefficient = theCoefficient;
        
    }
};




class IXGrainEngineAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    IXGrainEngineAudioProcessorEditor(IXGrainEngineAudioProcessor&);
        ~IXGrainEngineAudioProcessorEditor() override;
    

    //==============================================================================
    void pitchToggle(juce::ToggleButton& button, float coefficient);
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    IXGrainEngineAudioProcessor& audioProcessor;
    //pitch Buttons
    pitchButton minor2 = pitchButton("Minor 2nd", 1.05946);
    pitchButton major2 = pitchButton("Major 2nd", 1.12246);
    pitchButton minor3 = pitchButton("Minor 3rd",  1.18921);
    pitchButton major3 = pitchButton("Major 3rd",  1.25992);
    pitchButton perfect4  = pitchButton("Perfect 4th",1.33484);
    pitchButton tritone = pitchButton("Tritone",    1.41421);
    pitchButton perfect5 = pitchButton("Perfect 5th",1.49831);
    pitchButton minor6 = pitchButton("Minor 6th",  1.58740);
    pitchButton major6 = pitchButton("Major 6th",  1.68179);
    pitchButton minor7 = pitchButton("Minor 7th",  1.78180);
    pitchButton major7 = pitchButton("Major 7th",  1.88775);
    
    //segment size slider
    juce::Slider segmentSizeSlider;
    juce::Label  segmentSizeLabel;

    //=== Pitch Controls ===
    juce::Slider pitchMinSlider, pitchMaxSlider;
    juce::Label  pitchMinLabel, pitchMaxLabel;
    // (Optional) A custom LookAndFeel for a nicer appearance
    juce::LookAndFeel_V4 customLookAndFeel;
    
    
    //attachments to parameters
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> min2Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> maj2Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> min3Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> maj3Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> p4Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> ttAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> p5Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> min6Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> maj6Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> min7Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> maj7Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> segAttachment;
    
    juce::Label titleLabel;
    juce::Label intervalsLabel;
    juce::Label  segmentLabel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IXGrainEngineAudioProcessorEditor)
};
