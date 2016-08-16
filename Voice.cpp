#include "Voice.h"

double Voice::nextSample() {
	if (!isActive) return 0.0;

	double oscillatorOutput = mOscillator.nextSample();

	double ampEnvelopeValue = mAmpEnvelope.nextSample();
	double filterEnvelopeValue = mFilterEnvelope.nextSample();

	mFilter.setCutoffMod(filterEnvelopeValue * mFilterEnvelopeAmount); // ignore LFO now.

	return mFilter.process(oscillatorOutput * ampEnvelopeValue * mVelocity / 127.0);
}

void Voice::setFree() {
	isActive = false;
}

void Voice::reset() {
	mNoteNumber = -1;
	mVelocity = 0;
	mOscillator.reset();
	mAmpEnvelope.reset();
	mFilterEnvelope.reset();
	mFilter.reset();
}