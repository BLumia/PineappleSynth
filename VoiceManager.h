#pragma once

#include "Voice.h"

class VoiceManager {
private:
	static const int NumberOfVoices = 64;
	Voice voices[NumberOfVoices];
	Voice* findFreeVoice();
public:
	void onNoteOn(int noteNumber, int velocity);
	void onNoteOff(int noteNumber, int velocity);
	double nextSample();
	void setSampleRate(double sampleRate) {
		EnvelopeGenerator::setSampleRate(sampleRate);
		for (int i = 0; i < NumberOfVoices; i++) {
			Voice& voice = voices[i];
			voice.mOscillator.setSampleRate(sampleRate);
		}
	}
};

