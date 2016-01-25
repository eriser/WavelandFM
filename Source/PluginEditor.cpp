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
    ParameterSlider (AudioProcessorParameter* p)
    : Slider (p->getName (256)), paramBeingEdited (p)
    {
        setRange (0.0, 1.0, 0.0);
        startTimerHz (30);
        updateSliderPos();
        Slider::setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        Slider::setTextBoxStyle(Slider::TextBoxBelow, false, 60, 15);
        Slider::setColour(juce::Slider::rotarySliderFillColourId, Colours::black);
        Slider::setColour(juce::Slider::rotarySliderOutlineColourId, Colours::black);
        Slider::setColour(juce::Slider::textBoxBackgroundColourId, Colour::fromFloatRGBA (0.0, 0.0, 0.0, 0.0));
        Slider::setColour(juce::Slider::textBoxOutlineColourId, Colour::fromFloatRGBA (0.0, 0.0, 0.0, 0.0));
        Slider::setColour(juce::Slider::textBoxTextColourId, Colours::black);
        Slider::setSize(60, 60);
    }
    
    void valueChanged() override
    {
        paramBeingEdited->setValue ((float) Slider::getValue());
    }
    
    void timerCallback() override       { updateSliderPos(); }
    
    void startedDragging() override     { paramBeingEdited->beginChangeGesture(); }
    void stoppedDragging() override     { paramBeingEdited->endChangeGesture();   }
    
    double getValueFromText (const String& text) override   { return paramBeingEdited->getValueForText (text); }
    String getTextFromValue (double value) override         { return paramBeingEdited->getText ((float) value, 1024); }
    
    void updateSliderPos()
    {
        const float newValue = paramBeingEdited->getValue();
        
        if (newValue != (float) Slider::getValue())
            Slider::setValue (newValue);
    }
    
    void changeSliderParam (AudioProcessorParameter* newParam)
    {
        paramBeingEdited = newParam;
    }
    
    AudioProcessorParameter* paramBeingEdited;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterSlider)
};

//==============================================================================
WavelandFmAudioProcessorEditor::WavelandFmAudioProcessorEditor (WavelandFmAudioProcessor& owner)
    : AudioProcessorEditor (owner),
      OpSelectLabel     (String::empty, "Select Operator"),
      attackLabel       (String::empty, "Attack:"),
      decayLabel        (String::empty, "Decay:"),
      sustainLabel      (String::empty, "Sustain:"),
      releaseLabel      (String::empty, "Release:"),
      dshapeLabel       (String::empty, "Decay Shape:"),
      rshapeLabel       (String::empty, "Release Shape:"),
      tuneLabel         (String::empty, "Tune:"),

      IndexLabel        (String::empty, "Index:"),
      BendAmoundLabel   (String::empty, "Bend Amount:")
{
    pickOp.addItem("Op 1", 1);
    pickOp.addItem("Op 2", 2);
    pickOp.addListener(this);
    addAndMakeVisible(pickOp);
    ComboBox* pickOpPrt = &pickOp;
    pickOp.setSelectedId(1);
    
    addAndMakeVisible(attackSlider =     new ParameterSlider(owner.getOpArray()[0].Parms[0].param));
    addAndMakeVisible(decaySlider =      new ParameterSlider(owner.getOpArray()[0].Parms[1].param));
    addAndMakeVisible(sustainSlider =    new ParameterSlider(owner.getOpArray()[0].Parms[2].param));
    addAndMakeVisible(releaseSlider =    new ParameterSlider(owner.getOpArray()[0].Parms[3].param));
    addAndMakeVisible(dshapeSlider =     new ParameterSlider(owner.getOpArray()[0].Parms[4].param));
    addAndMakeVisible(rshapeSlider =     new ParameterSlider(owner.getOpArray()[0].Parms[5].param));
    addAndMakeVisible(tuneSlider =       new ParameterSlider(owner.getOpArray()[0].Parms[6].param));
    
    addAndMakeVisible(IndexSlider =      new ParameterSlider(owner.getVoiceParams()[0].param));
    addAndMakeVisible(BendAmoundSlider = new ParameterSlider(owner.getVoiceParams()[1].param));
    
    setupLabel(OpSelectLabel, pickOpPrt);
    setupLabel(attackLabel, attackSlider);
    setupLabel(decayLabel, decaySlider);
    setupLabel(sustainLabel, sustainSlider);
    setupLabel(releaseLabel, releaseSlider);
    setupLabel(dshapeLabel, dshapeSlider);
    setupLabel(rshapeLabel, rshapeSlider);
    setupLabel(tuneLabel, tuneSlider);
    
    setupLabel(IndexLabel, IndexSlider);
    setupLabel(BendAmoundLabel, BendAmoundSlider);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    setSize (600, 400);
}

