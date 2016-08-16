#ifndef __SYNTHESIS__
#define __SYNTHESIS__

#include "IPlug_include_in_plug_hdr.h"
#include "MIDIReceiver.h"
#include "ADSRVisualizationControl.h"
#include "VoiceManager.h"

class Synthesis : public IPlug
{
public:
	Synthesis(IPlugInstanceInfo instanceInfo);
	~Synthesis();

	void Reset();
	void OnParamChange(int paramIdx);
	void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);
	// to receive MIDI messages:
	void ProcessMidiMsg(IMidiMsg* pMsg);
	// Needed for the GUI keyboard:
	// Should return non-zero if one or more keys are playing.
	inline int GetNumKeys() const { return mMIDIReceiver.getNumKeys(); };
	// Should return true if the specified key is playing.
	inline bool GetKeyStatus(int key) const { return mMIDIReceiver.getKeyStatus(key); };
	static const int virtualKeyboardMinimumNoteNumber = 0;
	int lastVirtualKeyboardNoteNumber;

private:
	double mFrequency;
	void CreatePresets();
	VoiceManager voiceManager;
	MIDIReceiver mMIDIReceiver;
	IControl* mVirtualKeyboard;
	IKnobMultiControl* ampAdsrKnobs[4];
	IKnobMultiControl* filterAdsrKnobs[4];
	ADSRVisualizationControl* ampAdsrVisualization;
	ADSRVisualizationControl* filterEnvAdsrVisualization;
	void processVirtualKeyboard();
};

#endif
