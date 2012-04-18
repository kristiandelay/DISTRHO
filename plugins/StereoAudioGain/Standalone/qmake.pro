# qmake project file

QT = core gui

CONFIG += qt resources warn_on
CONFIG += release
#CONFIG += debug

TARGET   = ../../../bin/StereoAudioGain
TEMPLATE = app

SOURCES  = \
    ../source/StereoAudioGain.cpp \
    ../source/StereoAudioGainUi.cpp \
    ../../common/Qt4-Widgets/digitalpeakmeter.cpp \
    ../../common/Qt4-Widgets/pixmapdial.cpp \

HEADERS  = \
    ../source/DistrhoPlugin.h \
    ../source/StereoAudioGain.h \
    ../source/StereoAudioGainUi.h \
    ../../common/Qt4-Widgets/digitalpeakmeter.h \
    ../../common/Qt4-Widgets/pixmapdial.h

HEADERS += \
    ../../../libs/distrho-ports/PluginBase.h \
    ../../../libs/distrho-ports/UiBase.h

FORMS    = \
    ../source/StereoAudioGain.ui

RESOURCES = \
    ../../common/Artwork/knobs/knobs.qrc

INCLUDEPATH = \
    ../source \
    ../../common/Qt4-Widgets

# -----------------------------------------------------------

CONFIG += DISTRHO_PLUGIN_TARGET_JACK
include(../../../libs/distrho-ports/d_src.pri)