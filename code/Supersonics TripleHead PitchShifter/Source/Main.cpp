/*
  ==============================================================================

    Main.cpp
    Created: 30 Aug 2024 5:43:03pm
    Author:  Supersonics

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Plugin.h"

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TriplePhasePitchShifter();
}
