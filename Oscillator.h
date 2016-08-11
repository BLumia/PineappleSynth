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
	bool isMuted;
	double mFrequency;
	double mPhase;
	double mSampleRate;
	double mPhaseIncrement;
	void updateIncrement();
public:
	void setMode(OscillatorMode mode);
	void setFrequency(double frequency);
	void setSampleRate(double sampleRate);
	//void generate(double* buffer, int nFrames);
	inline void setMuted(bool muted) { isMuted = muted; }
	double nextSample();
	Oscillator() :
		mOscillatorMode(OSCILLATOR_MODE_SINE),
		mPI(2 * acos(0.0)),
		twoPI(2 * mPI), // This line is new
		isMuted(true),  // And this line
		mFrequency(440.0),
		mPhase(0.0),
		mSampleRate(44100.0) {
		updateIncrement();
	};
};