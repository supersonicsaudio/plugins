/*
  ==============================================================================

    PluginProcessor.h
    Created: 30 Aug 2024 5:43:03pm
    Author:  Supersonics

  ==============================================================================
*/

#pragma once

#ifndef _EFFECT_DYNAMICS_SHAPER_H_
#define _EFFECT_DYNAMICS_SHAPER_H_

#include <JuceHeader.h>
#include "EnvelopeDetector.h"
#include "ADSR.h"

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

class AudioEffectDynamicsShaper 
{
public:
	AudioEffectDynamicsShaper();
	~AudioEffectDynamicsShaper() {};
	//virtual void update();
	void update(juce::AudioBuffer<float>& buffer, int totalNumInputChannels, int totalNumOutputChannels);
	bool begin(void);
	
	/**
	* @brief controls the enevelope's detector attack time
	*
	* @param attack time in mS, range 0.0f to 1.0f
	*/
	void setEnvAttack(float attack)
	{
		attack = map(attack, 0.0f, 1.0f, 0.0f, 1.0f);
	}

	/**
	* @brief controls the enevelope's detector decay time
	*
	* @param decay time in mS, range 0.0f to 1.0f
	*/
	void setEnvDecay(float decay)
	{
		decay = map(decay, 0.0f, 1.0f, 0.0f, 1.0f);
	}

	/**
	* @brief controls the enevelope's detector sensitivity
	*
	* @param sensitivity in dB, range 0.0f to 1.0f
	*/
	void setSensitivity(float treshold)
	{
		treshold = map(treshold, 0.0f, 1.0f, 0.0f, 1.0f);
		TRHESHOLDset = treshold;
	}

	/**
	* @brief controls the Dynamics Shaper attack time
	*
	* @param attack range 0.0f to 1.0f
	*/
	void setAttack(float attack)
	{
		attack = map(attack, 0.0f, 1.0f, 0.01f, 30.0f); //from 10mS to 30s
		ATTACK_TIMEset = attack;
	}

	/**
	* @brief controls the Dynamics Shaper Sustain Level
	*
	* @param Sustain range 0.0f to 1.0f
	*/
	void setSustain(float sustain)
	{
		sustain = map(sustain, 0.0f, 1.0f, 0.0f, 1.0f);
		SUSTAIN_LEVELset = sustain;
	}

	/**
	* @brief controls the Dynamics Shaper Decay Time
	*
	* @param Decay range 0.0f to 1.0f
	*/
	void setDecay(float decay)
	{
		decay = map(decay, 0.0f, 1.0f, 0.01f, 10.0f); //from 10mS to 10s
		DECAY_TIMEset = decay;
	}

	/**
	* @brief controls the Dynamics Shaper Release Time
	*
	* @param Release range 0.0f to 1.0f
	*/
	void setRelease(float release)
	{
		release = map(release, 0.0f, 1.0f, 0.01f, 10.0f); //from 10mS to 10s
		RELEASE_TIMEset = release;
	}
	float map(float x, float in_min, float in_max, float out_min, float out_max) {
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}

private:
	float ENVELOPE_ATTACK_TIME = 0.0f;
	float ENVELOPE_DECAY_TIME = 0.05f;
	
	// create envelope Detectors
	EnvelopeDetector envelopeDetectorL = EnvelopeDetector(ENVELOPE_ATTACK_TIME, ENVELOPE_DECAY_TIME);
	EnvelopeDetector envelopeDetectorR = EnvelopeDetector(ENVELOPE_ATTACK_TIME, ENVELOPE_DECAY_TIME);

	// create ADSRs
	SS_ADSR ADSR_L;
	SS_ADSR ADSR_R;

	float TRHESHOLD = 1.0f;
	float TRHESHOLDset = 1.0f;
	float ATTACK_TIME = 1.0f;
	float ATTACK_TIMEset = 1.0f;
	float SUSTAIN_LEVEL = 1.0f;
	float SUSTAIN_LEVELset = 1.0f;
	float DECAY_TIME = 1.0f;
	float DECAY_TIMEset = 1.0f;
	float RELEASE_TIME = 1.0f;
	float RELEASE_TIMEset = 1.0f;
	
	float peakValueL = 0.0f;
	float peakValueR = 0.0f;

	float lastPeakValueL = 0.0f;
	float lastPeakValueR = 0.0f;

	bool initialised = false;
};

#endif