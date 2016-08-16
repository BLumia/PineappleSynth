#include "VoiceManager.h"

Voice* VoiceManager::findFreeVoice() {
	Voice* freeVoice = NULL;
	for (int i = 0; i < NumberOfVoices; i++) {
		if (!voices[i].isActive) {
			freeVoice = &(voices[i]);
			break;
		}
	}
	return freeVoice;
}

void VoiceManager::onNoteOn(int noteNumber, int velocity) {
	Voice* voice = findFreeVoice();
	if (!voice) {
		return;
	}
	voice->reset();
	voice->setNoteNumber(noteNumber);
	voice->mVelocity = velocity;
	voice->isActive = true;
	voice->mAmpEnvelope.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK);
	voice->mFilterEnvelope.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK);
}

void VoiceManager::onNoteOff(int noteNumber, int velocity) {
	// Find the voice(s) with the given noteNumber:
	for (int i = 0; i < NumberOfVoices; i++) {
		Voice& voice = voices[i];
		if (voice.isActive && voice.mNoteNumber == noteNumber) {
			voice.mAmpEnvelope.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
			voice.mFilterEnvelope.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
		}
	}
}

double VoiceManager::nextSample() {
	double output = 0.0;
	for (int i = 0; i < NumberOfVoices; i++) {
		Voice& voice = voices[i];
		output += voice.nextSample();
	}
	return output;
}