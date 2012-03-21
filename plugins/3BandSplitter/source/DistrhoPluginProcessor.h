
#ifndef __DISTRHOPLUGINPROCESSOR_H__
#define __DISTRHOPLUGINPROCESSOR_H__

#include "JuceHeader.h"
#include "JucePluginCharacteristics.h"

//==================================================================================
class DistrhoPluginAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    DistrhoPluginAudioProcessor();
    ~DistrhoPluginAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();

    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;

    //==============================================================================
    const String getName() const;

    int getNumParameters();

    float getParameter (int index);
    void setParameter (int index, float newValue);

    const String getParameterName (int index);
    const String getParameterText (int index);

    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;

    bool acceptsMidi() const;
    bool producesMidi() const;

    //==============================================================================
    int getNumPrograms();
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);

    //==============================================================================
    enum Parameters
    {
      pLow = 0,
      pMid,
      pHigh,
      pMaster,
      pLowMidFreq,
      pMidHighFreq,
      pMAX
    };

private:
    //==============================================================================
    float fLow, fMid, fHigh, fMaster, fLowMidFreq, fMidHighFreq;

    //==============================================================================
    float lowVol, midVol, highVol, outVol;
    float freqLP, freqHP;

    float xLP, a0LP, b1LP;
    float xHP, a0HP, b1HP;

    float out1LP, out2LP, out1HP, out2HP;
    float tmp1LP, tmp2LP, tmp1HP, tmp2HP;

    //==============================================================================
    bool initialized;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistrhoPluginAudioProcessor);
};

#endif  // __DISTRHOPLUGINPROCESSOR_H__