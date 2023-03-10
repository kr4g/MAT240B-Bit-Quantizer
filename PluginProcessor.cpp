/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/
#include "Bitwise.h"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "PluginProcessor.h"
#include "PluginEditor.h"

static Array<float> getWhiteNoise(int numSamples) {
    
    Array<float> noise;
    
    float z0 = 0;
    float z1 = 0;
    bool generate = false;
    
    float mu = 0; // center (0)
    float sigma = 1; // spread -1 <-> 1
    
    float output = 0;
    float u1 = 0;
    float u2 = 0;
    
    Random r = Random::getSystemRandom();
    r.setSeed(Time::getCurrentTime().getMilliseconds());
    const float epsilon = std::numeric_limits<float>::min();
    
    for (int s=0; s < numSamples; s++)
    {

        // GENERATE ::::
        // using box muller method
        // https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
        generate = !generate;
        
        if (!generate)
            output =  z1 * sigma + mu;
        else
        {
            do
            {
                u1 = r.nextFloat();
                u2 = r.nextFloat();
            }
            while ( u1 <= epsilon );
            
            z0 = sqrtf(-2.0 * logf(u1)) * cosf(2*float(double_Pi) * u2);
            z1 = sqrtf(-2.0 * logf(u1)) * sinf(2*float(double_Pi) * u2);
            
            output = z0 * sigma + mu;
        }
        
        // NAN check ...
        jassert(output == output);
        jassert(output > -50 && output < 50);
        
        //
        noise.add(output);   
    }
    return noise;    
}

//==============================================================================
NewProjectAudioProcessor::NewProjectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    noiseAmount = new BitCrush_Parameter();
    noiseAmount->defaultValue = 0;
    noiseAmount->currentValue = 0;
    noiseAmount->name = "Noise";
    addParameter(noiseAmount);
    
    rateRedux = new BitCrush_Parameter();
    rateRedux->defaultValue = 1;
    rateRedux->currentValue = 1;
    rateRedux->name = "Rate";
    addParameter(rateRedux);
    
    bitRedux = new BitCrush_Parameter();
    bitRedux->defaultValue = 32;
    bitRedux->currentValue = 32;
    bitRedux->name = "Bits";
    addParameter(bitRedux);
}

NewProjectAudioProcessor::~NewProjectAudioProcessor()
{
}

//==============================================================================
const String NewProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NewProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NewProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NewProjectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NewProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NewProjectAudioProcessor::setCurrentProgram (int index)
{
}

const String NewProjectAudioProcessor::getProgramName (int index)
{
    return {};
}

void NewProjectAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void NewProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void NewProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NewProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void NewProjectAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    int numSamples = buffer.getNumSamples();
   
    float noiseAmt = -120 + 120*(noiseAmount->getValue()/100); // dB
    float bitDepth = bitRedux->getValue();
    int rateDivide = rateRedux->getValue();
    
    // GET PARAMETERS :::::
    noiseAmt = jlimit<float>(-120, 0, noiseAmt);
    noiseAmt = Decibels::decibelsToGain(noiseAmt);
    
    // SAFETY CHECK :::: since some hosts will change buffer sizes without calling prepToPlay (Bitwig)
    if (noiseBuffer.getNumSamples() != numSamples) {
        noiseBuffer.setSize(2, numSamples, false, true, true); // clears
        currentOutputBuffer.setSize(2, numSamples, false, true, true); // clears
    }
    
    // COPY for processing ...
    currentOutputBuffer.copyFrom(0, 0, buffer.getReadPointer(0), numSamples);
    if (buffer.getNumChannels() > 1) currentOutputBuffer.copyFrom(1, 0, buffer.getReadPointer(1), numSamples);

    // ----------------------------------------------------------
    // NOISE ----------------------------------------------------
    // ----------------------------------------------------------
    // TODO: instead of noise, do something more interesting...
    {
        noiseBuffer.clear();
        
        Array<float> noise = getWhiteNoise(numSamples);  // TAKE OUT OF CALLBACK (e.g. prepToPlay)
        
        // range bound
        noiseAmt = jlimit<float>(0, 1, noiseAmt);
        
        FloatVectorOperations::multiply(noise.getRawDataPointer(), noiseAmt, numSamples);
        
        // ADD the noise ...
        FloatVectorOperations::add(noiseBuffer.getWritePointer(0), noise.getRawDataPointer(), numSamples);
        FloatVectorOperations::add(noiseBuffer.getWritePointer(1), noise.getRawDataPointer(), numSamples); // STEREO
    }
    
    // ADD NOISE to the incoming AUDIO ::::
    currentOutputBuffer.addFrom(0, 0, noiseBuffer.getReadPointer(0), numSamples);
    currentOutputBuffer.addFrom(1, 0, noiseBuffer.getReadPointer(1), numSamples);
    // ----------------------------------------------------------
    // ----------------------------------------------------------
    
    std::srand(std::time(nullptr));
    // int32_t intOperand = std::rand() % 32;
    BitwiseOp op = static_cast<BitwiseOp>(BitwiseOp::XOR);
    // BitwiseOp op = static_cast<BitwiseOp>(rand() % 4);
    // std::cout << "intOperand: " << intOperand << " op: " << bitwiseOpLabels.at(op) << std::endl;

    // RESAMPLE
    for (int chan = 0; chan < currentOutputBuffer.getNumChannels(); chan++) {
        float* data = currentOutputBuffer.getWritePointer(chan);
        
        for (int i = 1; i < numSamples; i++) {
            // BIT DEPTH
            float totalQLevels = powf(2, bitDepth - 1);
            float val = data[i];
            float remainder = fmodf(val, 1/totalQLevels);
            // Quantize ...
            data[i - 1] = val - remainder;
            // // ALTERNATE
            // int j = (int) (data[i] * totalQLevels);
            // data[i] = (float) j / totalQLevels;
            
            if (rateDivide > 1) {
                if (i%rateDivide != 0) data[i] = data[i - i%rateDivide];
                // TODO: Bresenham's line algorithm
            }

            // BITWISE OPERATION
            data[i - 1] = bitwise(data[i - 1], data[i], op);
            // int32_t intSample = floatTo24bit(data[i - 1]);
            // int32_t intOperand = floatTo24bit(data[i]);
            // data[i - 1] = intToFloat24bit(bitwise(intSample, intOperand, op));
        }
    }
    // COPY to the actual output buffer :::
    buffer.copyFrom(0, 0, currentOutputBuffer, 0, 0, numSamples);
    buffer.copyFrom(1, 0, currentOutputBuffer, 1, 0, numSamples);
}

//==============================================================================
bool NewProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* NewProjectAudioProcessor::createEditor()
{
    return new NewProjectAudioProcessorEditor (*this);
}

//==============================================================================
void NewProjectAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void NewProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}



// #include "PluginProcessor.h"
// #include "PluginEditor.h"

// //==============================================================================
// NewProjectAudioProcessor::NewProjectAudioProcessor()
// #ifndef JucePlugin_PreferredChannelConfigurations
//      : AudioProcessor (BusesProperties()
//                      #if ! JucePlugin_IsMidiEffect
//                       #if ! JucePlugin_IsSynth
//                        .withInput  ("Input",  AudioChannelSet::stereo(), true)
//                       #endif
//                        .withOutput ("Output", AudioChannelSet::stereo(), true)
//                      #endif
//                        )
// #endif
// {
//     noiseAmount = new BitCrush_Parameter();
//     noiseAmount->defaultValue = 0;
//     noiseAmount->currentValue = 0;
//     noiseAmount->name = "Noise";
//     addParameter(noiseAmount);
    
//     rateRedux = new BitCrush_Parameter();
//     rateRedux->defaultValue = 1;
//     rateRedux->currentValue = 1;
//     rateRedux->name = "Rate";
//     addParameter(rateRedux);

//     bitRedux = new BitCrush_Parameter();
//     bitRedux->defaultValue = 32;
//     bitRedux->currentValue = 32;
//     bitRedux->name = "Bits";
//     addParameter(bitRedux);
// }

// NewProjectAudioProcessor::~NewProjectAudioProcessor()
// {
// }

// //==============================================================================
// // AudioPluginAudioProcessor::AudioPluginAudioProcessor()
// //      : AudioProcessor (BusesProperties()
// //                      #if ! JucePlugin_IsMidiEffect
// //                       #if ! JucePlugin_IsSynth
// //                        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
// //                       #endif
// //                        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
// //                      #endif
// //                        ),
// //     apvts (*this, nullptr, "PARAMETERS",
// //              { std::make_unique<AudioParameterFloat> (juce::ParameterID("GAIN", ProjectInfo::versionNumber), "gain", NormalisableRange<float> (-100.0f, 1.0f), 0),
// //                std::make_unique<AudioParameterFloat> (juce::ParameterID("BIT", ProjectInfo::versionNumber), "bit", NormalisableRange<float> (0.0f, 1.0f), 0),
// //                std::make_unique<AudioParameterFloat> (juce::ParameterID("RATE", ProjectInfo::versionNumber), "rate", NormalisableRange<float> (0.0f, 1.0f), 0)
// //              })
// // {

