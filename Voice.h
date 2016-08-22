#pragma once
#pragma once

#include "Oscillator.h"
#include "EnvelopeGenerator.h"
#include "Filter.h"

class Voice {
private:
	// Voice Attribute
	bool isActive; // osc isMuted
	double mOscillatorMix;
	int mNoteNumber;
	int mVelocity;
	// Component
	Oscillator mOscillator1;
	Oscillator mOscillator2;
	Filter mFilter;
	EnvelopeGenerator mAmpEnvelope;
	EnvelopeGenerator mFilterEnvelope;
	// Component Attribute
	double mFilterEnvelopeAmount;
	double mAmpEnvelopeAmount;
	// Freq Adjust
	int mSemiOffset1; //Coarse knob
	int mSemiOffset2;
	int mCentOffset1; //Fine knob
	int mCentOffset2;
public:
	friend class VoiceManager;
	Voice() :
		mNoteNumber(-1),
		mVelocity(0),
		mFilterEnvelopeAmount(0.0),
		mAmpEnvelopeAmount(1.0),
		mSemiOffset1(0),
		mSemiOffset2(0),
		mCentOffset1(0),
		mCentOffset2(0),
		mOscillatorMix(0.0),
		isActive(false) {
		// Set myself free everytime my volume envelope has fully faded out of RELEASE stage:
		mAmpEnvelope.finishedEnvelopeCycle.Connect(this, &Voice::setFree);
	}
	// Setters
	inline void setFilterEnvelopeAmount(double amount) { mFilterEnvelopeAmount = amount; }
	inline void setAmpEnvelopeAmount(double amount) { mAmpEnvelopeAmount = amount; }
	inline void setSemiOffset1(int semi) { mSemiOffset1 = semi; }
	inline void setSemiOffset2(int semi) { mSemiOffset2 = semi; }
	inline void setCentOffset1(int cent) { mCentOffset1 = cent; }
	inline void setCentOffset2(int cent) { mCentOffset2 = cent; }
	inline void setOscillatorMix(double mix) { mOscillatorMix = mix; }
	inline void setNoteNumber(int noteNumber) {
		mNoteNumber = noteNumber;
		double frequency1 = 440.0 * pow(2.0, (mNoteNumber + mSemiOffset1 + (mCentOffset1 * 0.01) - 69.0) / 12.0);
		double frequency2 = 440.0 * pow(2.0, (mNoteNumber + mSemiOffset2 + (mCentOffset2 * 0.01) - 69.0) / 12.0);
		mOscillator1.setFrequency(frequency1);
		mOscillator2.setFrequency(frequency2);
	}
	// Method
	double nextSample();
	void setFree();
	void reset();
};