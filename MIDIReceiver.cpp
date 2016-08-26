#include "MIDIReceiver.h"

MIDIReceiver* MIDIReceiver::ref=NULL;

void MIDIReceiver::onMessageReceived(IMidiMsg* midiMessage) {
	IMidiMsg::EStatusMsg status = midiMessage->StatusMsg();
	// We're only interested in Note On/Off messages (not CC, pitch, etc.)
	if (status == IMidiMsg::kNoteOn || status == IMidiMsg::kNoteOff || status == IMidiMsg::kPitchWheel) {
		mMidiQueue.Add(midiMessage);
	}
}

//Be called on every sample while we¡¯re generating an audio buffer
void MIDIReceiver::advance() {
	while (!mMidiQueue.Empty()) {
		IMidiMsg* midiMessage = mMidiQueue.Peek();
		if (midiMessage->mOffset > mOffset) break;

		IMidiMsg::EStatusMsg status = midiMessage->StatusMsg();
		if (status == IMidiMsg::kPitchWheel)
		{
			pbend[midiMessage->Channel()] = midiMessage->PitchWheel();
			pbChanged();
			mMidiQueue.Remove();
			continue;
		}
		int channel = midiMessage->Channel();
		int noteNumber = midiMessage->NoteNumber();
		int velocity = midiMessage->Velocity();
		// There are only note on/off messages in the queue, see ::OnMessageReceived
		if (status == IMidiMsg::kNoteOn && velocity) {
			if (mKeyStatus[noteNumber] == false) {
				mKeyStatus[noteNumber] = true;
				mNumKeys += 1;
				noteOn(channel, noteNumber, velocity);
			}
		} else {
			if (mKeyStatus[noteNumber] == true) {
				mKeyStatus[noteNumber] = false;
				mNumKeys -= 1;
				noteOff(channel, noteNumber, velocity);
			}
		}
		mMidiQueue.Remove();
	}
	mOffset++;
}