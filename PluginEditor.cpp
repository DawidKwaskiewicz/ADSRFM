/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ADSRFM4AudioProcessorEditor::ADSRFM4AudioProcessorEditor (ADSRFM4AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), keyboardComponent(audioProcessor.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)

{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible(keyboardComponent);
    setSize(750, 300);
    startTimer(400);
    currentModule = 0;
    prevCurrentModule = 0;

    sliderA.setSliderStyle(juce::Slider::Rotary);
    sliderA.setRange(1.0, 250.0, 1.0);;
    sliderA.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    sliderA.setPopupDisplayEnabled(true, false, this);
    sliderA.setTextValueSuffix(" ms");
    sliderA.setValue(25.0);
    addAndMakeVisible(&sliderA);
    sliderA.addListener(this);

    labelA.setText("Attack", juce::dontSendNotification);
    labelA.setColour(juce::Label::textColourId, juce::Colours::cadetblue);
    labelA.setJustificationType(juce::Justification::centred);
    labelA.attachToComponent(&sliderA, false);
    addAndMakeVisible(&labelA);

    sliderD.setSliderStyle(juce::Slider::Rotary);
    sliderD.setRange(1.0, 500.0, 1.0);;
    sliderD.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    sliderD.setPopupDisplayEnabled(true, false, this);
    sliderD.setTextValueSuffix(" ms");
    sliderD.setValue(100.0);
    addAndMakeVisible(&sliderD);
    sliderD.addListener(this);

    labelD.setText("Decay", juce::dontSendNotification);
    labelD.setColour(juce::Label::textColourId, juce::Colours::cadetblue);
    labelD.setJustificationType(juce::Justification::centred);
    labelD.attachToComponent(&sliderD, false);
    addAndMakeVisible(&labelD);

    sliderS.setSliderStyle(juce::Slider::Rotary);
    sliderS.setRange(0.0, 100.0, 1.0);;
    sliderS.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    sliderS.setPopupDisplayEnabled(true, false, this);
    sliderS.setTextValueSuffix(" %");
    sliderS.setValue(30.0);
    addAndMakeVisible(&sliderS);
    sliderS.addListener(this);

    labelS.setText("Sus", juce::dontSendNotification);
    labelS.setColour(juce::Label::textColourId, juce::Colours::cadetblue);
    labelS.setJustificationType(juce::Justification::centred);
    labelS.attachToComponent(&sliderS, false);
    addAndMakeVisible(&labelS);

    sliderR.setSliderStyle(juce::Slider::Rotary);
    sliderR.setRange(10.0, 4000.0, 10.0);;
    sliderR.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    sliderR.setPopupDisplayEnabled(true, false, this);
    sliderR.setTextValueSuffix(" ms");
    sliderR.setValue(500.0);
    addAndMakeVisible(&sliderR);
    sliderR.addListener(this);

    labelR.setText("Release", juce::dontSendNotification);
    labelR.setColour(juce::Label::textColourId, juce::Colours::cadetblue);
    labelR.setJustificationType(juce::Justification::centred);
    labelR.attachToComponent(&sliderR, false);
    addAndMakeVisible(&labelR);

    sliderI.setSliderStyle(juce::Slider::Rotary);
    sliderI.setRange(0.0, 10.0, 0.1);;
    sliderI.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    sliderI.setPopupDisplayEnabled(true, false, this);
    //sliderI.setTextValueSuffix(" ms");
    sliderI.setValue(5.0);
    addAndMakeVisible(&sliderI);
    sliderI.addListener(this);

    //labelI.setText("Modulation Index", juce::dontSendNotification);
    labelI.setText("Volume", juce::dontSendNotification);
    labelI.setColour(juce::Label::textColourId, juce::Colours::cadetblue);
    labelI.setJustificationType(juce::Justification::centred);
    labelI.attachToComponent(&sliderI, false);
    addAndMakeVisible(&labelI);

    sliderfmfc.setSliderStyle(juce::Slider::Rotary);
    sliderfmfc.setRange(1.0, 10.0, 0.5);;
    sliderfmfc.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    sliderfmfc.setPopupDisplayEnabled(true, false, this);
    //sliderfcfm.setTextValueSuffix(" ms");
    sliderfmfc.setValue(1.0);
    addAndMakeVisible(&sliderfmfc);
    sliderfmfc.addListener(this);

    //labelfmfc.setText("fc/fm", juce::dontSendNotification);
    labelfmfc.setText("TBA...", juce::dontSendNotification);
    labelfmfc.setColour(juce::Label::textColourId, juce::Colours::cadetblue);
    labelfmfc.setJustificationType(juce::Justification::centred);
    labelfmfc.attachToComponent(&sliderfmfc, false);
    addAndMakeVisible(&labelfmfc);


    sine.setButtonText("Sine");
    sine.setToggleState(true, juce::sendNotification);
    sine.setClickingTogglesState(true);
    sine.setColour(juce::TextButton::buttonOnColourId, cText);
    sine.setColour(juce::TextButton::textColourOnId, cBackground);
    sine.setColour(juce::TextButton::textColourOffId, cText);
    sine.setRadioGroupId(1);
    addAndMakeVisible(&sine);
    sine.onClick = [this] {ADSRFM4AudioProcessorEditor::onStateSwitch(); };

    pulse.setButtonText("Pulse");
    pulse.setToggleState(false, juce::sendNotification);
    pulse.setClickingTogglesState(true);
    pulse.setColour(juce::TextButton::buttonOnColourId, cText);
    pulse.setColour(juce::TextButton::textColourOnId, cBackground);
    pulse.setColour(juce::TextButton::textColourOffId, cText);
    pulse.setRadioGroupId(1);
    addAndMakeVisible(&pulse);
    pulse.onClick = [this] {ADSRFM4AudioProcessorEditor::onStateSwitch(); };

    triangle.setButtonText("Triangle");
    triangle.setToggleState(false, juce::sendNotification);
    triangle.setClickingTogglesState(true);
    triangle.setColour(juce::TextButton::buttonOnColourId, cText);
    triangle.setColour(juce::TextButton::textColourOnId, cBackground);
    triangle.setColour(juce::TextButton::textColourOffId, cText);
    triangle.setRadioGroupId(1);
    addAndMakeVisible(&triangle);
    triangle.onClick = [this] {ADSRFM4AudioProcessorEditor::onStateSwitch(); };

    sawtooth.setButtonText("Sawtooth");
    sawtooth.setToggleState(false, juce::sendNotification);
    sawtooth.setClickingTogglesState(true);
    sawtooth.setColour(juce::TextButton::buttonOnColourId, cText);
    sawtooth.setColour(juce::TextButton::textColourOnId, cBackground);
    sawtooth.setColour(juce::TextButton::textColourOffId, cText);
    sawtooth.setRadioGroupId(1);
    addAndMakeVisible(&sawtooth);
    sawtooth.onClick = [this] {ADSRFM4AudioProcessorEditor::onStateSwitch(); };
    double sr = audioProcessor.getSampleRate();

    for (int i = 0; i < 8; i++)
    {
        modules[i]->setButtonText(juce::String(i+1));
        if (i==currentModule) modules[i]->setToggleState(true, juce::sendNotification);
        else modules[i]->setToggleState(false, juce::sendNotification);
        modules[i]->setClickingTogglesState(true);
        modules[i]->setColour(juce::TextButton::buttonOnColourId, cText);
        modules[i]->setColour(juce::TextButton::textColourOnId, cBackground);
        modules[i]->setColour(juce::TextButton::textColourOffId, cText);
        modules[i]->setRadioGroupId(2);
        addAndMakeVisible(modules[i]);
        modules[i]->onClick = [this] {ADSRFM4AudioProcessorEditor::onStateSwitch(); };
        if (r[i] > r[audioProcessor.longestBaseModule] && (i == 0 || i == 1 || i == 4 || i == 7))
            audioProcessor.longestBaseModule = i;


        /*audioProcessor.procA[i] = a[i] * sr / 1000.0;
        audioProcessor.procD[i] = d[i] * sr / 1000.0;
        audioProcessor.procS[i] = s[i] / 100.0;
        audioProcessor.procR[i] = r[i] * sr / 1000.0;
        audioProcessor.procI[i] = I[i];
        audioProcessor.procfmfc[i] = f[i];*/
    }

    DBG("works");
}

