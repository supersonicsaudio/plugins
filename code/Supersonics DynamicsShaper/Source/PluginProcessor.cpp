/*
  ==============================================================================

    PluginProcessor.cpp
    Created: 30 Aug 2024 6:00:11pm
    Author:  Supersonics

  ==============================================================================
*/

#include "PluginProcessor.h"

AudioEffectDynamicsShaper::AudioEffectDynamicsShaper() { begin(); }

bool AudioEffectDynamicsShaper::begin()
{

	envelopeDetectorL.setAttack_msec(0.0f);
	envelopeDetectorR.setAttack_msec(0.0f);
	envelopeDetectorL.setRelease_msec(0.05f);
	envelopeDetectorR.setRelease_msec(0.05f);	
	
	initialised = true;
	return true;
}

void AudioEffectDynamicsShaper::update(juce::AudioBuffer<float>& buffer, int totalNumInputChannels, int totalNumOutputChannels)
{
	auto* channelDataL = buffer.getWritePointer(0);
	auto* channelDataR = buffer.getWritePointer(1);
	
	lastPeakValueL = peakValueL;
	lastPeakValueR = peakValueR;

	for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
	{

		lastPeakValueL = envelopeDetectorL.smooth_env(channelDataL[sample], lastPeakValueL);
		lastPeakValueR = envelopeDetectorR.smooth_env(channelDataR[sample], lastPeakValueR);

		if (lastPeakValueL > TRHESHOLD)
		{
			ADSR_L.gate(true);
		}
		else
		{
			ADSR_L.gate(false);
		}

		if (lastPeakValueR > TRHESHOLD)
		{
			ADSR_R.gate(true);
		}
		else 
		{
			ADSR_R.gate(false);
		}

		channelDataL[sample] = channelDataL[sample] * ADSR_L.process();
		channelDataR[sample] = channelDataR[sample] * ADSR_R.process();

	}
	peakValueL = lastPeakValueL;
	peakValueR = lastPeakValueR;

	if (ATTACK_TIME != ATTACK_TIMEset)
	{
		ADSR_R.setAttackRate(ATTACK_TIMEset * 44100);
		ADSR_L.setAttackRate(ATTACK_TIMEset * 44100);
		ATTACK_TIME = ATTACK_TIMEset;
	}
	if (SUSTAIN_LEVEL != SUSTAIN_LEVELset)
	{
		ADSR_R.setSustainLevel(SUSTAIN_LEVELset * 44100);
		ADSR_L.setSustainLevel(SUSTAIN_LEVELset * 44100);
		SUSTAIN_LEVEL = SUSTAIN_LEVELset;
	}
	if (DECAY_TIME != DECAY_TIMEset)
	{
		ADSR_R.setDecayRate(DECAY_TIMEset * 44100);
		ADSR_L.setDecayRate(DECAY_TIMEset * 44100);
		DECAY_TIME = DECAY_TIMEset;
	}
	if (RELEASE_TIME != RELEASE_TIMEset)
	{
		ADSR_R.setReleaseRate(RELEASE_TIMEset * 44100);
		ADSR_L.setReleaseRate(RELEASE_TIMEset * 44100);
		RELEASE_TIME = RELEASE_TIMEset;
	}
	if (TRHESHOLD != TRHESHOLDset)
	{
		TRHESHOLD = TRHESHOLDset;
	}
}