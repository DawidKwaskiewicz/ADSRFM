#pragma once
#include "JuceHeader.h"
//==============================================================================
struct SineWaveSound : public juce::SynthesiserSound
{
	SineWaveSound() {}
	bool appliesToNote(int) override { return true; }
	bool appliesToChannel(int) override { return true; }
};
//==============================================================================
struct SineWaveVoice : public juce::SynthesiserVoice
{
	//SineWaveVoice(const juce::AudioSampleBuffer& wavetableToUse, unsigned int voices, juce::AudioSource *inputSource) : wavetable(wavetableToUse)
	SineWaveVoice(/*const juce::AudioSampleBuffer& wavetableToUse,*/ unsigned int voices,
		//double* ptrA[8], double* ptrD[8], double* ptrS[8], double* ptrR[8], double* ptrI[8], double* ptrfcfm[8], int* ptrCurrentWave[8], int* lm) : wavetable(wavetableToUse)
		//double (*ptrA)[8], double (*ptrD)[8], double (*ptrS)[8], double (*ptrR)[8], double (*ptrI)[8], double (*ptrfcfm)[8], int (*ptrCurrentWave)[8], int* lm)// : wavetable(wavetableToUse)
		double *ptrA, double *ptrD, double *ptrS, double *ptrR, double *ptrI, double *ptrfcfm, int *ptrCurrentWave, int* lm)
	{
		globalLevel = 0.5 / (float)voices;
		voiceA = ptrA;
		voiceD = ptrD;
		voiceS = ptrS;
		voiceR = ptrR;
		fmI = ptrI;
		fmfc = ptrfcfm;
		longestModule = lm;
		currentWave = ptrCurrentWave;
		//jassert(wavetable.getNumChannels() == 1);
		for (int i = 0; i < 8; i++)
		{
			/*voiceA[i] = ptrA[i];
			voiceD[i] = ptrD[i];
			voiceS[i] = ptrS[i];
			voiceR[i] = ptrR[i];
			fmI[i] = ptrI[i];
			fmfc[i] = ptrfcfm[i];*/
			/**voiceA[i] = 25.0 * getSampleRate() / 1000.0;
			*voiceD[i] = 100.0 * getSampleRate() / 1000.0;
			*voiceS[i] = 0.3;
			*voiceR[i] = 500.0 * getSampleRate() / 1000.0;
			*fmI[i] = 0.0;
			*fmfc[i] = 1.0;
			*lm = 0;
			currentWave[i] = ptrCurrentWave[i];*/
			if (i == 0 || i == 1 || i == 4 || i == 7)
			{
				*(voiceA + i) = 25.0 * getSampleRate() / 1000.0;
				*(voiceD + i) = 100.0 * getSampleRate() / 1000.0;
				*(voiceS + i) = 0.3;
				*(voiceR + i) = 500.0 * getSampleRate() / 1000.0;
				*(fmI + i) = 5.0;
				*(fmfc + i) = 1.0;
				*(currentWave + i) = 0;
			}
			else
			{
				*(voiceA + i) = 25.0 * getSampleRate() / 1000.0;
				*(voiceD + i) = 100.0 * getSampleRate() / 1000.0;
				*(voiceS + i) = 1.0;
				*(voiceR + i) = 500.0 * getSampleRate() / 1000.0;
				*(fmI + i) = 0.0;
				*(fmfc + i) = 1.0;
				*(currentWave + i) = 0;
			}
			//*(voiceA + i) = 25.0 * getSampleRate() / 1000.0;
			//*(voiceD + i) = 100.0 * getSampleRate() / 1000.0;
			//*(voiceS + i) = 0.3;
			//*(voiceR + i) = 500.0 * getSampleRate() / 1000.0;
			//*(fmI + i) = 0.0;
			//*(fmfc + i) = 1.0;
			*lm = 0;
			currentAngle[i] = 0.0;
			angleDelta[i] = 0.0;
			level[i] = 0.0;
			ampIncrease[i] = 0.0;
			tmpLevel[i] = 0.0;
			mValues[i] = 0.0;
			currentSegment[i] = 0;
			currentPosInSegment[i] = 0;
		}
	}

	/*void setFrequency(float frequency, float sampleRate)
	{
		auto tableSizeOverSampleRate = wavetable.getNumSamples() / sampleRate;
		tableDelta = frequency * tableSizeOverSampleRate;
	}*/

