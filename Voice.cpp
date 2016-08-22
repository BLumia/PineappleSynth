#include "Voice.h"

double Voice::nextSample() {
	if (!isActive) return 0.0;

	double oscillator1Output = mOscillator1.nextSample();
	double oscillator2Output = mOscillator2.nextSample();
	double oscillatorSum = ((1 - mOscillatorMix) * oscillator1Output) + (mOscillatorMix * oscillator2Output);

	double ampEnvelopeValue = mAmpEnvelope.nextSample();
	double filterEnvelopeValue = mFilterEnvelope.nextSample();

	oscillatorSum *= mAmpEnvelopeAmount * 0.01;

	mFilter.setCutoffMod(filterEnvelopeValue * mFilterEnvelopeAmount); // ignore LFO now.

	return mFilter.process(oscillatorSum * ampEnvelopeValue * mVelocity / 127.0);
}

void Voice::setFree() {
	isActive = false;
}

// Do reset after every noteoff
void Voice::reset() {
	mNoteNumber = -1;
	mVelocity = 0;
	//mOscillatorMix = 0;
	mOscillator1.reset();
	mOscillator2.reset();
	mAmpEnvelope.reset();
	mFilterEnvelope.reset();
	mFilter.reset();
}