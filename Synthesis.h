#ifndef __SYNTHESIS__
#define __SYNTHESIS__

#include "IPlug_include_in_plug_hdr.h"
#include "Oscillator.h"
#include "MIDIReceiver.h"
#include "EnvelopeGenerator.h"
#include "ADSRVisualizationControl.h"
#include "Filter.h"

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
	Filter mFilter;
	Oscillator mOscillator;
	MIDIReceiver mMIDIReceiver;
	IControl* mVirtualKeyboard;
	EnvelopeGenerator mEnvelopeGenerator;
	IKnobMultiControl* ampAdsrKnobs[4];
	ADSRVisualizationControl* ampAdsrVisualization;
	void processVirtualKeyboard();
	inline void onNoteOn(const int noteNumber, const int velocity) { mEnvelopeGenerator.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK); };
	inline void onNoteOff(const int noteNumber, const int velocity) { mEnvelopeGenerator.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE); };
	inline void onBeganEnvelopeCycle() { mOscillator.setMuted(false); }
	inline void onFinishedEnvelopeCycle() { mOscillator.setMuted(true); }
};

#endif
