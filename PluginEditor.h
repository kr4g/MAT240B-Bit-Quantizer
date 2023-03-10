#pragma once

#include "PluginProcessor.h"
#include <JuceHeader.h>
/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class NewProjectAudioProcessorEditor  : public AudioProcessorEditor, public Slider::Listener//, public TextButton::Listener
{
public:
    NewProjectAudioProcessorEditor (NewProjectAudioProcessor&);
    ~NewProjectAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    Slider bitRedux, rateRedux;//, noise;

    
    // TextButton currentSampleDisplay;
    
private:
    
    void sliderValueChanged (Slider* slider) override;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NewProjectAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
};

// //==============================================================================
// class AudioPluginAudioProcessorEditor  : public juce::AudioProcessorEditor
// {
// public:
//     explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
//     ~AudioPluginAudioProcessorEditor() override;

//     //==============================================================================
//     void paint (juce::Graphics&) override;
//     void resized() override;

// private:
//     juce::Slider gainSlider;
//     std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
//     juce::Slider bitSlider;
//     std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bitAttachment;
//     juce::Slider rateSlider;
//     std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateAttachment;
//     // This reference is provided as a quick way for your editor to
//     // access the processor object that created it.
//     AudioPluginAudioProcessor& processorRef;

//     JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
// };
