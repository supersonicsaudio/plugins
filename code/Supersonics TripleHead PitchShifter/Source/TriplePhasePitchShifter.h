/*
  ==============================================================================

	TripleHeadPitchShifter.h
	Created: 30 Aug 2024 6:00:11pm
	Author:  Supersonics

  ==============================================================================
*/
#ifndef _TRIPLE_HEAD_PITCH_SHIFTER_H_
#define _TRIPLE_HEAD_PITCH_SHIFTER_H_
#include <stdint.h>
#include <stdlib.h>
#include <cstring>
#include <math.h>

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
	

// 9 bit value = 2x fade table length (fade in + fade out)
#define CROSS_FADER_LENGHT_BITS 8
#define CROSS_FADER_DOUBLE_LENGTH (CROSS_FADER_LENGHT_BITS + 1)
#define CROSS_FADER_FRACTIONAL_BITS 23

const float AudioWaveformCrossFader[257] =
{
	0.000000f, 	 0.003075f, 	 0.006187f, 	 0.009336f, 	 0.012522f, 	 0.015745f, 	 0.019004f, 	 0.022300f,
	0.025633f, 	 0.029002f, 	 0.032407f, 	 0.035848f, 	 0.039324f, 	 0.042837f, 	 0.046384f, 	 0.049967f,
	0.053585f, 	 0.057238f, 	 0.060925f, 	 0.064647f, 	 0.068403f, 	 0.072193f, 	 0.076016f, 	 0.079873f,
	0.083764f, 	 0.087687f, 	 0.091643f, 	 0.095632f, 	 0.099653f, 	 0.103706f, 	 0.107791f, 	 0.111907f,
	0.116055f, 	 0.120233f, 	 0.124442f, 	 0.128681f, 	 0.132951f, 	 0.137250f, 	 0.141578f, 	 0.145936f,
	0.150322f, 	 0.154737f, 	 0.159180f, 	 0.163650f, 	 0.168148f, 	 0.172674f, 	 0.177226f, 	 0.181804f,
	0.186409f, 	 0.191039f, 	 0.195695f, 	 0.200375f, 	 0.205080f, 	 0.209810f, 	 0.214563f, 	 0.219340f,
	0.224139f, 	 0.228962f, 	 0.233806f, 	 0.238673f, 	 0.243561f, 	 0.248470f, 	 0.253400f, 	 0.258350f,
	0.263320f, 	 0.268309f, 	 0.273317f, 	 0.278343f, 	 0.283388f, 	 0.288450f, 	 0.293530f, 	 0.298626f,
	0.303738f, 	 0.308867f, 	 0.314011f, 	 0.319169f, 	 0.324342f, 	 0.329529f, 	 0.334730f, 	 0.339944f,
	0.345170f, 	 0.350408f, 	 0.355658f, 	 0.360920f, 	 0.366191f, 	 0.371473f, 	 0.376765f, 	 0.382066f,
	0.387375f, 	 0.392693f, 	 0.398018f, 	 0.403351f, 	 0.408690f, 	 0.414035f, 	 0.419386f, 	 0.424742f,
	0.430103f, 	 0.435468f, 	 0.440836f, 	 0.446208f, 	 0.451582f, 	 0.456958f, 	 0.462336f, 	 0.467714f,
	0.473093f, 	 0.478472f, 	 0.483851f, 	 0.489228f, 	 0.494604f, 	 0.499977f, 	 0.505348f, 	 0.510715f,
	0.516079f, 	 0.521438f, 	 0.526793f, 	 0.532142f, 	 0.537485f, 	 0.542821f, 	 0.548151f, 	 0.553473f,
	0.558787f, 	 0.564093f, 	 0.569389f, 	 0.574675f, 	 0.579952f, 	 0.585217f, 	 0.590471f, 	 0.595713f,
	0.600943f, 	 0.606160f, 	 0.611364f, 	 0.616553f, 	 0.621728f, 	 0.626888f, 	 0.632032f, 	 0.637160f,
	0.642271f, 	 0.647366f, 	 0.652442f, 	 0.657500f, 	 0.662539f, 	 0.667559f, 	 0.672559f, 	 0.677539f,
	0.682498f, 	 0.687435f, 	 0.692351f, 	 0.697244f, 	 0.702114f, 	 0.706960f, 	 0.711782f, 	 0.716580f,
	0.721353f, 	 0.726100f, 	 0.730822f, 	 0.735516f, 	 0.740184f, 	 0.744824f, 	 0.749436f, 	 0.754020f,
	0.758574f, 	 0.763099f, 	 0.767594f, 	 0.772058f, 	 0.776492f, 	 0.780894f, 	 0.785264f, 	 0.789602f,
	0.793907f, 	 0.798178f, 	 0.802416f, 	 0.806619f, 	 0.810788f, 	 0.814922f, 	 0.819020f, 	 0.823082f,
	0.827107f, 	 0.831096f, 	 0.835047f, 	 0.838960f, 	 0.842835f, 	 0.846672f, 	 0.850469f, 	 0.854227f,
	0.857945f, 	 0.861623f, 	 0.865260f, 	 0.868856f, 	 0.872411f, 	 0.875924f, 	 0.879394f, 	 0.882822f,
	0.886206f, 	 0.889548f, 	 0.892845f, 	 0.896099f, 	 0.899308f, 	 0.902472f, 	 0.905591f, 	 0.908664f,
	0.911692f, 	 0.914673f, 	 0.917608f, 	 0.920496f, 	 0.923336f, 	 0.926129f, 	 0.928875f, 	 0.931572f,
	0.934221f, 	 0.936821f, 	 0.939373f, 	 0.941875f, 	 0.944327f, 	 0.946730f, 	 0.949082f, 	 0.951384f,
	0.953636f, 	 0.955836f, 	 0.957986f, 	 0.960084f, 	 0.962131f, 	 0.964126f, 	 0.966069f, 	 0.967959f,
	0.969797f, 	 0.971583f, 	 0.973315f, 	 0.974995f, 	 0.976621f, 	 0.978194f, 	 0.979714f, 	 0.981179f,
	0.982591f, 	 0.983948f, 	 0.985252f, 	 0.986501f, 	 0.987695f, 	 0.988835f, 	 0.989920f, 	 0.990950f,
	0.991925f, 	 0.992845f, 	 0.993709f, 	 0.994519f, 	 0.995272f, 	 0.995971f, 	 0.996614f, 	 0.997201f,
	0.997732f, 	 0.998208f, 	 0.998628f, 	 0.998992f, 	 0.999300f, 	 0.999552f, 	 0.999748f, 	 1.000000f,
	1.000000f
};

