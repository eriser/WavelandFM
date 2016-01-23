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
currentMidiPitch(0.0f),
idx21Param (0.56),
Op1Tune(0.0f),
Op2Tune(0.0f)

{
    Op1.SetENVParam(0.0f, 0.3f, 0.6f, 0.6f, 0.6f, 0.85f, getSampleRate());
    Op2.SetENVParam(0.0f, 0.5f, 0.4f, 0.4f, 0.7f, 0.66f, getSampleRate());
    index21 = 13.0f * std::powf( idx21Param, 2.0f);
}

FMSynthVoice::~FMSynthVoice() {}

bool FMSynthVoice::canPlaySound(juce::SynthesiserSound *sound)
{
    return true;
}

void FMSynthVoice::setEnvParams(int whichOp, float att, float dec, float sus, float rel, float dshape, float rshape)
{
    switch (whichOp)
    {
        case 0:
            Op1.SetENVParam(att, dec, sus, rel, dshape, rshape, getSampleRate());
            break;
        case 1:
            Op2.SetENVParam(att, dec, sus, rel, dshape, rshape, getSampleRate());
            break;
        default:
            break;
    }
}

void FMSynthVoice::setTune(int whichOp, float tuneParam)
{
    switch (whichOp)
    {
        case 0:
            Op1Tune = (tuneParam - 0.5f) * 48.0f;
            break;
            
        case 1:
            Op2Tune = (tuneParam - 0.5f) * 48.0f;
            break;
        default:
            break;
    }
}

void FMSynthVoice::startNote(int midiNoteNumber, float velocity, SynthesiserSound* /**/, int /**/)
{
    level = velocity * 0.15f;
    noteVelocity = std::powf(velocity, 2.0f) ;
    
    currentMidiNote = (float) midiNoteNumber;
    currentMidiPitch = currentMidiNote + currentBend;
    
    Op1.ResetPhase();
    Op2.ResetPhase();
    
    Op1.StartOP (currentMidiPitch + Op1Tune, getSampleRate());
    Op2.StartOP (currentMidiPitch + Op2Tune, getSampleRate());
    
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
    Op1.setOpPitch (currentMidiPitch + Op1Tune);
    Op2.setOpPitch (currentMidiPitch + Op2Tune);
    
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