/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */




#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
LofiBoostAudioProcessor::LofiBoostAudioProcessor()
: AudioProcessor (BusesProperties()
                  .withInput  ("Input",  AudioChannelSet::stereo(), true)
                  .withOutput ("Output", AudioChannelSet::stereo(), true)),
lowPassFilter(dsp::IIR::Coefficients<float>::makeFirstOrderLowPass(48000.0, 20000.f)),
highPassFilter(dsp::IIR::Coefficients<float>::makeFirstOrderHighPass(48000.0, 20.f))
{
    
    //Create Parameters
    addParameter(inputVolumeParam = new AudioParameterFloat ("INPUT", "Input Volume", 2.0f, 20.f, 2.0f));
    addParameter(outputVolumeParam = new AudioParameterFloat("OUTPUT", "Output Volume", {-1.0f, 25.0f, 0.f, 1.0f}, 0.f, "dB"));

    addParameter(lowPassFilterFreqParam = new AudioParameterFloat("LPFREQ", "Post Lowpass Freq.", {20.f, 20000.f, 0.f, 0.5f}, 20000.f, "Hz"));
    addParameter(highPassFilterFreqParam = new AudioParameterFloat("HPFREQ", "Pre Highpass Freq", {20.f, 20000.f, 0.f, 0.5f}, 20.f, "Hz"));
    
    addParameter(seteroParam = new AudioParameterChoice("STEREO", "Stereo Processing", {"Always mono", "Yes"}, 1));
    addParameter(slopeParam = new AudioParameterChoice("SLOPE", "Slope", {"-6 dB / octave", "-12 / octave"}, 0));
    
}

LofiBoostAudioProcessor::~LofiBoostAudioProcessor()
{
}

//==============================================================================
const String LofiBoostAudioProcessor::getName() const  {return JucePlugin_Name;}

bool LofiBoostAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool LofiBoostAudioProcessor::producesMidi() const                                 {return true;return false;}

double LofiBoostAudioProcessor::getTailLengthSeconds() const                       {return 0.0;}

int LofiBoostAudioProcessor::getNumPrograms()                                      {return 1;}

int LofiBoostAudioProcessor::getCurrentProgram()                                   {return 0;}

void LofiBoostAudioProcessor::setCurrentProgram (int index)                        {}

const String LofiBoostAudioProcessor::getProgramName (int index)                   {return {};}

void LofiBoostAudioProcessor::changeProgramName (int index, const String& newName) {}

//==============================================================================
void LofiBoostAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
    auto channels = static_cast<uint32>(jmin (getMainBusNumInputChannels(), getMainBusNumOutputChannels()));
    
    dsp::ProcessSpec spec { sampleRate, static_cast<uint32>(samplesPerBlock), channels};
    
    updateParameters();
    
    inputVolume.prepare (spec);
    outputVolume.prepare (spec);
    
    lowPassFilter.prepare (spec);
    highPassFilter.prepare(spec);
    
}

void LofiBoostAudioProcessor::reset()
{
    
    lowPassFilter.reset();
    highPassFilter.reset();
    
}

void LofiBoostAudioProcessor::releaseResources() {}

void LofiBoostAudioProcessor::process (dsp::ProcessContextReplacing<float> context) noexcept
{
    
    inputVolume.process (context);
    outputVolume.process (context);
    
    lowPassFilter.process(context);
    highPassFilter.process(context);
    
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LofiBoostAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
#else
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

void LofiBoostAudioProcessor::processBlock (AudioSampleBuffer& inoutBuffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    
    auto numSamples = inoutBuffer.getNumSamples();
    
    for (auto i = jmin(2, totalNumInputChannels); i < totalNumOutputChannels; ++i)
        inoutBuffer.clear(i, 0, numSamples);
    
    updateParameters();
    
    dsp::AudioBlock<float> block (inoutBuffer);
    
    if (seteroParam->getIndex() == 1)
    {
        //Setero Processing mode:
        if (block.getNumChannels() > 2)
            block = block.getSubsetChannelBlock(0, 2);
        process (dsp::ProcessContextReplacing<float>(block));
    }
    
    else{
        
        //Mono Processing mode:
        auto firstChan = block.getSingleChannelBlock(0);
        
        process (dsp::ProcessContextReplacing<float> (firstChan));
        
        for(size_t chan = 1; chan < block.getNumChannels(); ++chan)
            block.getSingleChannelBlock(chan).copy (firstChan);
        
    }
}

//==============================================================================
bool LofiBoostAudioProcessor::hasEditor() const    {return true;}

AudioProcessorEditor* LofiBoostAudioProcessor::createEditor()  {return new LofiBoostAudioProcessorEditor (*this);}

void LofiBoostAudioProcessor::updateParameters()
{
    
    //Processing for input and output.
    auto inputdB = Decibels::decibelsToGain(inputVolumeParam->get());
    auto outputdB = Decibels::decibelsToGain(outputVolumeParam->get());
    
    if (inputVolume.getGainLinear() != inputdB) inputVolume.setGainLinear(inputdB);
    if (outputVolume.getGainLinear() != outputdB) outputVolume.setGainLinear(outputdB);
    
    dsp::IIR::Coefficients<float>::Ptr newHighPassCoeffs, newLowPassCoeffs;
    auto newSlopeType = slopeParam->getIndex();
    
    if (newSlopeType == 0)
    {
        
        *lowPassFilter.state = *dsp::IIR::Coefficients<float>::makeFirstOrderLowPass(getSampleRate(), lowPassFilterFreqParam->get());
        *highPassFilter.state = *dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), highPassFilterFreqParam->get());
        
    }
    else
    {
        
        *lowPassFilter.state = *dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), lowPassFilterFreqParam->get());
        *highPassFilter.state = *dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), highPassFilterFreqParam->get());
        
    }
    
}

//==============================================================================
void LofiBoostAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    
    
    ScopedPointer<XmlElement> xml(new XmlElement("BoostParams"));
    xml->setAttribute("INPUT", (double) *inputVolumeParam);
    xml->setAttribute("LPFREQ", (double) *lowPassFilterFreqParam);
    xml->setAttribute("HPFREQ", (double) *highPassFilterFreqParam);
    copyXmlToBinary(*xml, destData);
    
    
}

void LofiBoostAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary(data, sizeInBytes));
    if(xmlState != nullptr)
    {
        if(xmlState->hasTagName("BoostParams"))
        {
            
            //The 0.f is the defaul value in case the parameter isn't found.
            *inputVolumeParam = (float) xmlState->getDoubleAttribute("INPUT", 0.f);
            *highPassFilterFreqParam = (float) xmlState->getDoubleAttribute("HPFREQ", 20000.f);
            *lowPassFilterFreqParam = (float) xmlState->getDoubleAttribute("LPFREQ", 20.f);
            
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LofiBoostAudioProcessor();
}
