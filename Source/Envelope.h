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
    
    void setEnvelopeParams (float attackParam, float decayParam, float sustainParam, float releaseParam, float shapeParam);
    
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
    
    inline float calulateShapeCoeff (float ENVShapeParam)
    {
        float index;
        int indexIntPart;
        float indexDecimelPart;
        float difference;
        index = ENVShapeParam * 51;
        indexIntPart = static_cast<int>(index);
        indexDecimelPart = index - indexIntPart;
        difference = SCoef[indexIntPart + 1] - SCoef[indexIntPart];
        
        return SCoef[indexIntPart] + (difference * indexDecimelPart);
    }
    
private:
    float sampleRate, attack, decay, sustain, release, shape, dScaler, rScaler;
    double envelopeLevel;
    int envlopeTick, releasetick;
    envState envelopeState, statePrev;
    
    float SCoef [52] =
    {
        0.135335, 0.146607, 0.158817, 0.172045, 0.186374,
        0.201897, 0.218712, 0.236928, 0.256661, 0.278037,
        0.301194, 0.326280, 0.353455, 0.382893, 0.414783,
        0.449329, 0.486752, 0.527292, 0.571209, 0.618783,
        0.670320, 0.726149, 0.786628, 0.852144, 0.923116,
        
        1.000000, 1.083287, 1.173511, 1.271249, 1.377128,
        1.491825, 1.616074, 1.750673, 1.896481, 2.054433,
        2.225541, 2.410900, 2.611696, 2.829217, 3.064854,
        3.320117, 3.596640, 3.896193, 4.220696, 4.572225,
        4.953032, 5.365556, 5.812437, 6.296538, 6.820958,
        
        7.389056, 8.004469,
    };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Envelope);
};



#endif  // ENVELOPE_H_INCLUDED
