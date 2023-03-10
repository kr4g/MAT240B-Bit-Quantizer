/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class BitCrush_Parameter : public AudioProcessorParameter
{
    
public:
    
    float defaultValue{0};
    float currentValue{0};
    String name;
    
    float getValue() const override
    {
        return currentValue;
    }
    
    void setValue (float newValue) override
    {
        currentValue = newValue;
    }

    float getDefaultValue () const override
    {
        return defaultValue;
    }

    String getName (int maximumStringLength) const override
    {
        return name;
    }
    
    String getLabel () const override
    {
        return getName(10);
    }
    
    float getValueForText (const String &text) const override
    {
        return 1;
    }
};

//==============================================================================
/**
*/
class NewProjectAudioProcessor  : public AudioProcessor
{
    
    AudioSampleBuffer noiseBuffer, currentOutputBuffer;
    
    BitCrush_Parameter* mode;       // discrete or continuous quantization modes
    BitCrush_Parameter* autoDrive;  // automatically increase drive as amp approaches 0 (on or off)
    BitCrush_Parameter* drive;      // drive amount

    BitCrush_Parameter* bitRedux;   // bit reduction amount 
    BitCrush_Parameter* rateRedux;  // sample rate reduction amount (aka "downsampling")
    
    BitCrush_Parameter* wet;
    BitCrush_Parameter* dry;
    
    // BitCrush_Parameter* noiseAmount;

public:
    //==============================================================================
    NewProjectAudioProcessor();
    ~NewProjectAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect () const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessor)
};

// #pragma once

// #include <JuceHeader.h>

// // starter code from The Audio Programmer:
// // Juce Tutorial 39- Bitcrusher Plugin Walkthrough with Aaron Leese (Stagecraft Software)
// // https://youtu.be/1PLn8IAKEb4
// class BitCrush_Parameter : public AudioProcessorParameter
// {
// public:
//     float defaultValue{0};
//     float currentValue{0};
//     String name;
    
//     float getValue() const override {
//         return currentValue;
//     }
    
//     void setValue (float newValue) override {
//         currentValue = newValue;
//     }

//     float getDefaultValue () const override {
//         return defaultValue;
//     }

//     String getName (int maximumStringLength) const override {
//         return name;
//     }
    
//     String getLabel () const override {
//         return getName(10);
//     }
    
//     float getValueForText (const String &text) const override {
//         return 1;
//     }
// };
// //==============================================================================
// /**
// */
// class NewProjectAudioProcessor  : public AudioProcessor
// {
    
//     AudioSampleBuffer noiseBuffer, currentOutputBuffer;
    
//     BitCrush_Parameter* noiseAmount;
//     BitCrush_Parameter* rateRedux;
//     BitCrush_Parameter* bitRedux;
    
// public:
//     //==============================================================================
//     NewProjectAudioProcessor();
//     ~NewProjectAudioProcessor();

//     //==============================================================================
//     void prepareToPlay (double sampleRate, int samplesPerBlock) override;
//     void releaseResources() override;

//    #ifndef JucePlugin_PreferredChannelConfigurations
//     bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
//    #endif

//     void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

//     //==============================================================================
//     AudioProcessorEditor* createEditor() override;
//     bool hasEditor() const override;

//     //==============================================================================
//     const String getName() const override;

//     bool acceptsMidi() const override;
//     bool producesMidi() const override;
//     bool isMidiEffect () const override;
//     double getTailLengthSeconds() const override;

//     //==============================================================================
//     int getNumPrograms() override;
//     int getCurrentProgram() override;
//     void setCurrentProgram (int index) override;
//     const String getProgramName (int index) override;
//     void changeProgramName (int index, const String& newName) override;

//     //==============================================================================
//     void getStateInformation (MemoryBlock& destData) override;
//     void setStateInformation (const void* data, int sizeInBytes) override;

// private:
//     //==============================================================================
//     JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessor)
// };

// // //==============================================================================
// // class AudioPluginAudioProcessor  : public juce::AudioProcessor
// // {
// // public:
// //     //==============================================================================
// //     AudioPluginAudioProcessor();
// //     ~AudioPluginAudioProcessor() override;

// //     //==============================================================================
// //     void prepareToPlay (double sampleRate, int samplesPerBlock) override;
// //     void releaseResources() override;

// //     bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

// //     void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
// //     using AudioProcessor::processBlock;

// //     //==============================================================================
// //     juce::AudioProcessorEditor* createEditor() override;
// //     bool hasEditor() const override;

// //     //==============================================================================
// //     const juce::String getName() const override;

// //     bool acceptsMidi() const override;
// //     bool producesMidi() const override;
// //     bool isMidiEffect() const override;
// //     double getTailLengthSeconds() const override;

// //     //==============================================================================
// //     int getNumPrograms() override;
// //     int getCurrentProgram() override;
// //     void setCurrentProgram (int index) override;
// //     const juce::String getProgramName (int index) override;
// //     void changeProgramName (int index, const juce::String& newName) override;

// //     //==============================================================================
// //     void getStateInformation (juce::MemoryBlock& destData) override;
// //     void setStateInformation (const void* data, int sizeInBytes) override;

// //     juce::AudioProcessorValueTreeState apvts;

// // private:
// //     //==============================================================================
// //     JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)
// // };
