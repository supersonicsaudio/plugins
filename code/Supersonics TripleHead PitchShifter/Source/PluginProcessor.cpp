/*
  ==============================================================================

    PluginProcessor.cpp
    Created: 30 Aug 2024 6:00:11pm
    Author:  Supersonics

  ==============================================================================
*/

#include "PluginProcessor.h"

TripleHeadPitchShifterStereo::TripleHeadPitchShifterStereo() { begin(); }

bool TripleHeadPitchShifterStereo::begin()
{

	if (!pitchShifterL.init()) return false;
	if (!pitchShifterR.init()) return false;

	pitchShifterL.setPitch(1.0f); //natural pitch
	pitchShifterR.setPitch(1.0f); //natural pitch
	
	return true;
}

void TripleHeadPitchShifterStereo::update(juce::AudioBuffer<float>& buffer, int totalNumInputChannels, int totalNumOutputChannels)
{
	auto* channelDataL = buffer.getWritePointer(0);
	auto* channelDataR = buffer.getWritePointer(1);
	for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
	{
		channelDataL[sample] = pitchShifterL.process(channelDataL[sample]);
		channelDataR[sample] = pitchShifterR.process(channelDataR[sample]);
	}
}