	//forcedinline float getNextSample() noexcept
	//{
	//	//tableDelta *= pitchWheelCoef;
	//	auto tableSize = wavetable.getNumSamples();
	//	auto index0 = (unsigned int)currentIndex;
	//	auto index1 = index0 == (tableSize - 1) ? (unsigned int)0 : index0 + 1;
	//	auto frac = currentIndex - (float)index0;
	//	auto* table = wavetable.getReadPointer(0);
	//	auto value0 = table[index0];
	//	auto value1 = table[index1];
	//	auto currentSample = value0 + frac * (value1 - value0);
	//	//if ((currentIndex += tableDelta) > tableSize)
	//	if ((currentIndex += tableDelta * pitchWheelCoef) > tableSize)
	//		currentIndex -= tableSize;
	//	else if (currentIndex < 0)
	//		currentIndex += tableSize;
	//	return currentSample;
	//}

	forcedinline float getNextSample() noexcept
	{
		//if (currentSegment[0] != prevSegment0) DBG(juce::String(currentSegment[0]));
		//prevSegment0 = currentSegment[0];
		for (int i = 0; i < 8; i++)
		{
			currentAngle[i] += angleDelta[i];
			if (currentAngle[i] >= juce::MathConstants<double>::twoPi)
				currentAngle[i] = std::fmod(currentAngle[i], juce::MathConstants<double>::twoPi);
		}
		mValues[0] = getCurrentLevel(0) * getWave(0, currentAngle[0] * (double)pitchWheelCoef);
		mValues[1] = getCurrentLevel(1) * getWave(1, currentAngle[1] * (double)pitchWheelCoef);
		mValues[2] = getCurrentLevel(2) * getWave(2, currentAngle[2]);
		mValues[3] = getCurrentLevel(3) * getWave(3, currentAngle[3] + mValues[2]);
		mValues[4] = getCurrentLevel(4) * getWave(4, currentAngle[4] * (double)pitchWheelCoef + mValues[3]);
		mValues[5] = getCurrentLevel(5) * getWave(5, currentAngle[5]);
		mValues[6] = getCurrentLevel(6) * getWave(6, currentAngle[6] + mValues[5]);
		mValues[7] = getCurrentLevel(7) * getWave(7, currentAngle[7] * (double)pitchWheelCoef + mValues[6]);
		/*mValues[0] = getWave(0, currentAngle[0]);
		mValues[1] = getWave(1, currentAngle[1]);
		mValues[2] = getWave(2, currentAngle[2]);
		mValues[3] = getWave(3, currentAngle[3] + mValues[2]);
		mValues[4] = getWave(4, currentAngle[4] + mValues[3]);
		mValues[5] = getWave(5, currentAngle[5]);
		mValues[6] = getWave(6, currentAngle[6] + mValues[5]);
		mValues[7] = getWave(7, currentAngle[7] + mValues[6]);*/
		return mValues[0] * 0.25 + mValues[1] * 0.25 + mValues[4] * 0.25 + mValues[7] * 0.25;
	}


