/*
  ==============================================================================

    EnvelopDetector.h
    Created: 13 Sep 2024 9:20:13pm
    Author:  Supersonics

  ==============================================================================
*/

#pragma once

#ifndef _ENVELOP_DETECTOR_H_
#define _ENVELOP_DETECTOR_H_
#include<stdint.h>
#include<math.h>

class EnvelopeDetector
{
public:
	EnvelopeDetector(float attackRateHz, float releaseRateHz)
	{
		float attack_msec = 5.0f;
		float release_msec = 50.0f;
		setAttack_msec(attack_msec);
		setRelease_msec(release_msec);
	}

	float smooth_env(float x, float xpk){
		float  xab;
	
		xab = (x >= 0.0f) ? x : -x;
		
		if (xab >= xpk) {
			xpk = alfa * xpk + (1.f - alfa) * xab;
		}
		else {
			xpk = beta * xpk;
		}

		return xpk;
	}
	void setAttack_msec(const float atk_sec) {
		float tc = 1.0f / (atk_sec * 44100);
		alfa = expf(-2.0f * 3.14f * tc);
	}

	void setRelease_msec(const float rel_sec) {
		float tc = 1.0f / (rel_sec * 44100);
		beta = expf(-2.0f * 3.14f * tc);
	}
	void setDefaultValues(void) {

	}

private:
	float alfa, beta;  //time constants, but in terms of samples, not seconds
};

#endif // _ENVELOPE_DETECTOR_H_
