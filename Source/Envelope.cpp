/*
 ==============================================================================
 
 Envelope.cpp
 Created: 20 Dec 2015 3:38:11am
 Author:  John Caplan
 
 ==============================================================================
 */

#include "Envelope.h"

Envelope::Envelope ()
: attack (0.0f),
  decay (0.0f),
  sustain (1.0f),
  release (0.1f),
  dShape (0.5f),
  rShape (0.5f),

  attackSlope(0.1f),
  decaySlope(0.1f),
  releaseSlope(0.1f),
  decayShapeCoef(0.5f),
  releaseShapeCoef(0.5f),

  dScaler(0.0f),
  rScaler (0.0f),
  rScaler2 (0.0f),

  envelopeLevel(0.0),
  envelopeOutLevel(0.0f),
  envelopeState (idleState),
  statePrev (idleState)
{
}

Envelope::~Envelope()
{
}

void Envelope::setSampleRate(float SRate)
{
    sampleRate = SRate;
}

void Envelope::setEnvelopeParams (float attackParam, float decayParam, float sustainParam, float releaseParam,
                                  float dShapeParam, float rShapeParam)
{
    attack = attackParam;
    decay = decayParam;
    sustain = sustainParam;
    release = releaseParam;
    dShape = dShapeParam;
    rShape = rShapeParam;
    
    attackSlope = 1.0f / ((attack * sampleRate * 2.0f) + 1.0f);
    decaySlope = (1.0f - sustain) / ((decay * sampleRate * 2.0f) + 1.0f);
    releaseSlope = sustain / ((release * sampleRate * 2.0) + 1.0f);
    
    decayShapeCoef = calulateShapeCoeff (dShape);
    releaseShapeCoef = calulateShapeCoeff (rShape);
}

Envelope::envState Envelope::getEnvelopeState()
{
    return envelopeState;
}

void Envelope::setEnvelopeState( Envelope::envState newState)
{
    envelopeState = newState;
}

float Envelope::getAttack()
{
    return attack;
}

float Envelope::getDecay()
{
    return decay;
}

float Envelope::getSustain()
{
    return sustain;
}

float Envelope::getRelease()
{
    return release;
}

float Envelope::getenvelopeLevel()
{
    if (envelopeState == idleState)
    {
        return 0.0f;
    }
    return envelopeOutLevel;
}

void Envelope::startEnvelope()
{
    envelopeState = attackState;
}

void Envelope::endEnvelope()
{
    envelopeState = releaseState;
}

void Envelope::renderEnvelope ()
{
    switch (envelopeState)
    {
            
        case idleState:

            envelopeLevel = 0.0f;
            envelopeOutLevel = 0.0f;
            
            statePrev = idleState;
            break;
            
        case attackState:
            
            if (statePrev != attackState)
            {
                envelopeLevel = 0.0f;
                envelopeOutLevel = 0.0f;
            }
            
            envelopeLevel += attackSlope;
            envelopeOutLevel = envelopeLevel;
            
            if (envelopeLevel >= 1.0f)
            {
                envelopeState = decayState;
                envelopeLevel = 1.0f;
                envelopeOutLevel = 1.0f;
            }
            
            statePrev = attackState;
            break;
            
        case decayState:
            
            if (statePrev != decayState)
                dScaler = envelopeLevel;
            
            if (envelopeLevel - decaySlope <= sustain)
            {
                envelopeState = sustainState;
                envelopeLevel = sustain;
                envelopeOutLevel = envelopeLevel;
                
                statePrev = decayState;
                break;
            }
            envelopeLevel -= decaySlope;
            
            envelopeOutLevel = std::powf((envelopeLevel - sustain) / (dScaler - sustain), decayShapeCoef) * (dScaler - sustain) + sustain;

            statePrev = decayState;
            break;
            
        case sustainState:
            
            envelopeLevel = sustain;
            envelopeOutLevel = envelopeLevel;
            
            statePrev = sustainState;
            break;
            
        case releaseState:
            
            if (statePrev !=releaseState)
            {
                rScaler = envelopeLevel;
                rScaler2 = envelopeOutLevel;
                releaseSlope = rScaler / ((release * sampleRate * 2.0) + 1.0f);
            }
            
            if (envelopeLevel - releaseSlope <= 0.0f)
            {
                envelopeLevel = 0.0f;
                envelopeOutLevel = 0.0f;
                envelopeState = idleState;
            }
            
            envelopeLevel -= releaseSlope;
            envelopeOutLevel = std::powf(envelopeLevel / rScaler, releaseShapeCoef) * rScaler2;
            
            statePrev = releaseState;
            break;
            
        default:
            envelopeOutLevel = 0.0f;
            envelopeLevel = 0.0f;
            break;
    }
}