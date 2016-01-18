/*
  ==============================================================================

    FMOperator.cpp
    Created: 16 Jan 2016 5:54:15pm
    Author:  John Caplan

  ==============================================================================
*/

#include "FMOP.h"

FMOP::FMOP()
: angleDelta (0.0f),
  currentAngle (0.0f),
  OpMidiPitch (0.0f),
  OpLinearFM (0.0f),
  envShape (0.85f)
{
    OpENV.setEnvelopeState(Envelope::idleState);
}

FMOP::~FMOP()
{
}

float FMOP::getOpstate()
{
    return OpENV.getEnvelopeState();
}

void FMOP::updateAngleDelta()
{
    angleDelta = (mtf(OpMidiPitch) / OpSampleRate * twopi);
}

void FMOP::setOpPitch(float oppitch)
{
    OpMidiPitch = oppitch;
}

void FMOP::setOpLinearFm(float linearFM)
{
    OpLinearFM = linearFM;
}

void FMOP::ResetPhase()
{
    currentAngle = float_Pi * 1.5f;
}

void FMOP::SetENVParam(float attack, float decay, float sustain, float release)
{
    OpENV.setEnvelopeParams(attack, decay, sustain, release);
}

void FMOP::StartOP(float midiNote, float sampleRate)
{
    OpSampleRate = sampleRate;
    OpMidiPitch = midiNote;
    OpLinearFM = 0.0f;
    updateAngleDelta ();
    OpENV.setSampleRate(sampleRate);
    OpENV.setEnvelopeState(Envelope::attackState);
}

void FMOP::StopOp()
{
    OpENV.setEnvelopeState(Envelope::releaseState);
}

float FMOP::RenderOP()
{
    OpENV.renderEnvelope();
    if (OpENV.getEnvelopeState() == Envelope::idleState)
        return 0.0f;
    
    currentAngle += angleDelta;
    if (currentAngle >= twopi)
        currentAngle -= twopi;
    return SineOfAngle (currentAngle + OpLinearFM) * std::pow(OpENV.getenvelopeLevel(), calulateShapeCoeff(envShape));
}