	bool canPlaySound(juce::SynthesiserSound* sound) override
	{
		return dynamic_cast<SineWaveSound*> (sound) != nullptr;
	}
	//void startNote(int midiNoteNumber, float velocity,
	//	juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
	//{
	//	currentIndex = 0.0f;
	//	currentSegment = 1;
	//	currentPosInSegment = 0;
	//	level = velocity * globalLevel;
	//	tmpLevel = 0.0f;
	//	ampIncrease = level / (*voiceA);
	//	setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber), getSampleRate());
	//}
	void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
	{
		tailOff = 0.0;
		//DBG("Start");
		for (int i = 0; i < 8; i++)
		{
			currentSegment[i] = 1;
			currentPosInSegment[i] = 0;
			currentAngle[i] = 0.0;
			if (i == 0 || i == 1 || i == 4 || i == 7)
			{
				level[i] = velocity * globalLevel * (*(fmI + i)) / 10.0;
				angleDelta[i] = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) / getSampleRate() * juce::MathConstants<double>::twoPi;
			}
			else if (i == 3 || i == 6)
			{
				level[i] = *(fmI + i);
				angleDelta[i] = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) / getSampleRate() * juce::MathConstants<double>::twoPi * *(fmfc + i);
			}
			else if (i == 2 || i == 5)
			{
				level[i] = *(fmI + i);
				angleDelta[i] = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) / getSampleRate() * juce::MathConstants<double>::twoPi * *(fmfc + i) * *(fmfc + i + 1);
			}
			tmpLevel[i] = 0.0f;
			ampIncrease[i] = level[i] / (*(voiceA + i));
			//auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
			//auto cyclesPerSample = cyclesPerSecond / getSampleRate();
			//angleDelta[i] = cyclesPerSample * juce::MathConstants<double>::twoPi;
		}
	}
	void stopNote(float /*velocity*/, bool allowTailOff) override
	{
		//DBG("Stop");
		for (int i = 0; i < 8; i++)
		{
			currentPosInSegment[i] = 0;
			currentSegment[i] = 4;
			ampIncrease[i] = -1.0 * (tmpLevel[i] / (*(voiceR + i)));
		}
		/*if (allowTailOff)
		{
			if (tailOff == 0.0)
				tailOff = 1.0;
		}
		else
		{
			clearCurrentNote();
			for (int i = 0; i < 8; i++)
				angleDelta[i] = 0.0;
		}*/
	}
	void controllerMoved(int, int) override {}

	void pitchWheelMoved(int newPitchWheelValue) override
	{
		pitchWheelCoef = std::powf(2.0f, ((float)(newPitchWheelValue - 8192)) / 49152.0f);
	}
	double getSine(double angle)
	{
		return std::sin(angle);
	}
	double getPulse(double angle)
	{
		return std::sin(angle) >= 0 ? 1 : -1;
	}
	double getTriangle(double angle)
	{
		return 2.0 * std::abs(2.0 * (angle / juce::MathConstants<double>::twoPi - std::floor(angle / juce::MathConstants<double>::twoPi + 0.5))) - 1.0;
	}
	double getSawtooth(double angle)
	{
		return 2.0 * (angle / juce::MathConstants<double>::twoPi - std::floor(0.5 + angle / juce::MathConstants<double>::twoPi));
	}
	double getWave(int moduleInd, double angle)
	{
		switch (*(currentWave + moduleInd))
		{
		case 0: return getSine(angle);
		case 1: return getPulse(angle);
		case 2: return getTriangle(angle);
		case 3: return getSawtooth(angle);
		default: return 0.0;
		}
	}
	double getCurrentLevel(int moduleInd)
	{
		if (currentSegment[moduleInd] > 0)
		{
			tmpLevel[moduleInd] += ampIncrease[moduleInd];
			++currentPosInSegment[moduleInd];
			// A -> D
			if ((currentSegment[moduleInd] == 1) && (currentPosInSegment[moduleInd] > juce::roundToInt(*(voiceA + moduleInd))))
			{
				currentPosInSegment[moduleInd] = 0;
				currentSegment[moduleInd]++;
				ampIncrease[moduleInd] = -1.0 * (level[moduleInd] * (1.0 - (*(voiceS + moduleInd)))) / (*(voiceD + moduleInd));
			}
			// D -> S
			if ((currentSegment[moduleInd] == 2) && (currentPosInSegment[moduleInd] > juce::roundToInt(*(voiceD + moduleInd))))
			{
				currentPosInSegment[moduleInd] = 0;
				currentSegment[moduleInd]++;
				ampIncrease[moduleInd] = 0;

				tmpLevel[moduleInd] = level[moduleInd] * (*(voiceS + moduleInd));
			}
			// R -> stop
			if ((currentSegment[moduleInd] == 4) && (currentPosInSegment[moduleInd] > juce::roundToInt(*(voiceR + moduleInd))))
			{
				ampIncrease[moduleInd] = 0;
				//clearCurrentNote();
				angleDelta[moduleInd] = 0.0f;
				currentSegment[moduleInd] = 0;
				currentPosInSegment[moduleInd] = 0;
				return 0.0;
				//break;
			}
			return tmpLevel[moduleInd];
		}
		else return 0.0;
	}

	void renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
	{
		if (currentSegment > 0)
		{
			while (--numSamples >= 0)
			{
				//tmpLevel += ampIncrease;
				//auto currentSample = getNextSample() * tmpLevel;
				auto currentSample = getNextSample();
				for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
					outputBuffer.addSample(i, startSample, currentSample);
				++startSample;

				if ((currentSegment[*longestModule] == 4) && (currentPosInSegment[*longestModule] > juce::roundToInt(*(voiceR + *longestModule))))
				{
					clearCurrentNote();
					break;
				}
				//++currentPosInSegment;
				//// A -> D
				//if ((currentSegment == 1) && (currentPosInSegment > juce::roundToInt(*voiceA)))
				//{
				//	currentPosInSegment = 0;
				//	currentSegment++;
				//	ampIncrease = -1.0 * (level * (1.0 - (*voiceS))) / (*voiceD);
				//}
				//// D -> S
				//if ((currentSegment == 2) && (currentPosInSegment > juce::roundToInt(*voiceD)))
				//{
				//	currentPosInSegment = 0;
				//	currentSegment++;
				//	ampIncrease = 0;

				//	tmpLevel = level * (*voiceS);
				//}
				//// R -> stop
				//if ((currentSegment == 4) && (currentPosInSegment > juce::roundToInt(*voiceR)))
				//{
				//	ampIncrease = 0;

				//	clearCurrentNote();
				//	angleDelta = 0.0f;
				//	break;
				//}
			}
		}
		//if (angleDelta[0] != 0.0)
		//{
		//	if (tailOff > 0.0)
		//	{
		//		while (--numSamples >= 0)
		//		{
		//			auto currentSample = (float)getNextSample() * globalLevel * tailOff;
		//			for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
		//				outputBuffer.addSample(i, startSample, currentSample);
		//			//currentAngle += angleDelta;
		//			++startSample;
		//			tailOff *= 0.99;
		//			if (tailOff <= 0.005)
		//			{
		//				clearCurrentNote();
		//				for (int i = 0; i < 8; i++)
		//					angleDelta[i] = 0.0;
		//				break;
		//			}
		//		}
		//	}
		//	else
		//	{
		//		while (--numSamples >= 0)
		//		{
		//			auto currentSample = (float)(getNextSample() * globalLevel);
		//			for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
		//				outputBuffer.addSample(i, startSample, currentSample);
		//			//currentAngle += angleDelta;
		//			++startSample;
		//		}
		//	}
		//}
	}
