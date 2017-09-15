/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin editor.
 
 ==============================================================================
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "../JuceLibraryCode/BinaryData.h"


//==============================================================================
/**
 */

//Set Look and Feel for all dials
class SliderLookAndFeel : public LookAndFeel_V4
{
public:

    void drawRotarySlider (Graphics &g, int x, int y, int width, int height, float sliderPos,
                           float rotaryStartAngle, float rotaryEndAngle, Slider &slider) override
    {

        knobImg = ImageCache::getFromMemory(BinaryData::knob_png, BinaryData::knob_pngSize);
        
        float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));
        
        g.drawImageTransformed(knobImg, AffineTransform::identity.rotated(angle, knobImg.getWidth() /2, knobImg.getHeight() /2));
        

    }
    
    
private:
    Image knobImg;
};

struct ParameterSlider : public Slider,
public Timer
{
    ParameterSlider(AudioProcessorParameter& p)
    : Slider (p.getName (256)), param (p)
    {
        //Set Slider GUI features.
        setRange(0.0, 1.0, 0.0);
        updateSliderPos();
    }
    
    void valueChanged() override
    {
        //If mouse is down then grab the current floating value.
        if(isMouseButtonDown())
            param.setValueNotifyingHost((float) Slider::getValue());
        else
            param.setValue((float) Slider::getValue());
    }
    
    void timerCallback() override { updateSliderPos();}
    
    void startedDragging() override {param.beginChangeGesture();}
    void stoppedDragging() override { param.endChangeGesture();}
    
    double getValueFromText(const String& text) override {return param.getValueForText(text);}
    String getTextFromValue (double value) override {return param.getText((float) value, 1024) + " " + param.getLabel();}
    
    void updateSliderPos()
    {
        const float newValue = param.getValue();
        
        if (newValue != (float) Slider::getValue() && ! isMouseButtonDown())
            Slider::setValue(newValue);
    }
    
    AudioProcessorParameter& param;
};


class LofiBoostAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    LofiBoostAudioProcessorEditor (LofiBoostAudioProcessor&);
    ~LofiBoostAudioProcessorEditor();
    
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    
private:
    
    LofiBoostAudioProcessor& processor;
    
    Image Background;
    
    Image knobImg;
    
    //ScopedPointer for creating parameters.
    ScopedPointer<ParameterSlider> inputVolumeSlider, outputVolumeSlider, lowPassFilterFreqSlider, highPassFilterFreqSlider;
    
    SliderLookAndFeel sliderLookAndFeel;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LofiBoostAudioProcessorEditor)
};
