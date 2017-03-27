#include "VoiceManager.h"
#include "MIDIReceiver.h"

Voice* VoiceManager::findFreeVoice() {
	Voice* freeVoice = NULL;
	if (voices.size()<NumberOfVoices)
	{
		freeVoice=new Voice();
		voices.push_back(freeVoice);
	}
	return freeVoice;
}

void VoiceManager::onNoteOn(int channel, int noteNumber, int velocity) {
	Voice* voice = findFreeVoice();
	if (!voice) {
		return;
	}
	voice->reset();
	voice->mOscillatorMix=oscmix;
	voice->mFilter.setCutoff(fltcutoff);
	voice->mFilter.setResonance(fltreso);
	voice->mFilterEnvelopeAmount=fltamt;
	voice->mAmpEnvelopeAmount=ampamt;
	voice->mOscillator1.setMode(oscmod[0]);
	voice->mOscillator2.setMode(oscmod[1]);
	voice->mFilter.setFilterMode(fltmod);
	for(int i=0;i<EnvelopeGenerator::EnvelopeStage::kNumEnvelopeStages;++i)
	{
		voice->mAmpEnvelope.setStageValue((EnvelopeGenerator::EnvelopeStage)i,ampenv[i]);
		voice->mFilterEnvelope.setStageValue((EnvelopeGenerator::EnvelopeStage)i,fltenv[i]);
	}
	voice->mCentOffset1=oscfine[0];
	voice->mCentOffset2=oscfine[1];
	voice->mSemiOffset1=osccoarse[0];
	voice->mSemiOffset2=osccoarse[1];
	voice->mOscillator1.setBitCrusher(bcrush);
	voice->mOscillator2.setBitCrusher(bcrush);
	voice->setPhaseStart(bpstart);
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
	for (auto i = voices.begin(); i != voices.end(); i++) {
		Voice* voice = *i;
		if (voice->isActive && voice->mNoteNumber == noteNumber) {
			voice->mAmpEnvelope.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
			voice->mFilterEnvelope.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
		}
	}
}

void VoiceManager::onPbChanged()
{
	for(auto i = voices.begin(); i != voices.end(); i++) {
		if((*i)->isActive)
		(*i)->setPBAmount(pbendRange * MIDIReceiver::getReference()->getPitchBendAmount((*i)->channel()));
	}
}

double VoiceManager::nextSample() {
	double output = 0.0;
	for(auto i = voices.begin(); i != voices.end(); ) {
		Voice* voice = *i;
		output += voice->nextSample();
		if(!voice->isActive) {
			delete *i;auto t=i;++t;
			voices.erase(i);i=t;
		} else ++i;
	}
	return output;
}

// Setting attributes
void VoiceManager::setFilterCutoffForEachVoice(double newCutoff) {
	fltcutoff=newCutoff;
	for(auto i = voices.begin(); i != voices.end(); i++) {
		(*i)->mFilter.setCutoff(newCutoff);
	}
}

void VoiceManager::setFilterResonanceForEachVoice(double newResonance) {
	fltreso=newResonance;
	for(auto i = voices.begin(); i != voices.end(); i++) {
		(*i)->mFilter.setResonance(newResonance);
	}
}

void VoiceManager::setOscillatorModeForEachVoice(int oscID, Oscillator::OscillatorMode mode) {
	oscmod[oscID-1]=mode;
	if (oscID == 1) {
		for(auto i = voices.begin(); i != voices.end(); i++) {
			(*i)->mOscillator1.setMode(mode);
		}
	} else { // osc2
		for(auto i = voices.begin(); i != voices.end(); i++) {
			(*i)->mOscillator2.setMode(mode);
		}
	}
	
}

void VoiceManager::setOscillatorMixForEachVoice(double mix) {
	oscmix=mix;
	for(auto i = voices.begin(); i != voices.end(); i++) {
		(*i)->setOscillatorMix(mix);
	}
}

void VoiceManager::setFilterModeForEachVoice(Filter::FilterMode mode) {
	fltmod=mode;
	for(auto i = voices.begin(); i != voices.end(); i++) {
		(*i)->mFilter.setFilterMode(mode);
	}
}

void VoiceManager::setAmpEnvStageValueForEachVoice(EnvelopeGenerator::EnvelopeStage stage, double value) {
	ampenv[stage]=value;
	for(auto i = voices.begin(); i != voices.end(); i++) {
		(*i)->mAmpEnvelope.setStageValue(stage, value);
	}
}

void VoiceManager::setFilterEnvStageValueForEachVoice(EnvelopeGenerator::EnvelopeStage stage, double value) {
	fltenv[stage]=value;
	for(auto i = voices.begin(); i != voices.end(); i++) {
		(*i)->mFilterEnvelope.setStageValue(stage, value);
	}
}

void VoiceManager::setFilterAmountForEachVoice(double amount) {
	fltamt=amount;
	for(auto i = voices.begin(); i != voices.end(); i++) {
		(*i)->mFilterEnvelopeAmount = amount;
	}
}

void VoiceManager::setAmpAmountForEachVoice(double amount) {
	ampamt=amount;
	for(auto i = voices.begin(); i != voices.end(); i++) {
		(*i)->mAmpEnvelopeAmount = amount;
	}
}

void VoiceManager::setSemiOffsetForEachVoice(int oscID, int semi) {
	osccoarse[oscID-1]=semi;
	if (oscID == 1) {
		for(auto i = voices.begin(); i != voices.end(); i++) {
			(*i)->mSemiOffset1 = semi;
		}
	} else {
		for(auto i = voices.begin(); i != voices.end(); i++) {
			(*i)->mSemiOffset2 = semi;
		}
	}
}

void VoiceManager::setCentOffsetForEachVoice(int oscID, int cent) {
	oscfine[oscID-1]=cent;
	if (oscID == 1) {
		for(auto i = voices.begin(); i != voices.end(); i++) {
			(*i)->mCentOffset1 = cent;
		}
	} else {
		for(auto i = voices.begin(); i != voices.end(); i++) {
			(*i)->mCentOffset2 = cent;
		}
	}
}

void VoiceManager::setBitCrusherEnabledForEachVoice(bool enabled) {
	bcrush=enabled;
	for(auto i = voices.begin(); i != voices.end(); i++) {
		(*i)->setOscillatorBitCrusher(enabled);
	}
}

void VoiceManager::setPhaseStartForEachVoice(bool enabled) {
	bpstart=enabled;
	for(auto i = voices.begin(); i != voices.end(); i++) {
		(*i)->setPhaseStart(enabled);
	}
}