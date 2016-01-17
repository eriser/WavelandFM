/*
  ==============================================================================

    FMSynth.h
    Created: 16 Jan 2016 3:50:53pm
    Author:  John Caplan

  ==============================================================================
*/

#ifndef FMSYNTH_H_INCLUDED
#define FMSYNTH_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "FMOP.h"

class FMSynthSound : public SynthesiserSound
{
public:
    
    FMSynthSound() {}
    
    bool appliesToNote   (int /* */) override { return true; }
    bool appliesToChannel (int /* */) override { return true; }
};

class FMSynthVoice : public SynthesiserVoice
{
public:
    FMSynthVoice();
    ~FMSynthVoice();
    
    bool canPlaySound (SynthesiserSound* sound) override;
    
    void startNote (int midiNoteNumber, float velocity, SynthesiserSound* /**/, int /**/) override;
    
    void stopNote (float /**/, bool /**/) override;
    
    void pitchWheelMoved (int newValue) override;
    
    void controllerMoved (int /**/, int /**/) override {};
    
    void renderNextBlock (AudioBuffer<float>& outputbuffer, int startSample, int numSamples) override
    {
        processBlock (outputbuffer, startSample, numSamples);
    }
    
private:
    void processBlock (AudioBuffer<float>& outputbuffer, int startSample, int numSamples);
    
    float level;
    
    float currentMidiNote, currentBend, currentMidiPitch;
    
    float const twopi = 2 * float_Pi;
    
    FMOP Op1, Op2;
};


#endif  // FMSYNTH_H_INCLUDED
