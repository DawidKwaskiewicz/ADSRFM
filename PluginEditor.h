/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ADSRFM4AudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Timer, private juce::Slider::Listener
{
public:
    ADSRFM4AudioProcessorEditor (ADSRFM4AudioProcessor&);
    ~ADSRFM4AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void onStateSwitch();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ADSRFM4AudioProcessor& audioProcessor;

    void timerCallback() override
    {
        keyboardComponent.grabKeyboardFocus();
        stopTimer();
    }

    void sliderValueChanged(juce::Slider* slider) override;

    juce::MidiKeyboardComponent keyboardComponent;
    juce::Slider sliderA;
    juce::Slider sliderD;
    juce::Slider sliderS;
    juce::Slider sliderR;
    juce::Label labelA;
    juce::Label labelD;
    juce::Label labelS;
    juce::Label labelR;

    juce::Slider sliderI;
    juce::Slider sliderfmfc;
    juce::Label labelI;
    juce::Label labelfmfc;
    juce::TextButton sine, pulse, triangle, sawtooth;
    juce::TextButton m1, m2, m3, m4, m5, m6, m7, m8;
    juce::TextButton* modules[8] = { &m1, &m2, &m3, &m4, &m5, &m6, &m7, &m8 };

    juce::Colour cText = juce::Colours::cadetblue;
    juce::Colour cBackground = getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId);

    int currentModule;
    int prevCurrentModule;
    int modulesCount = 8;
    double moduleTimes[8];

    double a[8] = { 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0 };
    double d[8] = { 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0 };
    double s[8] = { 30.0, 30.0, 100.0, 100.0, 30.0, 100.0, 100.0, 30.0 };
    double r[8] = { 500.0, 500.0, 500.0, 500.0, 500.0, 500.0, 500.0, 500.0 };
    double I[8] = { 5.0, 5.0, 0.0, 0.0, 5.0, 0.0, 0.0, 5.0 };
    double f[8] = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
    int c[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    //int longestReleaseModule = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSRFM4AudioProcessorEditor)
};
