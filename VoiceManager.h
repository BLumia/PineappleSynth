#pragma once

#include <list>
#include "Voice.h"

class VoiceManager {
private:
	int NumberOfVoices = 64; // need a static const?
	std::list<Voice*> voices; // Max voice count: 64
	Voice* findFreeVoice();
	int pbendRange = 2;
	double
		oscmix=.0,
		fltcutoff=0.99,
		fltreso=0.01,
		fltamt=.0,
		ampamt=50.;
	Oscillator::OscillatorMode oscmod[2]={
		Oscillator::OscillatorMode::OSCILLATOR_MODE_SINE,
		Oscillator::OscillatorMode::OSCILLATOR_MODE_SINE
	};
	Filter::FilterMode fltmod=Filter::FilterMode::FILTER_MODE_LOWPASS;
	double ampenv[EnvelopeGenerator::EnvelopeStage::kNumEnvelopeStages]={
		.0,
		.01,
		.5,
		.1,
		1.
	},
	fltenv[EnvelopeGenerator::EnvelopeStage::kNumEnvelopeStages]={
		.0,
		.01,
		.5,
		.1,
		1.
	};
	int oscfine[2]={0,0},osccoarse[2]={0,0};
	bool bcrush=false,bpstart=false;
public:
	void onNoteOn(int channel, int noteNumber, int velocity);
	void onNoteOff(int channel, int noteNumber, int velocity);
	void onPbChanged();
	double nextSample();
	void setSampleRate(double sampleRate) {
		EnvelopeGenerator::setSampleRate(sampleRate);
		for (auto i = voices.begin(); i != voices.end(); i++) {
			Voice* voice = *i;
			voice->mOscillator1.setSampleRate(sampleRate);
			voice->mOscillator2.setSampleRate(sampleRate);
		}
	}
	void setPBRange(int pbr) { pbendRange=pbr; }
	int getPBRange() { return pbendRange; }
	void setNumberOfVoices(int num) { NumberOfVoices = num; };
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
	void setBitCrusherEnabledForEachVoice(bool enabled);
	void setPhaseStartForEachVoice(bool enabled);
};