WavelandFmAudioProcessorEditor::~WavelandFmAudioProcessorEditor()
{
}

//==============================================================================
void WavelandFmAudioProcessorEditor::comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &pickOp)
    {
        attackSlider->   changeSliderParam(getProcessor().getOpArray()[pickOp.getSelectedItemIndex()].Parms[0].param);
        decaySlider->    changeSliderParam(getProcessor().getOpArray()[pickOp.getSelectedItemIndex()].Parms[1].param);
        sustainSlider->  changeSliderParam(getProcessor().getOpArray()[pickOp.getSelectedItemIndex()].Parms[2].param);
        releaseSlider->  changeSliderParam(getProcessor().getOpArray()[pickOp.getSelectedItemIndex()].Parms[3].param);
        dshapeSlider->   changeSliderParam(getProcessor().getOpArray()[pickOp.getSelectedItemIndex()].Parms[4].param);
        rshapeSlider->   changeSliderParam(getProcessor().getOpArray()[pickOp.getSelectedItemIndex()].Parms[5].param);
        tuneSlider->     changeSliderParam(getProcessor().getOpArray()[pickOp.getSelectedItemIndex()].Parms[6].param);
     
    }
}

void WavelandFmAudioProcessorEditor::setupLabel(juce::Label& labelToUse, juce::Component *sliderToUse)
{
    addAndMakeVisible(labelToUse);
    labelToUse.attachToComponent(sliderToUse, false);
    labelToUse.setFont(Font ("Calibri", 17.0f, Font::italic));
    labelToUse.setSize(60, 20);
    labelToUse.setColour(Label::textColourId, juce::Colours::black);
    labelToUse.setColour(Label::backgroundColourId, juce::Colour::fromFloatRGBA(0.0, 0.0, 0.0, 0.0));
    labelToUse.setJustificationType(Justification::centred);
    labelToUse.setBorderSize(BorderSize<int> (12));
    //labelToUse.setComponentEffect(&labelBackGroundGlow);
}

void WavelandFmAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::white);

    g.setColour (Colours::black);
    g.setFont (15.0f);

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
    
    pickOp.             setBounds (sliderArea.removeFromLeft (distanceBetween));
    pickOp.             setSize   (distanceBetween, rowDistance / 2.0);
    
    attackSlider->      setBounds (sliderArea.removeFromLeft (distanceBetween));
    attackSlider->      setSize (distanceBetween, rowDistance);

    decaySlider->       setBounds (sliderArea.removeFromLeft (distanceBetween));
    decaySlider->       setSize (distanceBetween, rowDistance);
    
    sustainSlider->     setBounds (sliderArea.removeFromLeft (distanceBetween));
    sustainSlider->     setSize (distanceBetween, rowDistance);
    
    releaseSlider->     setBounds (sliderArea.removeFromLeft (distanceBetween));
    releaseSlider->     setSize (distanceBetween, rowDistance);
    
    dshapeSlider->      setBounds (sliderArea.removeFromLeft (distanceBetween));
    dshapeSlider->      setSize (distanceBetween, rowDistance);
    
    rshapeSlider->      setBounds (sliderArea.removeFromLeft (distanceBetween));
    rshapeSlider->      setSize (distanceBetween, rowDistance);
    
    tuneSlider->        setBounds (sliderArea.removeFromLeft (distanceBetween));
    tuneSlider->        setSize (distanceBetween, rowDistance);
    
    
    IndexSlider->       setBounds (sliderRow2.removeFromLeft (distanceBetween));
    IndexSlider->       setSize (distanceBetween, rowDistance);
    
    BendAmoundSlider->  setBounds (sliderRow2.removeFromLeft (distanceBetween));
    BendAmoundSlider->  setSize (distanceBetween, rowDistance);
}
