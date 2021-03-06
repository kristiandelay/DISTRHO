/*
  ==============================================================================

   Build options for juce-audio-processors_full static library

  ==============================================================================
*/

#ifndef __DISTRHO_JUCE_AUDIO_PROCESSORS_FULL_APPCONFIG_H__
#define __DISTRHO_JUCE_AUDIO_PROCESSORS_FULL_APPCONFIG_H__

#include "../build-gui-basics/AppConfig.h"
#include "../build-audio-basics/AppConfig.h"

#define JUCE_PLUGINHOST_LADSPA 1

//=============================================================================
/** Config: JUCE_PLUGINHOST_VST
    Enables the VST audio plugin hosting classes. This requires the Steinberg VST SDK to be
    installed on your machine.

    @see VSTPluginFormat, AudioPluginFormat, AudioPluginFormatManager, JUCE_PLUGINHOST_AU
*/
#define JUCE_PLUGINHOST_VST 1
#define JUCE_USE_VSTSDK_2_4 1

/** Config: JUCE_PLUGINHOST_AU
    Enables the AudioUnit plugin hosting classes. This is Mac-only, of course.

    @see AudioUnitPluginFormat, AudioPluginFormat, AudioPluginFormatManager, JUCE_PLUGINHOST_VST
*/
#if MAC
 #define JUCE_PLUGINHOST_AU 1
#else
 #define JUCE_PLUGINHOST_AU 0
#endif

#endif // __DISTRHO_JUCE_AUDIO_PROCESSORS_FULL_APPCONFIG_H__
