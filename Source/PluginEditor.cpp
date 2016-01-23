/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

class WavelandFmAudioProcessorEditor::ParameterSlider   : public Slider,
private Timer
{
public:
    ParameterSlider (AudioProcessorParameter& p)
    : Slider (p.getName (256)), param (p)
    {
        setRange (0.0, 1.0, 0.0);
        startTimerHz (30);
        updateSliderPos();
        Slider::setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        Slider::setTextBoxStyle(Slider::TextBoxBelow, false, 60, 15);
        Slider::setColour(juce::Slider::rotarySliderFillColourId, Colours::lightcyan);
        Slider::setColour(juce::Slider::rotarySliderOutlineColourId, Colours::cyan.darker(0.2f));
        Slider::setColour(juce::Slider::textBoxBackgroundColourId, Colour::fromFloatRGBA (0.0, 0.0, 0.0, 0.0));
        Slider::setColour(juce::Slider::textBoxOutlineColourId, Colour::fromFloatRGBA (0.0, 0.0, 0.0, 0.0));
        Slider::setColour(juce::Slider::textBoxTextColourId, Colours::lightcyan);
        Slider::setSize(60, 60);
    }
    
    void valueChanged() override
    {
        param.setValue ((float) Slider::getValue());
    }
    
    void timerCallback() override       { updateSliderPos(); }
    
    void startedDragging() override     { param.beginChangeGesture(); }
    void stoppedDragging() override     { param.endChangeGesture();   }
    
    double getValueFromText (const String& text) override   { return param.getValueForText (text); }
    String getTextFromValue (double value) override         { return param.getText ((float) value, 1024); }
    
    void updateSliderPos()
    {
        const float newValue = param.getValue();
        
        if (newValue != (float) Slider::getValue())
            Slider::setValue (newValue);
    }
    
    void changeSliderParam (AudioProcessorParameter& newParam)
    {
        param = newParam;
    }
    
    AudioProcessorParameter& param;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterSlider)
};

//==============================================================================
WavelandFmAudioProcessorEditor::WavelandFmAudioProcessorEditor (WavelandFmAudioProcessor& owner)
    : AudioProcessorEditor (owner)
{
    pickOp.addItem("Op 1", 1);
    pickOp.addItem("Op 2", 2);
    addAndMakeVisible(pickOp);
    
    addAndMakeVisible(attackSlider = new ParameterSlider(*owner.getParameters()[0]));
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    setSize (400, 300);
}

WavelandFmAudioProcessorEditor::~WavelandFmAudioProcessorEditor()
{
}

//==============================================================================
void WavelandFmAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::white);

    g.setColour (Colours::black);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void WavelandFmAudioProcessorEditor::resized()
{
    Rectangle<int> r (getLocalBounds().reduced (8));
    //BackImageObj.setBounds(0, 0, r.getWidth() + 16, r.getHeight() + 16);
    //BackImageObj.resized(r.getWidth() + 16, r.getHeight() + 16);
    
    //midiKeyboard.setBounds (r.removeFromBottom (70));
    int sliderMinDistance = 60;
    int divider = 9;
    
    
    r.removeFromTop (30);
    Rectangle<int> sliderArea;
    sliderArea.setBounds(r.getX() + 8, r.getY() + r.getHeight() / 2.5, r.getWidth() - 16, r.getHeight() / 4);
    Rectangle<int> sliderRow2;
    sliderRow2.setBounds(r.getX() + 8, r.getY() + sliderArea.getBottom(), r.getWidth() - 16, r.getHeight() /5);
    
    int distanceBetween {jmax (sliderArea.getWidth()/divider, sliderMinDistance)};
    int rowDistance {jmax (sliderRow2.getHeight(), sliderMinDistance)};
    
    pickOp.setBounds (sliderArea.removeFromLeft (distanceBetween));
    pickOp.setSize   (distanceBetween, rowDistance);
    
    attackSlider->setBounds (sliderArea.removeFromLeft (distanceBetween));
    attackSlider->setSize (distanceBetween, rowDistance);

}