/************************************************************************/
/* Pitch intervals in range from -1oct to +2 oct, step 1semitone        */
/************************************************************************/
const float musicIntevals[37] =
{
	 0.500000f,  0.529732f,  0.561231f,  0.594604f,  0.629961f,  0.667420f,  0.707107f,  0.749154f,
	 0.793701f,  0.840896f,  0.890899f,  0.943874f,  1.000000f,  1.059463f,  1.122462f,  1.189207f,
	 1.259921f,  1.334840f,  1.414214f,  1.498307f,  1.587401f,  1.681793f,  1.781797f,  1.887749f,
	 2.000000f,  2.118926f,  2.244924f,  2.378414f,  2.519842f,  2.669680f,  2.828427f,  2.996614f,
	 3.174802f,  3.363586f,  3.563595f,  3.775497f,  4.000000f
};


class TripleHeadPitchShifterMono
{
public:
	bool init()
	{
		bf = (float *) malloc( (1 << (int32_t) maxBuffBits) * sizeof(float)); // allocate buffer
		if (!bf) return false;
		reset();
		return true;
	}

	void setPitch(float ratio)
	{
		readAdder = (float)pitchDelta0 * ratio;
	}
	void setPitchSemintone(int s)
	{
		s = constrain(s, -12, +24); // limit to the predefined range
		setPitch(musicIntevals[s + 12]);
	}

