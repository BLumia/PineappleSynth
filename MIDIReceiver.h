#pragma once

#include <cstring>
#include "IPlug_include_in_plug_hdr.h"
#include "IMidiQueue.h"
#include "GallantSignal.h"
using Gallant::Signal3;
using Gallant::Signal0;

class MIDIReceiver {
private:
	IMidiQueue mMidiQueue;
	static const int keyCount = 128;
	static MIDIReceiver* ref;
	int mNumKeys; // how many keys are being played at the moment (via midi)
	bool mKeyStatus[keyCount]; // array of on/off for each key (index is note number)
	double pbend[16];
	int mOffset;

public:
	MIDIReceiver() :
		mNumKeys(0),
		mOffset(0){
		ref=this;
		for (int i = 0; i < keyCount; i++) {
			mKeyStatus[i] = false;
		}
		memset(pbend,0,sizeof(pbend));
	};

	// Returns true if the key with a given index is currently pressed
	inline bool getKeyStatus(int keyIndex) const { return mKeyStatus[keyIndex]; }
	// Returns the number of keys currently pressed
	inline int getNumKeys() const { return mNumKeys; }
	inline double getPitchBendAmount(int ch) { return pbend[ch]; }
	void advance();
	void onMessageReceived(IMidiMsg* midiMessage);
	inline void Flush(int nFrames) { mMidiQueue.Flush(nFrames); mOffset = 0; }
	inline void Resize(int blockSize) { mMidiQueue.Resize(blockSize); }

	Signal3< int, int, int > noteOn;
	Signal3< int, int, int > noteOff;
	Signal0<void> pbChanged;

	static MIDIReceiver* getReference(){return ref;}
};