/*
  ==============================================================================

   Build options for juce-audio-processors static library

  ==============================================================================
*/

#ifndef __DISTRHO_JUCE_AUDIO_PROCESSORS_APPCONFIG_H__
#define __DISTRHO_JUCE_AUDIO_PROCESSORS_APPCONFIG_H__

#include "../build-gui-basics/AppConfig.h"
#include "../build-audio-basics/AppConfig.h"

//=============================================================================
/** Config: JUCE_PLUGINHOST_VST
    Enables the VST audio plugin hosting classes. This requires the Steinberg VST SDK to be
    installed on your machine.

    @see VSTPluginFormat, AudioPluginFormat, AudioPluginFormatManager, JUCE_PLUGINHOST_AU
*/
#define JUCE_PLUGINHOST_VST 0

/** Config: JUCE_PLUGINHOST_AU
    Enables the AudioUnit plugin hosting classes. This is Mac-only, of course.

    @see AudioUnitPluginFormat, AudioPluginFormat, AudioPluginFormatManager, JUCE_PLUGINHOST_VST
*/
#define JUCE_PLUGINHOST_AU 0

#endif // __DISTRHO_JUCE_AUDIO_PROCESSORS_APPCONFIG_H__