ADSRFM4AudioProcessorEditor::~ADSRFM4AudioProcessorEditor()
{
}

//==============================================================================
void ADSRFM4AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
    g.drawFittedText("ADSR+FM", 0, 0, getWidth(), 30, juce::Justification::centred, 1);

    float midx0 = modules[0]->getX() + modules[0]->getWidth() / 2;
    float midx1 = modules[1]->getX() + modules[1]->getWidth() / 2;
    float midx2 = modules[2]->getX() + modules[2]->getWidth() / 2;
    float midx3 = modules[7]->getX() + modules[7]->getWidth() / 2;
    float tail = 8.0f;
    float bottom = modules[0]->getBottom();
    float thickness = 2.0f;
    g.drawLine(midx2, modules[2]->getBottom(), midx2, modules[3]->getY(), thickness);
    g.drawLine(midx2, modules[3]->getBottom(), midx2, modules[4]->getY(), thickness);
    g.drawLine(midx3, modules[5]->getBottom(), midx3, modules[6]->getY(), thickness);
    g.drawLine(midx3, modules[6]->getBottom(), midx3, modules[7]->getY(), thickness);
    g.drawLine(midx0, bottom, midx0, bottom + tail, thickness);
    g.drawLine(midx1, bottom, midx1, bottom + tail, thickness);
    g.drawLine(midx2, bottom, midx2, bottom + tail, thickness);
    g.drawLine(midx3, bottom, midx3, bottom + tail, thickness);
    g.drawLine(midx0, bottom + tail, midx3, bottom + tail, thickness);
}

