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

	switch (mOscillatorMode) {
	case OSCILLATOR_MODE_SINE:
		value = sin(mPhase);
		break;
	case OSCILLATOR_MODE_SAW:
		value = 1.0 - (2.0 * mPhase / twoPI);
		break;
	case OSCILLATOR_MODE_SQUARE:
		if (mPhase <= mPI) {
			value = 1.0;
		} else {
			value = -1.0;
		}
		break;
	case OSCILLATOR_MODE_TRIANGLE:
		value = -1.0 + (2.0 * mPhase / twoPI);
		value = 2.0 * (fabs(value) - 0.5);
		break;
	case OSCILLATOR_MODE_NOISE:
		value = ((double)(std::rand()) / RAND_MAX) * 2.0 - 1.0;
		break;
	}
	mPhase += mPhaseIncrement;
	while (mPhase >= twoPI) {
		mPhase -= twoPI;
	}
	return value;
}