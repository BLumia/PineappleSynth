#include "VoiceManager.h"
#include "MIDIReceiver.h"

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

void VoiceManager::onNoteOn(int channel, int noteNumber, int velocity) {
	Voice* voice = findFreeVoice();
	if (!voice) {
		return;
	}
	voice->reset();
	voice->setChannel(channel);
	voice->setPBAmount(pbendRange * MIDIReceiver::getReference()->getPitchBendAmount(channel));
	voice->setNoteNumber(noteNumber);
	voice->mVelocity = velocity;
	voice->isActive = true;
	voice->mAmpEnvelope.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK);
	voice->mFilterEnvelope.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK);
}

void VoiceManager::onNoteOff(int channel, int noteNumber, int velocity) {
	// Find the voice(s) with the given noteNumber:
	for (int i = 0; i < NumberOfVoices; i++) {
		Voice& voice = voices[i];
		if (voice.isActive && voice.mNoteNumber == noteNumber) {
			voice.mAmpEnvelope.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
			voice.mFilterEnvelope.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
		}
	}
}

void VoiceManager::onPbChanged()
{
	for(int i = 0; i < NumberOfVoices; i++) {
		if(voices[i].isActive)
		voices[i].setPBAmount(pbendRange * MIDIReceiver::getReference()->getPitchBendAmount(voices[i].channel()));
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

// Setting attributes
void VoiceManager::setFilterCutoffForEachVoice(double newCutoff) {
	for (int i = 0; i < NumberOfVoices; i++) {
		voices[i].mFilter.setCutoff(newCutoff);
	}
}

void VoiceManager::setFilterResonanceForEachVoice(double newResonance) {
	for (int i = 0; i < NumberOfVoices; i++) {
		voices[i].mFilter.setResonance(newResonance);
	}
}

void VoiceManager::setOscillatorModeForEachVoice(int oscID, Oscillator::OscillatorMode mode) {
	if (oscID == 1) {
		for (int i = 0; i < NumberOfVoices; i++) {
			voices[i].mOscillator1.setMode(mode);
		}
	} else { // osc2
		for (int i = 0; i < NumberOfVoices; i++) {
			voices[i].mOscillator2.setMode(mode);
		}
	}
	
}

void VoiceManager::setOscillatorMixForEachVoice(double mix) {
	for (int i = 0; i < NumberOfVoices; i++) {
		voices[i].setOscillatorMix(mix);
	}
}

void VoiceManager::setFilterModeForEachVoice(Filter::FilterMode mode) {
	for (int i = 0; i < NumberOfVoices; i++) {
		voices[i].mFilter.setFilterMode(mode);
	}
}

void VoiceManager::setAmpEnvStageValueForEachVoice(EnvelopeGenerator::EnvelopeStage stage, double value) {
	for (int i = 0; i < NumberOfVoices; i++) {
		voices[i].mAmpEnvelope.setStageValue(stage, value);
	}
}

void VoiceManager::setFilterEnvStageValueForEachVoice(EnvelopeGenerator::EnvelopeStage stage, double value) {
	for (int i = 0; i < NumberOfVoices; i++) {
		voices[i].mFilterEnvelope.setStageValue(stage, value);
	}
}

void VoiceManager::setFilterAmountForEachVoice(double amount) {
	for (int i = 0; i < NumberOfVoices; i++) {
		voices[i].mFilterEnvelopeAmount = amount;
	}
}

void VoiceManager::setAmpAmountForEachVoice(double amount) {
	for (int i = 0; i < NumberOfVoices; i++) {
		voices[i].mAmpEnvelopeAmount = amount;
	}
}

void VoiceManager::setSemiOffsetForEachVoice(int oscID, int semi) {
	if (oscID == 1) {
		for (int i = 0; i < NumberOfVoices; i++) {
			voices[i].mSemiOffset1 = semi;
		}
	} else {
		for (int i = 0; i < NumberOfVoices; i++) {
			voices[i].mSemiOffset2 = semi;
		}
	}
}

void VoiceManager::setCentOffsetForEachVoice(int oscID, int cent) {
	if (oscID == 1) {
		for (int i = 0; i < NumberOfVoices; i++) {
			voices[i].mCentOffset1 = cent;
		}
	} else {
		for (int i = 0; i < NumberOfVoices; i++) {
			voices[i].mCentOffset2 = cent;
		}
	}
}

void VoiceManager::setBitCrusherEnabledForEachVoice(bool enabled) {
	for (int i = 0; i < NumberOfVoices; i++) {
		voices[i].setOscillatorBitCrusher(enabled);
	}
}

void VoiceManager::setPhaseStartForEachVoice(bool enabled) {
	for (int i = 0; i < NumberOfVoices; i++) {
		voices[i].setPhaseStart(enabled);
	}
}