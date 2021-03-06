/*
  ==============================================================================

   Build options for juce-audio-devices static library

  ==============================================================================
*/

#ifndef __DISTRHO_JUCE_AUDIO_DEVICES_APPCONFIG_H__
#define __DISTRHO_JUCE_AUDIO_DEVICES_APPCONFIG_H__

#include "../build-events/AppConfig.h"
#include "../build-audio-basics/AppConfig.h"
#include "../build-audio-formats/AppConfig.h"

//=============================================================================
/** Config: JUCE_ASIO
    Enables ASIO audio devices (MS Windows only).
    Turning this on means that you'll need to have the Steinberg ASIO SDK installed
    on your Windows build machine.

    See the comments in the ASIOAudioIODevice class's header file for more
    info about this.
*/
#define JUCE_ASIO 0

/** Config: JUCE_WASAPI
    Enables WASAPI audio devices (Windows Vista and above).
*/
#define JUCE_WASAPI 0

/** Config: JUCE_DIRECTSOUND
    Enables DirectSound audio (MS Windows only).
*/
#define JUCE_DIRECTSOUND 0

/** Config: JUCE_ALSA
    Enables ALSA audio devices (Linux only).
*/
#define JUCE_ALSA 0

/** Config: JUCE_JACK
    Enables JACK audio devices (Linux only).
*/
#define JUCE_JACK 0

/** Config: JUCE_USE_ANDROID_OPENSLES
    Enables OpenSLES devices (Android only).
*/
#define JUCE_USE_ANDROID_OPENSLES 0

//=============================================================================
/** Config: JUCE_USE_CDREADER
    Enables the AudioCDReader class (on supported platforms).
*/
#define JUCE_USE_CDREADER 0

/** Config: JUCE_USE_CDBURNER
    Enables the AudioCDBurner class (on supported platforms).
*/
#define JUCE_USE_CDBURNER 0

#endif // __DISTRHO_JUCE_AUDIO_DEVICES_APPCONFIG_H__
