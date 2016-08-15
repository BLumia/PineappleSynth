#include "Synthesis.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "IKeyboardControl.h"
#include "ADSRVisualizationControl.h"
#include "resource.h"

const int kNumPrograms = 5;

enum EAdsr
{
	E_Att = 0,
	E_Dec,
	E_Sus,
	E_Rel,
};

enum EParams
{
	mWaveform = 0,
	mAttack,
	mDecay,
	mSustain,
	mRelease,
	mFilterMode,
	mFilterCutoff,
	mFilterResonance,
	mFilterAttack,
	mFilterDecay,
	mFilterSustain,
	mFilterRelease,
	mFilterEnvelopeAmount,
	kNumParams
};

enum ELayout
{
	kWidth = GUI_WIDTH,
	kHeight = GUI_HEIGHT,
	kKeybX = 12,
	kKeybY = 154
};

Synthesis::Synthesis(IPlugInstanceInfo instanceInfo)
	: IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo),
	lastVirtualKeyboardNoteNumber(virtualKeyboardMinimumNoteNumber - 1),
	filterEnvelopeAmount(0.0), lfoFilterModAmount(0.1) {

	TRACE;

	IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
	pGraphics->AttachBackground(BG_ID, BG_FN);

	IBitmap pianoKeyImage = pGraphics->LoadIBitmap(PIANO_KEY_ID, PIANO_KEY_FN);

	//                            C#     D#          F#      G#      A#
	int keyCoordinates[12] = { 0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55 };
	mVirtualKeyboard = new IKeyboardControl(this, kKeybX, kKeybY, virtualKeyboardMinimumNoteNumber, /* octaves: */ 10, &pianoKeyImage, &pianoKeyImage, keyCoordinates);

	pGraphics->AttachControl(mVirtualKeyboard);

	// Waveform switch
	GetParam(mWaveform)->InitEnum("Waveform", OSCILLATOR_MODE_SINE, kNumOscillatorModes);
	GetParam(mWaveform)->SetDisplayText(0, "Sine"); // Needed for VST3, thanks plunntic
	IBitmap waveformBitmap = pGraphics->LoadIBitmap(WAVEFORM_ID, WAVEFORM_FN, 4);
	pGraphics->AttachControl(new ISwitchControl(this, 42, 20, mWaveform, &waveformBitmap));

	// Knob bitmap for ADSR
	IBitmap greenKnobBitmap = pGraphics->LoadIBitmap(GREEN_KNOB_ID, GREEN_KNOB_FN, 31);
	IBitmap blueKnobBitmap = pGraphics->LoadIBitmap(BLUE_KNOB_ID, BLUE_KNOB_FN, 31);
	IBitmap blueKnobCenterBitmap = pGraphics->LoadIBitmap(BLUE_KNOB_CENTER_ID, BLUE_KNOB_CENTER_FN, 31);

	// Attack knob:
	ampAdsrKnobs[E_Att] = new IKnobMultiControl(this, 329, 10, mAttack, &greenKnobBitmap);
	GetParam(mAttack)->InitDouble("Attack", 0.01, 0.01, 10.0, 0.001);
	GetParam(mAttack)->SetShape(3);
	pGraphics->AttachControl(ampAdsrKnobs[E_Att]);
	// Decay knob:
	ampAdsrKnobs[E_Dec] = new IKnobMultiControl(this, 383, 10, mDecay, &greenKnobBitmap);
	GetParam(mDecay)->InitDouble("Decay", 0.5, 0.01, 15.0, 0.001);
	GetParam(mDecay)->SetShape(3);
	pGraphics->AttachControl(ampAdsrKnobs[E_Dec]);
	// Sustain knob:
	ampAdsrKnobs[E_Sus] = new IKnobMultiControl(this, 437, 10, mSustain, &greenKnobBitmap);
	GetParam(mSustain)->InitDouble("Sustain", 0.1, 0.001, 1.0, 0.001);
	GetParam(mSustain)->SetShape(2);
	pGraphics->AttachControl(ampAdsrKnobs[E_Sus]);
	// Release knob:
	ampAdsrKnobs[E_Rel] = new IKnobMultiControl(this, 491, 10, mRelease, &greenKnobBitmap);
	GetParam(mRelease)->InitDouble("Release", 1.0, 0.001, 15.0, 0.001);
	GetParam(mRelease)->SetShape(3);
	pGraphics->AttachControl(ampAdsrKnobs[E_Rel]);

	// ADSR Visualization
	ampAdsrVisualization = new ADSRVisualizationControl(this, IRECT(546, 15, 648, 63));
	pGraphics->AttachControl(ampAdsrVisualization);

	// Filter switch
	GetParam(mFilterMode)->InitEnum("Filter Mode", Filter::FILTER_MODE_LOWPASS, Filter::kNumFilterModes);
	GetParam(mFilterMode)->SetDisplayText(0, "LP"); // Needed for VST3, thanks plunntic
	IBitmap filtermodeBitmap = pGraphics->LoadIBitmap(FILTERMODE_ID, FILTERMODE_FN, 3);
	pGraphics->AttachControl(new ISwitchControl(this, 42, 95, mFilterMode, &filtermodeBitmap));

	// Filter options
	// Cutoff knob:
	GetParam(mFilterCutoff)->InitDouble("Cutoff", 0.99, 0.01, 0.99, 0.001);
	GetParam(mFilterCutoff)->SetShape(2);
	pGraphics->AttachControl(new IKnobMultiControl(this, 137, 82, mFilterCutoff, &blueKnobBitmap));
	// Resonance knob:
	GetParam(mFilterResonance)->InitDouble("Resonance", 0.01, 0.01, 1.0, 0.001);
	pGraphics->AttachControl(new IKnobMultiControl(this, 195, 82, mFilterResonance, &blueKnobBitmap));
	// Filter envelope amount knob:
	GetParam(mFilterEnvelopeAmount)->InitDouble("Filter Env Amount", 0.0, -1.0, 1.0, 0.001);
	pGraphics->AttachControl(new IKnobMultiControl(this, 255, 82, mFilterEnvelopeAmount, &blueKnobCenterBitmap));

	// Attack knob:
	filterAdsrKnobs[E_Att] = new IKnobMultiControl(this, 329, 82, mFilterAttack, &blueKnobBitmap);
	GetParam(mFilterAttack)->InitDouble("Filter Env Attack", 0.01, 0.01, 10.0, 0.001);
	GetParam(mFilterAttack)->SetShape(3);
	pGraphics->AttachControl(filterAdsrKnobs[E_Att]);
	// Decay knob:
	filterAdsrKnobs[E_Dec] = new IKnobMultiControl(this, 383, 82, mFilterDecay, &blueKnobBitmap);
	GetParam(mFilterDecay)->InitDouble("Filter Env Decay", 0.5, 0.01, 15.0, 0.001);
	GetParam(mFilterDecay)->SetShape(3);
	pGraphics->AttachControl(filterAdsrKnobs[E_Dec]);
	// Sustain knob:
	filterAdsrKnobs[E_Sus] = new IKnobMultiControl(this, 437, 82, mFilterSustain, &blueKnobBitmap);
	GetParam(mFilterSustain)->InitDouble("Filter Env Sustain", 0.1, 0.001, 1.0, 0.001);
	GetParam(mFilterSustain)->SetShape(2);
	pGraphics->AttachControl(filterAdsrKnobs[E_Sus]);
	// Release knob:
	filterAdsrKnobs[E_Rel] = new IKnobMultiControl(this, 491, 82, mFilterRelease, &blueKnobBitmap);
	GetParam(mFilterRelease)->InitDouble("Filter Env Release", 1.0, 0.001, 15.0, 0.001);
	GetParam(mFilterRelease)->SetShape(3);
	pGraphics->AttachControl(filterAdsrKnobs[E_Rel]);

	// Filter ADSR Visualization
	filterEnvAdsrVisualization = new ADSRVisualizationControl(this, IRECT(546, 87, 648, 135));
	pGraphics->AttachControl(filterEnvAdsrVisualization);

	AttachGraphics(pGraphics);

	CreatePresets();

	mMIDIReceiver.noteOn.Connect(this, &Synthesis::onNoteOn);
	mMIDIReceiver.noteOff.Connect(this, &Synthesis::onNoteOff);
	mEnvelopeGenerator.beganEnvelopeCycle.Connect(this, &Synthesis::onBeganEnvelopeCycle);
	mEnvelopeGenerator.finishedEnvelopeCycle.Connect(this, &Synthesis::onFinishedEnvelopeCycle);

	mLFO.setMode(OSCILLATOR_MODE_TRIANGLE);
	mLFO.setFrequency(6.0);
	mLFO.setMuted(false);
}

