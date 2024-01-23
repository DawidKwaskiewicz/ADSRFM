/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthAudioSource.h"

//==============================================================================
/**
*/
class ADSRFM4AudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    ADSRFM4AudioProcessor();
    ~ADSRFM4AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::MidiKeyboardState keyboardState;
    SynthAudioSource synthAudioSource;
    //int modulesCount = 8;

    /*double procA[8]{ 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0 };
    double procD[8]{ 100.0, 100.0, 0.0, 0.0, 100.0, 0.0, 0.0, 100.0 };
    double procS[8]{ 30.0, 30.0, 100.0, 100.0, 30.0, 100.0, 100.0, 30.0 };
    double procR[8]{ 500.0, 500.0, 500.0, 500.0, 500.0, 500.0, 500.0, 500.0 };
    double procI[8]{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    double procfmfc[8]{ 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
    int currentWave[8]{ 0, 0, 0, 0, 0, 0, 0, 0 };*/
    double procA[8];
    double procD[8];
    double procS[8];
    double procR[8];
    double procI[8];
    double procfmfc[8];
    int currentWave[8];
    int longestBaseModule = 0;


private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSRFM4AudioProcessor)
};
