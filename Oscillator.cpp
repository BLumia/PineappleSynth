#include "Oscillator.h"

double Oscillator::mSampleRate = 44100.0;

//Setters
void Oscillator::setMode(OscillatorMode mode) {
	mOscillatorMode = mode;
}

void Oscillator::setFrequency(double frequency) {
	mFrequency = frequency;
	updateIncrement();
}

void Oscillator::setSampleRate(double sampleRate) {
	mSampleRate = sampleRate;
	updateIncrement();
}

void Oscillator::updateIncrement() {
	mPhaseIncrement = mFrequency * 2 * mPI / mSampleRate;
}

double Oscillator::nextSample() {
	double value = 0.0;

	const float segLength = twoPI / mSegCount; 
	float calcPhase = isBitCrusherOn ? int(mPhase / segLength) * segLength : mPhase;

	switch (mOscillatorMode) {
	case OSCILLATOR_MODE_SINE:
		value = sin(calcPhase);
		break;
	case OSCILLATOR_MODE_SAW:
		value = 1.0 - (2.0 * calcPhase / twoPI);
		break;
	case OSCILLATOR_MODE_SQUARE:
		if (calcPhase <= mPI) {
			value = 1.0;
		} else {
			value = -1.0;
		}
		break;
	case OSCILLATOR_MODE_TRIANGLE:
		value = -1.0 + (2.0 * calcPhase / twoPI);
		value = 2.0 * (fabs(value) - 0.5);
		break;
	case OSCILLATOR_MODE_NOISE:
		// TODO: if using Bit Crusher, generate noise only at new seg.
		value = ((double)(std::rand()) / RAND_MAX) * 2.0 - 1.0;
		break;
	}
	mPhase += mPhaseIncrement;
	while (mPhase >= twoPI) {
		mPhase -= twoPI;
	}
	return value;
}