private:
	double currentAngle[8], angleDelta[8], level[8], globalLevel = 0.0;
	//const juce::AudioSampleBuffer& wavetable;
	//float currentIndex = 0.0f, tableDelta = 0.0f, level = 0.0f, globalLevel = 0.0f;
	//juce::AudioSource* inputSource;
	//juce::ResamplingAudioSource(inputSource, false) pitchShifter;
	float pitchWheelCoef = 1.0;

	//double *voiceA[8], *voiceD[8], *voiceS[8], *voiceR[8];
	//double *fmI[8], *fmfc[8];
	double* voiceA, * voiceD, * voiceS, * voiceR;
	double* fmI, * fmfc;
	int currentPosInSegment[8];
	int currentSegment[8];
	int prevSegment0;
	float ampIncrease[8], tmpLevel[8];
	double mValues[8];
	int *longestModule;
	double tailOff = 0.0;

	//enum wave { sine, square, triangle, saw };
	int *currentWave;
};
//==============================================================================
class SynthAudioSource : public juce::AudioSource
{
public:
	SynthAudioSource(juce::MidiKeyboardState& keyState,
		double *ptrA, double *ptrD, double *ptrS, double *ptrR, double *ptrI, double *ptrfmfc, int *ptrCurrentWave, int* lm) : keyboardState(keyState)
	{
		for (auto i = 0; i < numVoices; ++i)
			synth.addVoice(new SineWaveVoice(/*signalTable, */numVoices, ptrA, ptrD, ptrS, ptrR, ptrI, ptrfmfc, ptrCurrentWave, lm));
		synth.addSound(new SineWaveSound());
	}

	void setUsingSineWaveSound()
	{
		synth.clearSounds();
	}
	void releaseResources() override {}
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override
	{
		bufferToFill.clearActiveBufferRegion();
		juce::MidiBuffer incomingMidi;
		midiCollector.removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples);
		keyboardState.processNextMidiBuffer(incomingMidi, bufferToFill.startSample, bufferToFill.numSamples, true);
		synth.renderNextBlock(*bufferToFill.buffer, incomingMidi, bufferToFill.startSample, bufferToFill.numSamples);
	}
	juce::MidiMessageCollector* getMidiCollector()
	{
		return &midiCollector;
	}
	//void createWavetable()
	//{
	//	signalTable.setSize(1, tableSize);
	//	auto* samples = signalTable.getWritePointer(0);
	//	auto currentAngle = 0.0;
	//	auto angleDelta = juce::MathConstants<double>::twoPi / (double)(tableSize - 1);
	//	for (auto i = 0; i < tableSize; ++i)
	//	{
	//		//auto sample = std::sin(currentAngle);
	//		auto sample = std::sin(currentAngle) >= 0 ? 1 : -1;
	//		//if (sample > 0) sample = 1;
	//		//else sample = -1;
	//		samples[i] = (float)sample;
	//		currentAngle += angleDelta;
	//	}
	//}
	void prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate) override
	{
		synth.setCurrentPlaybackSampleRate(sampleRate);
		//createWavetable(); // ***
	}


	juce::MidiMessageCollector midiCollector;
	//juce::ResamplingAudioSource pitchShifter();
private:
	juce::MidiKeyboardState& keyboardState;
	juce::Synthesiser synth;
	//int tableSize = 128;
	//int numVoices = 4;
	int numVoices = 20;
	//juce::AudioSampleBuffer signalTable;
};