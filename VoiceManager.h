#pragma once

#include "Voice.h"

class VoiceManager {
private:
	static const int NumberOfVoices = 64;
	Voice voices[NumberOfVoices];
	Voice* findFreeVoice();
public:
	void onNoteOn(int noteNumber, int velocity);
	void onNoteOff(int noteNumber, int velocity);
	double nextSample();
	void setSampleRate(double sampleRate) {
		EnvelopeGenerator::setSampleRate(sampleRate);
		for (int i = 0; i < NumberOfVoices; i++) {
			Voice& voice = voices[i];
			voice.mOscillator1.setSampleRate(sampleRate);
			voice.mOscillator2.setSampleRate(sampleRate);
		}
	}
	void setOscillatorMixForEachVoice(double mix);
	void setOscillatorModeForEachVoice(int oscID, Oscillator::OscillatorMode mode);
	void setFilterModeForEachVoice(Filter::FilterMode mode);
	void setFilterCutoffForEachVoice(double newCutoff);
	void setFilterResonanceForEachVoice(double newResonance);
	void setAmpEnvStageValueForEachVoice(EnvelopeGenerator::EnvelopeStage stage, double value);
	void setFilterEnvStageValueForEachVoice(EnvelopeGenerator::EnvelopeStage stage, double value);
	void setFilterAmountForEachVoice(double amount);
	void setAmpAmountForEachVoice(double amount);
	void setSemiOffsetForEachVoice(int oscID, int semi);
	void setCentOffsetForEachVoice(int oscID, int cent);
};

