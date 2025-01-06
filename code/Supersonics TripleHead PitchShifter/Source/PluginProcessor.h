/*
  ==============================================================================

    PluginProcessor.h
    Created: 30 Aug 2024 5:43:03pm
    Author:  Supersonics

  ==============================================================================
*/

#pragma once

#ifndef _EFFECT_PITCHSHIFTER_H_
#define _EFFECT_PITCHSHIFTER_H_

#include "TriplePhasePitchShifter.h"
#include <JuceHeader.h>

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

class TripleHeadPitchShifterStereo 
{
public:
	TripleHeadPitchShifterStereo();
	~TripleHeadPitchShifterStereo() {};
	//virtual void update();
	void update(juce::AudioBuffer<float>& buffer, int totalNumInputChannels, int totalNumOutputChannels);
	bool begin(void);
	
	/**
	 * @brief set the pitch.  Range -12 to +24
	 *
	 * @param semitones pitch shift in semitones
	 */
	void pitchSemitones(float semitones)
	{
		pitchShifterL.setPitchSemintone(semitones);
		pitchShifterR.setPitchSemintone(semitones);
	}

	/**
	* @brief set the pitch mix.  Range 0 to +1
	*
	* @param mix
	*/
	void pitchMix(float mix)
	{
		pitchShifterL.setPitchMix(mix);
		pitchShifterR.setPitchMix(mix);
	}
	/**
	 * @brief sets the pitch using the built in table
	 * 			input range is float 0.0 to 1.0
	 *
	 * @param value
	 */
	void pitchNormalized(float value)
	{
		value = constrain(value, 0.0f, 1.0f);
		float idx = map(value, 0.0f, 1.0f, 0.0f, (float)sizeof(semitoneTable) + 0.499f);
		pitch_semit = semitoneTable[(uint8_t)idx];
		pitchShifterL.setPitchSemintone(pitch_semit);
		pitchShifterR.setPitchSemintone(pitch_semit);
	}
	
	/**
	 * @brief setAudioBlocks
	 *
	 * @param s 256f(no audio block) - 32768f (100% audio blocks)
	 */
	void pitchAudioBlocks(float s)
	{
		pitchShifterL.setAudioBlocks(s);
		pitchShifterR.setAudioBlocks(s);
	}
	

	/**
	* @brief  pitch shifter triphase enable
	*
	* @param enable FALSEb(bi-phase) - TRUEb(tri phase)
	*/
	void enableTripleHead(bool enable)
	{
		pitchShifterL.setTriPhase(enable);
		pitchShifterR.setTriPhase(enable);
	}
	
	/**
	* @brief  pitch shifter reverse enable
	*
	* @param enable FALSEb(dir forward) - TRUEb (dir Reverse)
	*/
	void enableReverse(bool enable)
	{
		pitchShifterL.enableDirFoward(enable);
		pitchShifterR.enableDirFoward(enable);
	}
	
	float map(float x, float in_min, float in_max, float out_min, float out_max) {
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}

private:
	TripleHeadPitchShifterMono pitchShifterL;
	TripleHeadPitchShifterMono pitchShifterR;

	const int8_t semitoneTable[9] = { -12, -7, -5, -3, 0, 3, 5, 7, 12 };
	int8_t pitch_semit;
	bool initialised = false;
};

#endif