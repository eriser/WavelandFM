/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
WavelandFmAudioProcessor::WavelandFmAudioProcessor()
{
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < Ops[i].Parms.size(); ++j)
        {
            addParameter(Ops[i].Parms[j].param = new AudioParameterFloat (Ops[i].Parms[j].nameLong,
                                                                          Ops[i].Parms[j].nameShort,
                                                                          0.0f,
                                                                          1.0f,
                                                                          Ops[i].Parms[j].DefaultValue));
        }
    }
    innitSynth();
}

WavelandFmAudioProcessor::~WavelandFmAudioProcessor()
{
}

void WavelandFmAudioProcessor::updateParameters()
{
    for (int h = 8; --h > 0 ; )
    {
        if (FMSynthVoice* myVoice = dynamic_cast<FMSynthVoice*> ( synth.getVoice(h) ))
        {
            for (int i = 0; i < 2; ++i)
            {
                myVoice->setEnvParams(i, *(Ops[i].Parms[0].param),
                                         *(Ops[i].Parms[1].param),
                                         *(Ops[i].Parms[2].param),
                                         *(Ops[i].Parms[3].param),
                                         *(Ops[i].Parms[4].param),
                                         *(Ops[i].Parms[5].param));
            }
        }
    }
}

//==============================================================================

void WavelandFmAudioProcessor::innitSynth()
{
    const int numVoices = 8;
    
    for (int i = numVoices; --i >= 0;)
    {
        synth.addVoice(new FMSynthVoice());
    }
    
    for (int h = numVoices; --h > 0 ; )
    {
        if (FMSynthVoice* myVoice = dynamic_cast<FMSynthVoice*> ( synth.getVoice(h) ))
        {
            for (int i = 0; i < 2; ++i)
            {
                myVoice->setEnvParams(i, *(Ops[i].Parms[0].param),
                                         *(Ops[i].Parms[1].param),
                                         *(Ops[i].Parms[2].param),
                                         *(Ops[i].Parms[3].param),
                                         *(Ops[i].Parms[4].param),
                                         *(Ops[i].Parms[5].param));
            }
        }
    }

    
    synth.addSound(new FMSynthSound());
}

const String WavelandFmAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WavelandFmAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WavelandFmAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WavelandFmAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double WavelandFmAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WavelandFmAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WavelandFmAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WavelandFmAudioProcessor::setCurrentProgram (int index)
{
}

const String WavelandFmAudioProcessor::getProgramName (int index)
{
    return String();
}

void WavelandFmAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void WavelandFmAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate (sampleRate);
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void WavelandFmAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void WavelandFmAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int numSamples = buffer.getNumSamples();
    synth.renderNextBlock(buffer, midiMessages, 0, numSamples);
}

//==============================================================================
bool WavelandFmAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* WavelandFmAudioProcessor::createEditor()
{
    return new WavelandFmAudioProcessorEditor (*this);
}

//==============================================================================
void WavelandFmAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void WavelandFmAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WavelandFmAudioProcessor();
}
