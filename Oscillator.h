#pragma once

#include <math.h>

enum OscillatorMode {
	OSCILLATOR_MODE_SINE = 0,
	OSCILLATOR_MODE_SAW,
	OSCILLATOR_MODE_SQUARE,
	OSCILLATOR_MODE_TRIANGLE,
	kNumOscillatorModes
};

class Oscillator {
private:
	OscillatorMode mOscillatorMode;
	const double mPI;
	const double twoPI;
	double mFrequency;
	double mPhase;
	static double mSampleRate;
	double mPhaseIncrement;
	void updateIncrement();
public:
	void setMode(OscillatorMode mode);
	void setFrequency(double frequency);
	void setSampleRate(double sampleRate);
	//void generate(double* buffer, int nFrames);
	double nextSample();
	void reset() { mPhase = 0.0; } // Lets the waveform start from the beginning everytime a voice starts to play.
	Oscillator() :
		mOscillatorMode(OSCILLATOR_MODE_SINE),
		mPI(2 * acos(0.0)),
		twoPI(2 * mPI), // This line is new
		mFrequency(440.0),
		mPhase(0.0) {
		updateIncrement();
	};
};