Synthesis::~Synthesis() {}

void Synthesis::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	// Mutex is already locked for us.

	double *leftOutput = outputs[0];
	double *rightOutput = outputs[1];

	processVirtualKeyboard();
	for (int i = 0; i < nFrames; ++i) {
		mMIDIReceiver.advance();
		int velocity = mMIDIReceiver.getLastVelocity();
		double lfoFilterModulation = mLFO.nextSample() * lfoFilterModAmount;
		mOscillator.setFrequency(mMIDIReceiver.getLastFrequency());
		mFilter.setCutoffMod((mFilterEnvelopeGenerator.nextSample() * filterEnvelopeAmount) + lfoFilterModulation);
		//leftOutput[i] = rightOutput[i] = mOscillator.nextSample() * mEnvelopeGenerator.nextSample() * velocity / 127.0;
		leftOutput[i] = rightOutput[i] = mFilter.process(mOscillator.nextSample() * mEnvelopeGenerator.nextSample() * velocity / 127.0);
	}

	mMIDIReceiver.Flush(nFrames);
}

void Synthesis::CreatePresets()
{
}

void Synthesis::Reset()
{
	TRACE;
	IMutexLock lock(this);
	mOscillator.setSampleRate(GetSampleRate());
	mEnvelopeGenerator.setSampleRate(GetSampleRate());
	mFilterEnvelopeGenerator.setSampleRate(GetSampleRate());
	mLFO.setSampleRate(GetSampleRate());
}

