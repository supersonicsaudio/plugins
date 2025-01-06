/*
  ==============================================================================

    Plugin.h
    Created: 30 Aug 2024 5:43:03pm
    Author:  Supersonics

  ==============================================================================
*/

#pragma once
#include "PluginProcessor.h"
//==============================================================================
class DynamicsShaperClass  : public juce::AudioProcessor
{
public:
    //==============================================================================
    DynamicsShaperClass()
        : AudioProcessor (BusesProperties().withInput  ("Input",     juce::AudioChannelSet::stereo())          
                                           .withOutput ("Output",    juce::AudioChannelSet::stereo())
                                           .withInput  ("Sidechain", juce::AudioChannelSet::stereo()))
    {

        addParameter(sensitivity = new juce::AudioParameterFloat("Sensitivity", "Sensitivity",  juce::NormalisableRange<float>(0.0f, 1.0f, 0.000001f, 0.15f), 0.087953f));
        addParameter(attack      = new juce::AudioParameterFloat("Attack",      "Attack",       juce::NormalisableRange<float>(0.0f, 1.0f, 0.000001f, 0.3f), 1.0f));
        addParameter(decay       = new juce::AudioParameterFloat("Decay",       "Decay",        juce::NormalisableRange<float>(0.0f, 1.0f, 0.000001f, 0.15f), 0.0f));
        addParameter(sustain     = new juce::AudioParameterFloat("Sustain",     "Sustain",      juce::NormalisableRange<float>(0.0f, 1.0f, 0.000001f, 0.15f), 0.0f));
        addParameter(release     = new juce::AudioParameterFloat("Release",     "Release",      juce::NormalisableRange<float>(0.0f, 1.0f, 0.000001f, 0.15f), 0.0f));
   }

    //==============================================================================
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override
    {
        // the sidechain can take any layout, the main bus needs to be the same on the input and output
        return layouts.getMainInputChannelSet() == layouts.getMainOutputChannelSet()
                 && ! layouts.getMainInputChannelSet().isDisabled();
    }

    //==============================================================================
    void prepareToPlay(double, int) override
    {
        createEditor()->setResizable(true, true);
    }

    void releaseResources() override          {}

    void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override
    {
  
        // Update any changes from UI
        DynamicsShaper.setSensitivity((sensitivity->get()));
        DynamicsShaper.setAttack((attack->get()));
        DynamicsShaper.setSustain((sustain->get()));
        DynamicsShaper.setDecay((decay->get()));
        DynamicsShaper.setRelease((release->get()));
       
        juce::ScopedNoDenormals noDenormals;
        auto totalNumInputChannels = getTotalNumInputChannels();
        auto totalNumOutputChannels = getTotalNumOutputChannels();
     
        for (auto i = totalNumInputChannels; i < totalNumOutputChannels; i++)
            buffer.clear(i, 0, buffer.getNumSamples());

        DynamicsShaper.update(buffer, totalNumInputChannels, totalNumOutputChannels);
    }

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override            { return new juce::GenericAudioProcessorEditor (*this); }
    bool hasEditor() const override                                { return true; }
    const juce::String getName() const override                    { return "Verbarose"; }
    bool acceptsMidi() const override                              { return false; }
    bool producesMidi() const override                             { return false; }
    double getTailLengthSeconds() const override                   { return 0.0; }
    int getNumPrograms() override                                  { return 1; }
    int getCurrentProgram() override                               { return 0; }
    void setCurrentProgram (int) override                          {}
    const juce::String getProgramName (int) override               { return {}; }
    void changeProgramName (int, const juce::String&) override     {}

    bool isVST2() const noexcept                                   { return (wrapperType == wrapperType_VST); }

    //==============================================================================

    void getStateInformation (juce::MemoryBlock& destData) override
    {
        juce::MemoryOutputStream stream (destData, true);
 
        stream.writeFloat(*sensitivity);
        stream.writeFloat(*attack);
        stream.writeFloat(*sustain);
        stream.writeFloat(*decay);
        stream.writeFloat(*release);
    }

    void setStateInformation (const void* data, int sizeInBytes) override
    {
        juce::MemoryInputStream stream (data, static_cast<size_t> (sizeInBytes), false);

        sensitivity->setValueNotifyingHost(stream.readFloat());
        attack->setValueNotifyingHost(stream.readFloat());
        sustain->setValueNotifyingHost(stream.readFloat());
        decay->setValueNotifyingHost(stream.readFloat());
        release->setValueNotifyingHost(stream.readFloat());
    }

private:
    //==============================================================================
    AudioEffectDynamicsShaper   DynamicsShaper;
    juce::AudioParameterFloat*  sensitivity;
    juce::AudioParameterFloat*  attack;
    juce::AudioParameterFloat*  sustain;
    juce::AudioParameterFloat*  decay;
    juce::AudioParameterFloat*  release;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DynamicsShaperClass)
};
