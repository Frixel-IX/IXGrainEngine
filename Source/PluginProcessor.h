/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#define SS_ID   "segmentSize"
#define SS_NAME "SegmentSize"
#define min2_ID "minor2"
#define min2_NAME "Minor 2nd On"
#define maj2_ID "major2"
#define maj2_NAME "Major 2nd On"
#define min3_ID "minor3"
#define min3_NAME "Minor 3rd On"
#define maj3_ID "major3"
#define maj3_NAME "Major 3rd On"
#define p4_ID "perfect4"
#define p4_NAME "Perfect 4th On"
#define tt_ID "tritone"
#define tt_NAME "Tritone On"
#define p5_ID "perfect5"
#define p5_NAME "Perfect 5th On"
#define min6_ID "minor6"
#define min6_NAME "Minor 6th On"
#define maj6_ID "major6"
#define maj6_NAME "Major 6th On"
#define min7_ID "minor7"
#define min7_NAME "Minor 7th On"
#define maj7_ID "major7"
#define maj7_NAME "Major 7th On"

#include <vector>
#include <array>
#include <iostream>
/**
*/
class SimpleEQAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    SimpleEQAudioProcessor();
    ~SimpleEQAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void updateFilters();
    
    juce::dsp::Oscillator<float> osc;
    
    
    juce::AudioBuffer<float> internalBuffer;
    int internalBufferLength = 44100*5;
    int writePosition = 0;
    
    int segmentSize = 2048;
    std::vector<int> segmentOrder;
    int readSegmentIndex = 0;
    int readSampleOffset = 0;
    float readSampleOffsetFloat = 0.0f;
    std::vector<float> segmentPitchFactors; 
   // float pitchMin = 0.8f;   // lower bound for random pitch
   // float pitchMax = 1.2f;   // upper bound for random pitch

    void prepareSegments();
    void recordIncomingAduio (juce::AudioBuffer<float>& buffer);
    void playRearrangedAudio (juce::AudioBuffer<float>& buffer);
    void updatePitchList();
    juce::AudioProcessorValueTreeState treeState;
    std::vector<float> pitchList;
    
    int getEditorWidth() const { return 1000; }
    int getEditorHeight() const { return 600; }
    
    
private:
    
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleEQAudioProcessor)
};
