#pragma once

#include <math.h>
#include <random>
#include <ctime>

class Oscillator {
public:
	enum OscillatorMode {
		OSCILLATOR_MODE_SINE = 0,
		OSCILLATOR_MODE_SAW,
		OSCILLATOR_MODE_SQUARE,
		OSCILLATOR_MODE_TRIANGLE,
		OSCILLATOR_MODE_NOISE,
		kNumOscillatorModes
	};
private:
	OscillatorMode mOscillatorMode;
	bool isBitCrusherOn;
	bool mPhaseStart; 
	const double mPI;
	const double twoPI;
	int mSegCount;
	double mFrequency;
	double mPhase;
	static double mSampleRate;
	double mPhaseIncrement;
	void updateIncrement();
public:
	void setMode(OscillatorMode mode);
	void setFrequency(double frequency);
	void setSampleRate(double sampleRate);
	void setBitCrusher(bool on) { isBitCrusherOn = on; };
	void setPhaseStart(bool on) { mPhaseStart = on; };
	double nextSample();
	void reset() { if (mPhaseStart) mPhase = 0.0; } // Lets the waveform start from the beginning everytime a voice starts to play.
	Oscillator() :
		mOscillatorMode(OSCILLATOR_MODE_SINE),
		mSegCount(32), // Thanks UN1C0DE for this idea and impl.
		mPI(2 * acos(0.0)),
		twoPI(2 * mPI), 
		isBitCrusherOn(false),
		mPhaseStart(true),
		mFrequency(440.0),
		mPhase(0.0) {
		updateIncrement();
		std::srand(std::time(0)); // RNG may need a better effectice impl
	};
};