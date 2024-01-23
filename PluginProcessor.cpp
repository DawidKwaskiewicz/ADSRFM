/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

ADSRFM4AudioProcessor::ADSRFM4AudioProcessor() : synthAudioSource(keyboardState, &procA[0], &procD[0], &procS[0], &procR[0], &procI[0], &procfmfc[0], &currentWave[0], &longestBaseModule)
#ifndef JucePlugin_PreferredChannelConfigurations
, AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
    .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
    .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
)

#endif
{
    /*for (int i = 0; i < 8; i++)
    {
        double sr = getSampleRate();
        procA[i] = 25.0 * sr / 1000.0;
        procD[i] = 100.0 * sr / 1000.0;
        procS[i] = 30.0 / 100.0;
        procR[i] = 500.0 * sr / 1000.0;
        procI[i] = 0.0;
        procfmfc[i] = 1.0;
        currentWave[i] = 0;
    }*/
}

ADSRFM4AudioProcessor::~ADSRFM4AudioProcessor()
{
}

//==============================================================================
const juce::String ADSRFM4AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ADSRFM4AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ADSRFM4AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ADSRFM4AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ADSRFM4AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ADSRFM4AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ADSRFM4AudioProcessor::getCurrentProgram()
{
    return 0;
}

void ADSRFM4AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ADSRFM4AudioProcessor::getProgramName (int index)
{
    return {};
}

void ADSRFM4AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ADSRFM4AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synthAudioSource.prepareToPlay(samplesPerBlock, sampleRate);
    synthAudioSource.midiCollector.reset(sampleRate);
}

void ADSRFM4AudioProcessor::releaseResources()
{
    synthAudioSource.releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ADSRFM4AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ADSRFM4AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::MidiMessage m;
    int time;
    for (juce::MidiBuffer::Iterator i(midiMessages); i.getNextEvent(m, time);)
        synthAudioSource.midiCollector.addMessageToQueue(m);
    juce::AudioSourceChannelInfo channelInfo = juce::AudioSourceChannelInfo(buffer);
    synthAudioSource.getNextAudioBlock(channelInfo);
}

//==============================================================================
bool ADSRFM4AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ADSRFM4AudioProcessor::createEditor()
{
    return new ADSRFM4AudioProcessorEditor (*this);
}

//==============================================================================
void ADSRFM4AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ADSRFM4AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ADSRFM4AudioProcessor();
}
