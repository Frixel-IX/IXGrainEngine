/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

// 读取音频并分段
// segmentSize  number of segments   segmentOrder

// 设置音高   min-max  change the playing speed
// 重新组合播放



#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "random"
//==============================================================================
SimpleEQAudioProcessor::SimpleEQAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
        treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
}

SimpleEQAudioProcessor::~SimpleEQAudioProcessor()
{
}

//==============================================================================
const juce::String SimpleEQAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleEQAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SimpleEQAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SimpleEQAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SimpleEQAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleEQAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleEQAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleEQAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SimpleEQAudioProcessor::getProgramName (int index)
{
    return {};
}

void SimpleEQAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SimpleEQAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
    juce::ignoreUnused(sampleRate, samplesPerBlock);
    
    internalBuffer.setSize(2, internalBufferLength);
    internalBuffer.clear();
    writePosition=0;
    
    updatePitchList();
    prepareSegments();
//
//    juce::dsp::ProcessSpec spec;
//    
//    spec.maximumBlockSize = samplesPerBlock;
//    
//    spec.numChannels = 1;
//    
//    spec.sampleRate = sampleRate;
//    
//    leftChain.prepare(spec);
//    rightChain.prepare(spec);
//    
//    updateFilters();
//    
//    leftChannelFifo.prepare(samplesPerBlock);
//    rightChannelFifo.prepare(samplesPerBlock);
//    
//    osc.initialise([](float x) { return std::sin(x); });
//    
//    spec.numChannels = getTotalNumOutputChannels();
//    osc.prepare(spec);
//    osc.setFrequency(440);
}

void SimpleEQAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimpleEQAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (//layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo()
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SimpleEQAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    
    recordIncomingAduio(buffer);
    playRearrangedAudio(buffer);
    
//    updateFilters();
//    
//    juce::dsp::AudioBlock<float> block(buffer);
//    
////    buffer.clear();
////
////    for( int i = 0; i < buffer.getNumSamples(); ++i )
////    {
////        buffer.setSample(0, i, osc.processSample(0));
////    }
////
////    juce::dsp::ProcessContextReplacing<float> stereoContext(block);
////    osc.process(stereoContext);
 
}

void SimpleEQAudioProcessor::updatePitchList()
{
    pitchList.clear();
    
    if (treeState.getRawParameterValue(min2_ID)->load())
        pitchList.push_back(std::pow(2.0f, 1.0f/12.0f));
    if (treeState.getRawParameterValue(maj2_ID)->load())
        pitchList.push_back(std::pow(2.0f, 2.0f/12.0f));
    if (treeState.getRawParameterValue(min3_ID)->load())
        pitchList.push_back(std::pow(2.0f, 3.0f/12.0f));
    if (treeState.getRawParameterValue(maj3_ID)->load())
        pitchList.push_back(std::pow(2.0f, 4.0f/12.0f));
    if (treeState.getRawParameterValue(p4_ID)->load())
        pitchList.push_back(std::pow(2.0f, 5.0f/12.0f));
    if (treeState.getRawParameterValue(tt_ID)->load())
        pitchList.push_back(std::pow(2.0f, 6.0f/12.0f));
    if (treeState.getRawParameterValue(p5_ID)->load())
        pitchList.push_back(std::pow(2.0f, 7.0f/12.0f));
    if (treeState.getRawParameterValue(min6_ID)->load())
        pitchList.push_back(std::pow(2.0f, 8.0f/12.0f));
    if (treeState.getRawParameterValue(maj6_ID)->load())
        pitchList.push_back(std::pow(2.0f, 9.0f/12.0f));
    if (treeState.getRawParameterValue(min7_ID)->load())
        pitchList.push_back(std::pow(2.0f, 10.0f/12.0f));
    if (treeState.getRawParameterValue(maj7_ID)->load())
        pitchList.push_back(std::pow(2.0f, 11.0f/12.0f));
}

void SimpleEQAudioProcessor::prepareSegments()
{
    updatePitchList();
    
    const int totalNumSegments = internalBufferLength / segmentSize;
    segmentOrder.resize(totalNumSegments);

    for (int i = 0; i < totalNumSegments; ++i)
        segmentOrder[i] = i;

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(segmentOrder.begin(), segmentOrder.end(), g);
    
    std::vector<float> pitchPool = pitchList;
        if (std::find(pitchPool.begin(), pitchPool.end(), 1.0f) == pitchPool.end())
            pitchPool.push_back(1.0f);
    

    // Example: random pitch factor between 0.5x and 2.0x for each segment
    segmentPitchFactors.resize(totalNumSegments);
    for (int i = 0; i < totalNumSegments; ++i)
    {
//        float randomPitch = juce::Random::getSystemRandom().nextFloat() * 1.5f + 0.5f;
        float r = juce::Random::getSystemRandom().nextFloat();  // [0,1)
        int index = static_cast<int>(r * pitchPool.size());
        float pitch = pitchPool[index];
        segmentPitchFactors[i] = pitch;
        
    }

    readSegmentIndex = 0;
    readSampleOffsetFloat = 0.0f; // We'll store offset as float now
}