void Synthesis::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);
	switch (paramIdx) {
	case mWaveform:
		mOscillator.setMode(static_cast<OscillatorMode>(GetParam(mWaveform)->Int()));
		break;
	case mAttack:
	case mDecay:
	case mSustain:
	case mRelease:
		mEnvelopeGenerator.setStageValue(static_cast<EnvelopeGenerator::EnvelopeStage>(paramIdx), GetParam(paramIdx)->Value());
		ampAdsrVisualization->setADSR(ampAdsrKnobs[E_Att]->GetValue(), ampAdsrKnobs[E_Dec]->GetValue(),
			ampAdsrKnobs[E_Sus]->GetValue(), ampAdsrKnobs[E_Rel]->GetValue());
		//ampAdsrVisualization->setADSR(ampAdsrKnobs[E_Att]->GetValue, ampAdsrKnobs[E_Dec]->GetValue, ampAdsrKnobs[E_Sus]->GetValue, ampAdsrKnobs[E_Rel]->GetValue);
		break;
	case mFilterCutoff:
		mFilter.setCutoff(GetParam(paramIdx)->Value());
		break;
	case mFilterResonance:
		mFilter.setResonance(GetParam(paramIdx)->Value());
		break;
	case mFilterMode:
		mFilter.setFilterMode(static_cast<Filter::FilterMode>(GetParam(paramIdx)->Int()));
		break;
	case mFilterAttack:
		mFilterEnvelopeGenerator.setStageValue(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK, GetParam(paramIdx)->Value());
		filterEnvAdsrVisualization->setADSR(filterAdsrKnobs[E_Att]->GetValue(), filterAdsrKnobs[E_Dec]->GetValue(),
			filterAdsrKnobs[E_Sus]->GetValue(), filterAdsrKnobs[E_Rel]->GetValue());
		break;
	case mFilterDecay:
		mFilterEnvelopeGenerator.setStageValue(EnvelopeGenerator::ENVELOPE_STAGE_DECAY, GetParam(paramIdx)->Value());
		filterEnvAdsrVisualization->setADSR(filterAdsrKnobs[E_Att]->GetValue(), filterAdsrKnobs[E_Dec]->GetValue(),
			filterAdsrKnobs[E_Sus]->GetValue(), filterAdsrKnobs[E_Rel]->GetValue());
		break;
	case mFilterSustain:
		mFilterEnvelopeGenerator.setStageValue(EnvelopeGenerator::ENVELOPE_STAGE_SUSTAIN, GetParam(paramIdx)->Value());
		filterEnvAdsrVisualization->setADSR(filterAdsrKnobs[E_Att]->GetValue(), filterAdsrKnobs[E_Dec]->GetValue(),
			filterAdsrKnobs[E_Sus]->GetValue(), filterAdsrKnobs[E_Rel]->GetValue());
		break;
	case mFilterRelease:
		mFilterEnvelopeGenerator.setStageValue(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE, GetParam(paramIdx)->Value());
		filterEnvAdsrVisualization->setADSR(filterAdsrKnobs[E_Att]->GetValue(), filterAdsrKnobs[E_Dec]->GetValue(),
			filterAdsrKnobs[E_Sus]->GetValue(), filterAdsrKnobs[E_Rel]->GetValue());
		break;
	case mFilterEnvelopeAmount:
		filterEnvelopeAmount = GetParam(paramIdx)->Value();
		break;
	}
}

void Synthesis::ProcessMidiMsg(IMidiMsg* pMsg) {
	mMIDIReceiver.onMessageReceived(pMsg);
	mVirtualKeyboard->SetDirty();
}

void Synthesis::processVirtualKeyboard() {
	IKeyboardControl* virtualKeyboard = (IKeyboardControl*)mVirtualKeyboard;
	int virtualKeyboardNoteNumber = virtualKeyboard->GetKey() + virtualKeyboardMinimumNoteNumber;

	if (lastVirtualKeyboardNoteNumber >= virtualKeyboardMinimumNoteNumber && virtualKeyboardNoteNumber != lastVirtualKeyboardNoteNumber) {
		// The note number has changed from a valid key to something else (valid key or nothing). Release the valid key:
		IMidiMsg midiMessage;
		midiMessage.MakeNoteOffMsg(lastVirtualKeyboardNoteNumber, 0);
		mMIDIReceiver.onMessageReceived(&midiMessage);
	}

	if (virtualKeyboardNoteNumber >= virtualKeyboardMinimumNoteNumber && virtualKeyboardNoteNumber != lastVirtualKeyboardNoteNumber) {
		// A valid key is pressed that wasn't pressed the previous call. Send a "note on" message to the MIDI receiver:
		IMidiMsg midiMessage;
		midiMessage.MakeNoteOnMsg(virtualKeyboardNoteNumber, virtualKeyboard->GetVelocity(), 0);
		mMIDIReceiver.onMessageReceived(&midiMessage);
	}

	lastVirtualKeyboardNoteNumber = virtualKeyboardNoteNumber;
}