/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin editor.
 
 ==============================================================================
 */


/*
 ==============================================================================
                Things to do! 
 Fix knobs in PhotoShop!

 ==============================================================================
 */


#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
LofiBoostAudioProcessorEditor::LofiBoostAudioProcessorEditor (LofiBoostAudioProcessor& p)
: AudioProcessorEditor (&p), processor (p)
{
    //==============================================================================
    
    //Add Background Photo.
    Background = ImageCache::getFromMemory(BinaryData::body_png, BinaryData::body_pngSize);
    
    //Add Knob Photo for Data.
    knobImg = ImageCache::getFromMemory(BinaryData::knob_png, BinaryData::knob_pngSize);
    
    
    //Add Sliders with attached Labels.
    addAndMakeVisible(inputVolumeSlider = new ParameterSlider (*processor.inputVolumeParam));
    addAndMakeVisible(lowPassFilterFreqSlider = new ParameterSlider (*processor.lowPassFilterFreqParam));
    addAndMakeVisible(highPassFilterFreqSlider = new ParameterSlider (*processor.highPassFilterFreqParam));
    
    
    //Input Slider
    inputVolumeSlider->setLookAndFeel(&sliderLookAndFeel);
    inputVolumeSlider->setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    inputVolumeSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 0, 0);
    
    
    //Lowpass Slider
    lowPassFilterFreqSlider->setLookAndFeel(&sliderLookAndFeel);
    lowPassFilterFreqSlider->setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    lowPassFilterFreqSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 0, 0);
    
    
    //Highpass Slider
    highPassFilterFreqSlider->setLookAndFeel(&sliderLookAndFeel);
    highPassFilterFreqSlider->setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    highPassFilterFreqSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 0, 0);
    
    
    setSize (Background.getWidth(), Background.getHeight());
    
}

LofiBoostAudioProcessorEditor::~LofiBoostAudioProcessorEditor()                   {}

//==============================================================================
void LofiBoostAudioProcessorEditor::paint (Graphics& g)
{
    
    //Draw Background Image.
    g.drawImageAt(Background, 0, 0);
    
}

void LofiBoostAudioProcessorEditor::resized()
{
    
    
    //Set local bounds.
    Rectangle<int> body (20, 15, 260, 360);
    
    Rectangle<int> lowPass (180, 25, knobImg.getWidth(), knobImg.getHeight());
    
    Rectangle<int> highPass (30, 25, knobImg.getWidth(), knobImg.getHeight());
    
    Rectangle<int> input(109, 125, knobImg.getWidth(), knobImg.getHeight());
    
    
    
    //Placing Sliders
    //Set bounds for Lowpass Slider.
    lowPassFilterFreqSlider->setBounds(lowPass);
    
    //Set bounds for Highpass Slider
    highPassFilterFreqSlider->setBounds(highPass);
    
    //Set bounds for Input Slider.
    inputVolumeSlider->setBounds(input);
    
    
}