void SimpleEQAudioProcessor::recordIncomingAduio (juce::AudioBuffer<float>& buffer)
{
    const int numSamples = buffer.getNumSamples();
    const int numChannels = juce::jmin(buffer.getNumChannels(), internalBuffer.getNumChannels());

    for (int i = 0; i < numSamples; ++i)
    {
        for (int ch = 0; ch < numChannels; ++ch)
        {
            float* dst = internalBuffer.getWritePointer(ch);
            const float* src = buffer.getReadPointer(ch);
            
            dst[writePosition] = src[i];
        }

        // Advance the ring buffer *after* writing all channels for this sample
        writePosition = (writePosition + 1) % internalBufferLength;
    }
}


void SimpleEQAudioProcessor::playRearrangedAudio(juce::AudioBuffer<float>& buffer)
{
    const int numSamples = buffer.getNumSamples();
    const int numChannels = juce::jmin(buffer.getNumChannels(), internalBuffer.getNumChannels());
    const int totalNumSegments = (int) segmentOrder.size();
    
    for (int sample = 0; sample < numSamples; ++sample)
    {
        // If we've reached the end of the segment list, wrap around
        if (readSegmentIndex >= totalNumSegments)
            readSegmentIndex = 0;
        
        // Identify which segment we are reading from
        // segmentOrder [0, 1, 2,3,4,5]
        // segmentOrder [5, 1, 3, 2, 0, 4]
        // segmentPitchFactors [0.8 ,0.7, 0.9, x, x, x]
        // 1 , 2, 3, 4
        // 1,1,2,2,3,3,4,4
        const int currentSegment = segmentOrder[readSegmentIndex];

        // If each segment has its own random pitch:
        float pitchFactor = segmentPitchFactors[currentSegment];
        
        // Or if you have a single global pitch:
        // float pitchFactor = segmentPitchFactor;

        // The segment starts at this position:
        const int segmentStart = currentSegment * segmentSize;

        // We'll read at float offset from the segment start
        // Combine integer offset with float offset
        const float readPosFloat = segmentStart + readSampleOffsetFloat;

        // Wrap around the ring buffer length
        // (We do an int cast for indexing, but also keep fractional part for interpolation)
        const int baseIndex = (int)std::floor(readPosFloat) % internalBufferLength;
        const float frac = readPosFloat - std::floor(readPosFloat);
        
        // Next index for interpolation
        const int nextIndex = (baseIndex + 1) % internalBufferLength;

        // 1, 2, 3, 4, 5
        // 1, 1.5, 2, 2.5, 3, 3.5 ....
        // For each channel, do linear interpolation
        for (int ch = 0; ch < numChannels; ++ch)
        {
            const float* internalRead = internalBuffer.getReadPointer(ch);
            float sampleA = internalRead[baseIndex];
            float sampleB = internalRead[nextIndex];
            
            float outSample = sampleA + (sampleB - sampleA) * frac;
            buffer.setSample(ch, sample, outSample);
        }

        // Advance our float offset by the pitch factor
        readSampleOffsetFloat += pitchFactor;
        // sample segment  buffer
        
        // If we've read an entire segment, move to the next one
        if (readSampleOffsetFloat >= (float)segmentSize)
        {
            readSampleOffsetFloat = 0.0f;
            readSegmentIndex = (readSegmentIndex + 1) % totalNumSegments;
        }
    }
}




//==============================================================================
bool SimpleEQAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SimpleEQAudioProcessor::createEditor()
{
    auto* editor = new SimpleEQAudioProcessorEditor(*this);
        editor->setSize(getEditorWidth(), getEditorHeight());
        return editor;
}

//==============================================================================
void SimpleEQAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    juce::MemoryOutputStream mos(destData, true);
    treeState.state.writeToStream(mos);
}

void SimpleEQAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid())
    {
        treeState.replaceState(tree);
        prepareSegments();
    }
//
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleEQAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout SimpleEQAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{SS_ID, 1},
        SS_NAME,
        juce::NormalisableRange<float>(128.0f, 81920.0f, 128.0f),
        128.0f
    ));
    
    std::array<std::tuple<const char*, const char*>, 12> intervalParams = {{
            {min2_ID, min2_NAME},
            {maj2_ID, maj2_NAME},
            {min3_ID, min3_NAME},
            {maj3_ID, maj3_NAME},
            {p4_ID,   p4_NAME},
            {tt_ID,   tt_NAME},
            {p5_ID,   p5_NAME},
            {min6_ID, min6_NAME},
            {maj6_ID, maj6_NAME},
            {min7_ID, min7_NAME},
            {maj7_ID, maj7_NAME},
        }};

        for (auto& [id, name] : intervalParams)
        {
            params.push_back(std::make_unique<juce::AudioParameterBool>(
                juce::ParameterID{id, 1},
                name,
                false
            ));
        }


    return { params.begin(), params.end() };
}