	void setPitchMix(float _mix)
	{
		mix = _mix;
	}
	inline float process(float newSample)
	{
		/*
		1) Write to buffer and update pointer
		2) Get heads's samples
		3) Interpolate between heads
		*/

				
		float head1Value, head2Value, head3Value;
		float head1CrossFadeValue, head2CrossFadeValue, head3CrossFadeValue;
		float head1InterSampleCrossfadeCoeff, head2InterSampleCrossfadeCoeff, head3InterSampleCrossfadeCoeff;
		uint32_t head1Idx, head2Idx, head3Idx;
		uint32_t head1CrossfadeIdx, head2CrossfadeIdx, head3CrossfadeIdx;

		uint32_t deltaAcc, delta2, delta3;
		uint32_t deltaInt;
		float deltaFrac;
		
		float outSample = 0;


		bf[writeAddr] = newSample;	// write new sample
		
		// update read pointer, 
		// readAdder controls the pitch
		if(directionReverse)
			readAddr = readAddr - readAdder;		
		else
			readAddr = readAddr + readAdder;

		// bypass if no pitch change
		if ( readAdder == pitchDelta0)
		{
			writeAddr = (writeAddr + 1) & buffMask;
			return newSample;
		}

		if (tripleHeads) {
			// fractional delay head #1 @ 0° (offest = 0)
			head1Idx = (readAddr >> (counterMaxBits - buffBits)) & buffMask;						// index of the last sample 
			head1InterSampleCrossfadeCoeff = (float)(readAddr & buffFracMask) / (float)buffFracMask;	// fractional part
			head1Value = bf[head1Idx] * (1.0f - head1InterSampleCrossfadeCoeff);
			head1Value += bf[(head1Idx + 1) & buffMask] * head1InterSampleCrossfadeCoeff;							// interpolated sample

			// fractional delay head #2 @ 120° (offset = 1/3 Lenght)
			head2Idx = ((readAddr + counterOneThirdOffset) >> (counterMaxBits - buffBits)) & buffMask;
			head2InterSampleCrossfadeCoeff = (float)((readAddr + counterOneThirdOffset) & buffFracMask) / (float)buffFracMask;
			head2Value = bf[head2Idx] * (1.0f - head2InterSampleCrossfadeCoeff);
			head2Value += bf[(head2Idx + 1) & buffMask] * head2InterSampleCrossfadeCoeff;

			// fractional delay head #3 @ 240° (offset = 2/3 Length)
			head3Idx = ((readAddr + counterTwoThirdsOffset) >> (counterMaxBits - buffBits)) & buffMask;
			head3InterSampleCrossfadeCoeff = (float)((readAddr + counterTwoThirdsOffset) & buffFracMask) / (float)buffFracMask;
			head3Value = bf[head3Idx] * (1.0f - head3InterSampleCrossfadeCoeff);
			head3Value += bf[(head3Idx + 1) & buffMask] * head3InterSampleCrossfadeCoeff;

			//Interpolate between heads
			deltaAcc = readAddr - (writeAddr << (counterMaxBits - buffBits));	// distance between the write and read pointer

			deltaInt = (deltaAcc >> (counterMaxBits - CROSS_FADER_DOUBLE_LENGTH)) & 0x1FF;	// 9 bit value = 2x fade table length (fade in + fade out)
			deltaFrac = (float)(deltaAcc & ((1 << CROSS_FADER_FRACTIONAL_BITS) - 1)) / (float)((1 << CROSS_FADER_FRACTIONAL_BITS) - 1);	// fractional part for the xfade curve

			head1CrossfadeIdx = (deltaInt + 0) & 0xFF;
			head1CrossFadeValue = AudioWaveformCrossFader[head1CrossfadeIdx] * (1.0f - deltaFrac) + AudioWaveformCrossFader[head1CrossfadeIdx + 1] * deltaFrac;	// interpolated smooth crossfade coeff.

			if ((deltaInt + 0) > 255) head1CrossFadeValue = 1.0f - head1CrossFadeValue;					// invert the curve for the fade out part

			outSample = head1Value * head1CrossFadeValue;			// crossfade the first sample

			delta2 = (deltaInt + 170) & 0x1FF; // 120° offset
			head2CrossfadeIdx = (delta2) & 0xFF;
			head2CrossFadeValue = AudioWaveformCrossFader[head2CrossfadeIdx] * (1.0f - deltaFrac) + AudioWaveformCrossFader[head2CrossfadeIdx + 1] * deltaFrac;	// interpolated smooth crossfade coeff.

			if ((delta2 + 0) > 255) head2CrossFadeValue = 1.0f - head2CrossFadeValue;					// invert the curve for the fade out part

			outSample += head2Value * head2CrossFadeValue;		// crossfade the second sample

			delta3 = (deltaInt + 340) & 0x1FF; // 240° offset
			head3CrossfadeIdx = (delta3) & 0xFF;
			head3CrossFadeValue = AudioWaveformCrossFader[head3CrossfadeIdx] * (1.0f - deltaFrac) + AudioWaveformCrossFader[head3CrossfadeIdx + 1] * deltaFrac;	// interpolated smooth crossfade coeff.

			if ((delta3 + 0) > 255) head3CrossFadeValue = 1.0f - head3CrossFadeValue;					// invert the curve for the fade out part

			outSample += head3Value * head3CrossFadeValue;		// crossfade the last sample

		}
		else
		{
			// fractional delay head #1 @ 0° (offest = 0)
			head1Idx = (readAddr >> (counterMaxBits - buffBits)) & buffMask;				// index of the last sample 
			head1InterSampleCrossfadeCoeff = (float)(readAddr & buffFracMask) / (float)buffFracMask;	// fractional part
			head1Value = bf[head1Idx] * (1.0f - head1InterSampleCrossfadeCoeff);
			head1Value += bf[(head1Idx + 1) & buffMask] * head1InterSampleCrossfadeCoeff;						// Head1's interpolated sample
			
			// fractional delay head #2 @ 180° (offest = 0.5*L)
			head2Idx = ((readAddr + counterHalfOffset) >> (counterMaxBits - buffBits)) & buffMask;
			head2InterSampleCrossfadeCoeff = (float)((readAddr + counterHalfOffset) & buffFracMask) / (float)buffFracMask;
			head2Value = bf[head2Idx] * (1.0f - head2InterSampleCrossfadeCoeff);
			head2Value += bf[(head2Idx + 1) & buffMask] * head2InterSampleCrossfadeCoeff;						// Head2's interpolated sample

			// Interpolate between heads
			deltaAcc = readAddr - (writeAddr << (counterMaxBits - buffBits));			// distance between the write and read pointer

			deltaInt = (deltaAcc >> (counterMaxBits - 9)) & 0x1FF;					// 9 bit value = 2x fade table length (fade in + fade out)
			deltaFrac = (float)(deltaAcc & ((1 << CROSS_FADER_FRACTIONAL_BITS) - 1)) / (float)((1 << CROSS_FADER_FRACTIONAL_BITS) - 1);	// fractional part for the xfade curve
			head1CrossfadeIdx = deltaInt & 0xFF;
			head1CrossFadeValue = AudioWaveformCrossFader[head1CrossfadeIdx] * (1.0f - deltaFrac) + AudioWaveformCrossFader[head1CrossfadeIdx + 1] * deltaFrac;	// interpolated smooth crossfade coeff.

			if (deltaInt > 0xFF) head1CrossFadeValue = 1.0f - head1CrossFadeValue;					// invert the curve for the fade out part

			outSample = head1Value * head1CrossFadeValue + head2Value * (1.0f - head1CrossFadeValue);			// crossfade the last and mid sample

		}

		writeAddr = (writeAddr + 1) & buffMask;		// update the write pointer
		return (outSample * mix + newSample * (1.0f - mix));			//return full wet. Let dry-mix controll be made be calling process
	}


