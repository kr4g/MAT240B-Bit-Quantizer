#include "PluginProcessor.h"
#include "PluginEditor.h"

/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    
    getLookAndFeel().setColour(Slider::trackColourId, Colours::red);
    
    getLookAndFeel().setColour(Slider::thumbColourId, Colours::white);
    
    
    // noise.setName("Noise");
    // noise.setRange(0, 100); // %
    // noise.setValue(processor.getParameters()[0]->getValue());
    // noise.setSliderStyle(Slider::LinearVertical);
    // noise.setColour(Slider::textBoxTextColourId, Colours::white);
    // noise.setTextBoxStyle(Slider::TextBoxBelow, false, 200, 20);
    // noise.setValue(0);
    // noise.addListener(this);
    // addAndMakeVisible(&noise);
    
    bitRedux.setName("Bit Depth");
    bitRedux.setRange(32, 1);

    bitRedux.setSkewFactorFromMidPoint(8);  // TODO: dont do it like this

    bitRedux.setValue(processor.getParameters()[0]->getValue());
    bitRedux.setSliderStyle(Slider::LinearVertical);
    bitRedux.setColour(Slider::textBoxTextColourId, Colours::white);
    bitRedux.setTextBoxStyle(Slider::TextBoxBelow, false, 200, 20);
    bitRedux.setValue(32);
    bitRedux.addListener(this);
    addAndMakeVisible(&bitRedux);
    
    rateRedux.setName("Sample Rate");
    rateRedux.setRange(1, 50); // division rate (rate / x)
    rateRedux.setValue(processor.getParameters()[0]->getValue());
    rateRedux.setSliderStyle(Slider::LinearVertical);
    rateRedux.setColour(Slider::textBoxTextColourId, Colours::white);
    rateRedux.setTextBoxStyle(Slider::TextBoxBelow, false, 200, 20);
    rateRedux.setValue(1);
    rateRedux.addListener(this);
    addAndMakeVisible(&rateRedux);
    
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
}

void NewProjectAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    
    // if ( slider == &noise)
    // {
    //     processor.getParameters()[0]->setValue(slider->getValue());
    // }
    if ( slider == &rateRedux)
    {
        processor.getParameters()[1]->setValue(slider->getValue());
        
    }
    else if ( slider == &bitRedux)
    {
        processor.getParameters()[2]->setValue(slider->getValue());
        
    }
}


//==============================================================================
void NewProjectAudioProcessorEditor::paint (Graphics& g) {
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::black.withMultipliedBrightness(.4));
 
    g.setColour(Colours::beige);
    
    int labelW = 100;
    // g.drawText("Noise", noise.getX() + noise.getWidth()/2 - labelW/2, 10, labelW, 20, Justification::centred);
    g.drawText("Bit", bitRedux.getX() + bitRedux.getWidth()/2 - labelW/2, 10, labelW, 20, Justification::centred);
    g.drawText("Rate", rateRedux.getX() + rateRedux.getWidth()/2 - labelW/2, 10, labelW, 20, Justification::centred);
}

void NewProjectAudioProcessorEditor::resized() {
    int margin = 10;
    int w = 60;
    int y = 50;
    
    // noise.setBounds(getWidth()/4 - w/2, y, w, getHeight() - y - margin);
    bitRedux.setBounds(2*getWidth()/4 - w/2, y, w, getHeight() - y - margin);
    rateRedux.setBounds(3*getWidth()/4 - w/2, y, w, getHeight() - y - margin);   
}

// //==============================================================================
// AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
//     : AudioProcessorEditor (&p), processorRef (p), 
//     gainSlider(juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextEntryBoxPosition::TextBoxLeft),
//     bitSlider(juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextEntryBoxPosition::TextBoxLeft),
//     rateSlider(juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextEntryBoxPosition::TextBoxLeft)
// {
//     juce::ignoreUnused (processorRef);
//     // Make sure that before the constructor has finished, you've set the
//     // editor's size to whatever you need it to be.
//     setSize (400, 300);
//     // SETUP GAIN SLIDER
//     gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, "GAIN", gainSlider);
//     bitAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, "BIT", bitSlider);
//     rateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, "RATE", rateSlider);
//     // gainAttachment->sendInitialUpdate();
//     addAndMakeVisible(gainSlider);
//     addAndMakeVisible(bitSlider);
//     addAndMakeVisible(rateSlider);
// }

// AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
// {
// }

// //==============================================================================
// void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
// {
//     // (Our component is opaque, so we must completely fill the background with a solid colour)
//     g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

//     // g.setColour (juce::Colours::white);
//     // g.setFont (15.0f);
//     // g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
// }

// void AudioPluginAudioProcessorEditor::resized()
// {
//     // This is generally where you'll want to lay out the positions of any
//     // subcomponents in your editor..
//     // gainSlider.setBounds(getLocalBounds());
//     gainSlider.setBounds(0, 0, 400, 100);
//     bitSlider.setBounds(0, 50, 400, 100);
//     rateSlider.setBounds(0, 100, 400, 100);
//     // bitSlider.setBounds(getLocalBounds());
//     // rateSlider.setBounds(getLocalBounds());
//     // std::cout << ((gainSlider == nullptr) ? "gain is null" : "gain is not null") << std::endl;
//     // gainSlider->setBounds(getLocalBounds());
//     // rateSlider->setBoundsRelative(0.1, 0.1, 0.8, 0.3);
// }
