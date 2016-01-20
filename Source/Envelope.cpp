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
  shape (0.5f),
  dScaler(0.0f),
  rScaler(0.0f),
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

void Envelope::setEnvelopeParams (float attackParam, float decayParam, float sustainParam, float releaseParam, float shapeParam)
{
    attack = attackParam;
    decay = decayParam;
    sustain = sustainParam;
    release = releaseParam;
    shape = shapeParam;
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
    switch (envelopeState)
    {
        case idleState:
            return 0.0f;
            break;
            
        case attackState:
            return envelopeOutLevel;
            break;
            
        case decayState:
            return envelopeOutLevel;
            break;
            
        case sustainState:
            return envelopeOutLevel;
            break;
            
        case releaseState:
            return envelopeOutLevel;
            break;
            
        default:
            return 0.0f;
            break;
    }
}

void Envelope::startEnvelope()
{
    envelopeState = attackState;
    envlopeTick = 0;
}

void Envelope::endEnvelope()
{
    envelopeState = releaseState;
    releasetick = 0;
}

void Envelope::renderEnvelope ()
{
    double attackSlope = 1.0 / ((attack * sampleRate * 2.0) + 1.0);
    double decaySlope = (1.0 - sustain) / ((decay * sampleRate * 2.0) + 1.0);
    double releaseSlope = sustain / ((release * sampleRate * 2.0) + 1.0);
    
    
    switch (envelopeState)
    {
            
        case idleState:
            
            envlopeTick = 0.0;
            releasetick = 0.0;
            envelopeLevel = 0.0;
            envelopeOutLevel = 0.0;
            
            statePrev = idleState;
            break;
            
        case attackState:
            
            envelopeLevel += attackSlope;
            envelopeOutLevel = envelopeLevel;
            
            if (envelopeLevel >= 1.0)
            {
                envelopeState = decayState;
                envelopeLevel = 1.0;
                envelopeOutLevel = 1.0;
            }
            
            ++envlopeTick;
            
            statePrev = attackState;
            break;
            
        case decayState:
            
            if (statePrev != decayState)
                dScaler = envelopeLevel;
            
            if (envelopeLevel <= sustain)
            {
                envelopeState = sustainState;
                envelopeLevel = sustain;
                envelopeOutLevel = envelopeLevel;
                
                statePrev = decayState;
                break;
            }
            envelopeLevel -= decaySlope;
            
            envelopeOutLevel = std::powf((envelopeLevel - sustain) / (dScaler - sustain), calulateShapeCoeff(shape)) * (dScaler - sustain) + sustain;
            
            statePrev = decayState;
            break;
            
        case sustainState:
            
            envelopeLevel = sustain;
            envelopeOutLevel = envelopeLevel;
            
            ++envlopeTick;
            
            statePrev = sustainState;
            break;
            
        case releaseState:
            
            if (statePrev !=releaseState)
                rScaler = envelopeOutLevel;

            envelopeLevel -= releaseSlope;
            envelopeOutLevel = std::powf(envelopeLevel / rScaler, calulateShapeCoeff(shape)) * rScaler;
            
            if (envelopeLevel < 0.0)
            {
                envelopeLevel = 0.0;
                envelopeState = idleState;
            }
            
            ++envlopeTick;
            ++releasetick;
            
            statePrev = releaseState;
            break;
            
        default:
            break;
    }
}