	void setAudioBlocks(float audioB)
	{
		audioBlocks = log2f(audioB);
		buffBits = (uint32_t)(audioBlocks);
		buffSize = (1 << buffBits);
		buffSizeHalf = (1 << (buffSize - 1));
		buffMask = (buffSize - 1);
		buffFracMask = ((1 << (counterMaxBits - buffBits)) - 1);
		pitchDelta0 = buffFracMask + 1;
	}
	
	void setTriPhase(bool enable)
	{
		tripleHeads = enable;
	}

	void enableDirFoward(bool dir)
	{
		directionReverse = dir;
	}

	float map(float x, float in_min, float in_max, float out_min, float out_max) {
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}

	void reset()
	{
		memset(bf, 0, (1 << maxBuffBits) *sizeof(float));

		readAddr = 0;
		writeAddr = 0;
		readAdder = pitchDelta0;
	}

private:
	float *bf = 0;
	
	uint32_t readAddr;
	uint32_t readAdder;
	uint16_t writeAddr;
	
	float audioBlocks;
	float mix;
	bool tripleHeads = true;
	bool directionReverse = true;

	const uint32_t counterMaxBits = 32;
	uint64_t  counterMaxValue = ((uint64_t)1 << (uint64_t)counterMaxBits);
	uint64_t  counterHalfOffset = counterMaxValue / 2;
	uint64_t  counterOneThirdOffset = counterMaxValue / 3;
	uint64_t  counterTwoThirdsOffset = counterMaxValue * 2 / 3;


	const uint32_t maxBuffBits	= counterMaxBits / 2;
	uint32_t buffBits			= 14; // Inital value
	uint32_t buffSize			= (1 << buffBits);
	uint32_t buffSizeHalf		= (1 << (buffBits - 1));
	uint32_t buffMask			= (buffSize - 1);
	uint32_t buffFracMask		= ((1 << (32 - buffBits)) - 1);
	uint32_t pitchDelta0		= buffFracMask + 1;

};
#endif // _TRIPLE_HEAD_PITCH_SHIFTER_H_
