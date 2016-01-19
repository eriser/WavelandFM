/*
  ==============================================================================

    FMSynth.cpp
    Created: 16 Jan 2016 3:50:53pm
    Author:  John Caplan

  ==============================================================================
*/

#include "FMSynth.h"

FMSynthVoice::FMSynthVoice():
noteVelocity(0.0f),
currentMidiNote(0.0f),
currentBend(0.0f),
currentMidiPitch(0.0f)
{
    Op1.SetENVParam(0.01f, 0.2f, 0.8f, 0.6f, 0.85f);
    Op2.SetENVParam(0.0f, 0.3f, 0.7f, 0.4f, 0.85f);
    index21 = 13.0f * std::powf(0.56f, 2.0f);
}

FMSynthVoice::~FMSynthVoice() {}

bool FMSynthVoice::canPlaySound(juce::SynthesiserSound *sound)
{
    return true;
}

void FMSynthVoice::startNote(int midiNoteNumber, float velocity, SynthesiserSound* /**/, int /**/)
{
    level = velocity * 0.15f;
    noteVelocity = std::powf(velocity, 2.0f) ;
    
    currentMidiNote = (float) midiNoteNumber;
    currentMidiPitch = currentMidiNote + currentBend;
    
    Op1.ResetPhase();
    Op2.ResetPhase();
    
    Op1.StartOP (currentMidiPitch, getSampleRate());
    Op2.StartOP (currentMidiPitch, getSampleRate());
    
}

void FMSynthVoice::stopNote(float /**/, bool /**/)
{
    Op1.StopOp();
    Op2.StopOp();
}

void FMSynthVoice::pitchWheelMoved(int newValue)
{
    currentBend = (newValue - 8192.0f) / 8192.0f;
    currentMidiPitch = currentMidiNote + currentBend;
    Op1.setOpPitch (currentMidiPitch);
    Op2.setOpPitch (currentMidiPitch);
    
    Op1.updateAngleDelta();
    Op2.updateAngleDelta();
}

void FMSynthVoice::processBlock(AudioBuffer<float> &outputbuffer, int startSample, int numSamples)
{
    if (Op1.getOpstate() != Envelope::idleState && currentMidiNote != 0)
    {
        while (--numSamples >= 0)
        {

            const float currentSample = (Op1.RenderOP() * level);
            Op1.setOpLinearFm(index21 * Op2.RenderOP() * noteVelocity);
            Op1.updateAngleDelta();
            
            for (int i = outputbuffer.getNumChannels(); --i >= 0;)
            {
                outputbuffer.addSample(i, startSample, currentSample);
            }
            ++startSample;
        }
    }
}