void ADSRFM4AudioProcessorEditor::resized()
{
    keyboardComponent.setBounds(10, 10, 500, getHeight() - 150);
    sliderA.setBounds(10, 200, 100, 100);
    sliderD.setBounds(120, 200, 100, 100);
    sliderS.setBounds(230, 200, 100, 100);
    sliderR.setBounds(340, 200, 100, 100);
    sliderI.setBounds(450, 200, 100, 100);
    sliderfmfc.setBounds(560, 200, 100, 100);
    sine.setBounds(670, 20, 70, 50);
    pulse.setBounds(670, 90, 70, 50);
    triangle.setBounds(670, 160, 70, 50);
    sawtooth.setBounds(670, 230, 70, 50);

    int box = 25;
    int spacex = 10;
    int spacey = 25;
    int x = 530;
    m1.setBounds(x, spacey * 3 + box * 2, box, box);
    m2.setBounds(x + box + spacex, spacey * 3 + box * 2, box, box);
    m3.setBounds(x + 2*box + 2*spacex, spacey, box, box);
    m4.setBounds(x + 2 * box + 2 * spacex, 2*spacey + box, box, box);
    m5.setBounds(x + 2 * box + 2 * spacex, 3 * spacey + 2 * box, box, box);
    m6.setBounds(x + 3 * box + 3 * spacex, spacey, box, box);
    m7.setBounds(x + 3 * box + 3 * spacex, 2 * spacey + box, box, box);
    m8.setBounds(x + 3 * box + 3 * spacex, 3 * spacey + 2 * box, box, box);
}

void ADSRFM4AudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    double sr = audioProcessor.getSampleRate();
    a[currentModule] = sliderA.getValue();
    d[currentModule] = sliderD.getValue();
    s[currentModule] = sliderS.getValue();
    r[currentModule] = sliderR.getValue();
    I[currentModule] = sliderI.getValue();
    f[currentModule] = sliderfmfc.getValue();
    audioProcessor.procA[currentModule] = a[currentModule] * sr / 1000.0;
    audioProcessor.procD[currentModule] = d[currentModule] * sr / 1000.0;
    audioProcessor.procS[currentModule] = s[currentModule] / 100.0;
    audioProcessor.procR[currentModule] = r[currentModule] * sr / 1000.0;
    audioProcessor.procI[currentModule] = I[currentModule];
    audioProcessor.procfmfc[currentModule] = f[currentModule];
    for (int i = 0; i < modulesCount; i++)
        if (r[i] > r[audioProcessor.longestBaseModule] && (i == 0 || i == 1 || i == 4 || i == 7))
            audioProcessor.longestBaseModule = i;
}

void ADSRFM4AudioProcessorEditor::onStateSwitch()
{
    for (int i = 0; i < modulesCount; i++)
    {
        if (modules[i]->getToggleState())
        {
            currentModule = i;
            break;
        }
    }
    //DBG(juce::String(currentModule));

    if (currentModule != prevCurrentModule)
    {
        sliderA.setValue(a[currentModule]);
        sliderD.setValue(d[currentModule]);
        sliderS.setValue(s[currentModule]);
        sliderR.setValue(r[currentModule]);
        sliderI.setValue(I[currentModule]);
        sliderfmfc.setValue(f[currentModule]);
        switch (c[currentModule])
        {
        case 0: sine.setToggleState(true, juce::NotificationType::dontSendNotification);
            break;
        case 1: pulse.setToggleState(true, juce::NotificationType::dontSendNotification);
            break;
        case 2: triangle.setToggleState(true, juce::NotificationType::dontSendNotification);
            break;
        case 3: sawtooth.setToggleState(true, juce::NotificationType::dontSendNotification);
            break;
        }
        if (currentModule == 0 || currentModule == 1 || currentModule == 4 || currentModule == 7)
        {
            labelI.setText("Volume", juce::NotificationType::dontSendNotification);
            labelfmfc.setText("TBA...", juce::NotificationType::dontSendNotification);
        }
        else
        {
            labelI.setText("Modulation index", juce::NotificationType::dontSendNotification);
            labelfmfc.setText("fc/fm", juce::NotificationType::dontSendNotification);
        }
    }
    else
    {
        if (sine.getToggleState())
        {
            c[currentModule] = 0;
            audioProcessor.currentWave[currentModule] = 0;
        }
        else if (pulse.getToggleState())
        {
            c[currentModule] = 1;
            audioProcessor.currentWave[currentModule] = 1;
        }
        else if (triangle.getToggleState())
        {
            c[currentModule] = 2;
            audioProcessor.currentWave[currentModule] = 2;
        }
        else if (sawtooth.getToggleState())
        {
            c[currentModule] = 3;
            audioProcessor.currentWave[currentModule] = 3;
        }
    }

    prevCurrentModule = currentModule;
}