// // }

// // AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
// // {
// // }

// //==============================================================================
// const juce::String AudioPluginAudioProcessor::getName() const
// {
//     return JucePlugin_Name;
// }

// bool AudioPluginAudioProcessor::acceptsMidi() const
// {
//    #if JucePlugin_WantsMidiInput
//     return true;
//    #else
//     return false;
//    #endif
// }

// bool AudioPluginAudioProcessor::producesMidi() const
// {
//    #if JucePlugin_ProducesMidiOutput
//     return true;
//    #else
//     return false;
//    #endif
// }

// bool AudioPluginAudioProcessor::isMidiEffect() const
// {
//    #if JucePlugin_IsMidiEffect
//     return true;
//    #else
//     return false;
//    #endif
// }

// double AudioPluginAudioProcessor::getTailLengthSeconds() const
// {
//     return 0.0;
// }

// int AudioPluginAudioProcessor::getNumPrograms()
// {
//     return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
//                 // so this should be at least 1, even if you're not really implementing programs.
// }

// int AudioPluginAudioProcessor::getCurrentProgram()
// {
//     return 0;
// }

// void AudioPluginAudioProcessor::setCurrentProgram (int index)
// {
//     juce::ignoreUnused (index);
// }

// const juce::String AudioPluginAudioProcessor::getProgramName (int index)
// {
//     juce::ignoreUnused (index);
//     return {};
// }

// void AudioPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
// {
//     juce::ignoreUnused (index, newName);
// }

// //==============================================================================
// void AudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
// {
//     // Use this method as the place to do any pre-playback
//     // initialisation that you need..
//     juce::ignoreUnused (sampleRate, samplesPerBlock);
// }

// void AudioPluginAudioProcessor::releaseResources()
// {
//     // When playback stops, you can use this as an opportunity to free up any
//     // spare memory, etc.
// }

// bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
// {
//   #if JucePlugin_IsMidiEffect
//     juce::ignoreUnused (layouts);
//     return true;
//   #else
//     // This is the place where you check if the layout is supported.
//     // In this template code we only support mono or stereo.
//     // Some plugin hosts, such as certain GarageBand versions, will only
//     // load plugins that support stereo bus layouts.
//     if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
//      && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
//         return false;

//     // This checks if the input layout matches the output layout
//    #if ! JucePlugin_IsSynth
//     if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
//         return false;
//    #endif

//     return true;
//   #endif
// }

// void AudioPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
//                                               juce::MidiBuffer& midiMessages)
// {
//     juce::ignoreUnused (midiMessages);

//     ScopedNoDenormals noDenormals;
//     const int totalNumInputChannels  = getTotalNumInputChannels();
//     const int totalNumOutputChannels = getTotalNumOutputChannels();

//     // In case we have more outputs than inputs, this code clears any output
//     // channels that didn't contain input data, (because these aren't
//     // guaranteed to be empty - they may contain garbage).
//     // This is here to avoid people getting screaming feedback
//     // when they first compile a plugin, but obviously you don't need to keep
//     // this code if your algorithm always overwrites all the output channels.
//     for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
//         buffer.clear (i, 0, buffer.getNumSamples());

   
    
//     int numSamples = buffer.getNumSamples();
   
//     float noiseAmt = -120 + 120*(noiseAmount->getValue()/100); // dB
//     float bitDepth = bitRedux->getValue();
//     int rateDivide = rateRedux->getValue();
    
//     // GET PARAMETERS :::::
//     noiseAmt = jlimit<float>(-120, 0, noiseAmt);
//     noiseAmt = Decibels::decibelsToGain(noiseAmt);
// }

// //==============================================================================
// bool AudioPluginAudioProcessor::hasEditor() const
// {
//     return true; // (change this to false if you choose to not supply an editor)
// }

// juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
// {
//     return new AudioPluginAudioProcessorEditor (*this);
// }

// //==============================================================================
// void AudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
// {
//     // You should use this method to store your parameters in the memory block.
//     // You could do that either as raw data, or use the XML or ValueTree classes
//     // as intermediaries to make it easy to save and load complex data.
//     juce::ignoreUnused (destData);
// }

// void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
// {
//     // You should use this method to restore your parameters from this memory block,
//     // whose contents will have been created by the getStateInformation() call.
//     juce::ignoreUnused (data, sizeInBytes);
// }

// //==============================================================================
// // This creates new instances of the plugin..
// juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
// {
//     return new AudioPluginAudioProcessor();
// }
