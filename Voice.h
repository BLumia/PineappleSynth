#pragma once
#pragma once

#include "Oscillator.h"
#include "EnvelopeGenerator.h"
#include "Filter.h"

class Voice {
private:
	// Voice Attribute
	bool isActive; // osc isMuted
	int mNoteNumber;
	int mVelocity;
	// Component
	Oscillator mOscillator;
	Filter mFilter;
	EnvelopeGenerator mAmpEnvelope;
	EnvelopeGenerator mFilterEnvelope;
	// Component Attribute
	double mFilterEnvelopeAmount;
public:
	friend class VoiceManager;
	Voice() :
		mNoteNumber(-1),
		mVelocity(0),
		mFilterEnvelopeAmount(0.0),
		isActive(false) {
		// Set myself free everytime my volume envelope has fully faded out of RELEASE stage:
		mAmpEnvelope.finishedEnvelopeCycle.Connect(this, &Voice::setFree);
	}
	// Setters
	inline void setFilterEnvelopeAmount(double amount) { mFilterEnvelopeAmount = amount; }
	inline void setNoteNumber(int noteNumber) {
		mNoteNumber = noteNumber;
		double frequency = 440.0 * pow(2.0, (mNoteNumber - 69.0) / 12.0);
		mOscillator.setFrequency(frequency);
	}
	// Method
	double nextSample();
	void setFree();
	void reset();
};