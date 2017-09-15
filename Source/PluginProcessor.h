/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
*/
class LofiBoostAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    LofiBoostAudioProcessor();
    ~LofiBoostAudioProcessor();

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
    
    void reset() override;
    void updateParameters();
    
    //Add params from processor.cpp
    AudioParameterFloat* inputVolumeParam;
    AudioParameterFloat* outputVolumeParam;
    
    AudioParameterFloat* lowPassFilterFreqParam;
    AudioParameterFloat* highPassFilterFreqParam;
    
    AudioParameterChoice* slopeParam;
    AudioParameterChoice* seteroParam;


private:
    
    void process (dsp::ProcessContextReplacing<float>) noexcept;
    
    dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> lowPassFilter, highPassFilter;
    dsp::Gain<float> inputVolume, outputVolume;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LofiBoostAudioProcessor)
};
