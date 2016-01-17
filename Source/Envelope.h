/*
 ==============================================================================
 
 Envelope.h
 Created: 20 Dec 2015 3:38:11am
 Author:  John Caplan
 
 ==============================================================================
 */

#ifndef ENVELOPE_H_INCLUDED
#define ENVELOPE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class Envelope
{
public:
    Envelope ();
    ~Envelope();
    
    void setSampleRate(float SRate);
    
    void setEnvelopeParams (float attackParam, float decayParam, float sustainParam, float releaseParam);
    
    enum envState
    {
        idleState,
        attackState,
        decayState,
        sustainState,
        releaseState
    };
    
    envState getEnvelopeState();
    
    void setEnvelopeState( Envelope::envState newState);
    
    float getAttack();
    
    float getDecay();
    
    float getSustain();
    
    float getRelease();
    
    float getenvelopeLevel();
    
    void startEnvelope();
    
    void endEnvelope();
    
    void renderEnvelope ();
    
private:
    float sampleRate, attack, decay, sustain, release;
    double envelopeLevel;
    int envlopeTick, releasetick;
    envState envelopeState;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Envelope);
};



#endif  // ENVELOPE_H_INCLUDED
