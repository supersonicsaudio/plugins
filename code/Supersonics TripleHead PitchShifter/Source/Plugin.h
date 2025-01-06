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
class TriplePhasePitchShifter  : public juce::AudioProcessor
{
    enum RadioButtonIds
    {
        GenderButtons = 1001
    };

public:
    //==============================================================================
    TriplePhasePitchShifter()
        : AudioProcessor (BusesProperties().withInput  ("Input",     juce::AudioChannelSet::stereo())          
                                           .withOutput ("Output",    juce::AudioChannelSet::stereo())
                                           .withInput  ("Sidechain", juce::AudioChannelSet::stereo()))
    {
        auto choices = std::vector<double>{ 4096, 8192, 16384, 32768 };
        juce::StringArray sa;
        for (auto choice : choices)
        {
            sa.add(juce::String(choice));
        }

        addParameter(blockSize = new juce::AudioParameterChoice("Block Size", "Block Size", sa, 1));

        addParameter(pitch       = new juce::AudioParameterInt("Pitch",             "Pitch",        -12, 24, 12));
        addParameter(mix         = new juce::AudioParameterFloat("Mix",             "Mix",    0.0f, 1.0f, 0.66f));
        addParameter(tripleHead  = new juce::AudioParameterBool("3-HEADS",          "3-HEADS",              true));
        addParameter(reverse     = new juce::AudioParameterBool("REVERSE",          "REVERSE",             false));
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
        auto phase = *tripleHead ? -1.0f : 1.0f;
        auto dir = *reverse ? -1.0f : 1.0f;
    }

    void releaseResources() override          {}

    void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override
    {
  
        // Update any changes from UI
        pitchShifter.pitchSemitones(pitch->get());
        pitchShifter.pitchMix(mix->get());
        pitchShifter.pitchAudioBlocks(blockSize->getCurrentChoiceName().getFloatValue() );
        
        auto triple = tripleHead->get(); 
        pitchShifter.enableTripleHead(triple);
        
        auto dir = reverse->get(); 
        pitchShifter.enableReverse(dir);
        
        juce::ScopedNoDenormals noDenormals;
        auto totalNumInputChannels = getTotalNumInputChannels();
        auto totalNumOutputChannels = getTotalNumOutputChannels();
     
        for (auto i = totalNumInputChannels; i < totalNumOutputChannels; i++)
            buffer.clear(i, 0, buffer.getNumSamples());

        pitchShifter.update(buffer, totalNumInputChannels, totalNumOutputChannels);
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
        //juce::MemoryOutputStream stream (destData, true);

    }

    void setStateInformation (const void* data, int sizeInBytes) override
    {
       // juce::MemoryInputStream stream (data, static_cast<size_t> (sizeInBytes), false);
    }

private:
    //==============================================================================
    TripleHeadPitchShifterStereo    pitchShifter;
    juce::AudioParameterInt*        pitch;
    juce::AudioParameterFloat*      mix;
    juce::AudioParameterChoice*     blockSize;
    juce::AudioParameterBool*       tripleHead;
    juce::AudioParameterBool*       reverse;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TriplePhasePitchShifter)
};
