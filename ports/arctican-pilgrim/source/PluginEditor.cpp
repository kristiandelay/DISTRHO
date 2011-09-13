/*
  ==============================================================================

    This file was auto-generated by the Jucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
ThePilgrimAudioProcessorEditor::ThePilgrimAudioProcessorEditor (ThePilgrimAudioProcessor* ownerFilter)
    : AudioProcessorEditor (ownerFilter)
{
    // This is where our plugin's editor size is set.
    setSize (155, 140);

	// Filter Knob
	addAndMakeVisible (&filterKnob);
    filterKnob.setSliderStyle (Slider::RotaryVerticalDrag);
    filterKnob.addListener (this);
    filterKnob.setRange (0.0, 1.0, 0.0001);
	filterKnob.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	filterKnob.setPopupDisplayEnabled(true,0);
	filterKnob.setReadoutType("doublefilter");

	// Wet/Dry Knob
	addAndMakeVisible (&wetdryKnob);
    wetdryKnob.setSliderStyle (Slider::RotaryVerticalDrag);
    wetdryKnob.addListener (this);
    wetdryKnob.setRange (0.0, 1.0, 0.0001);
	wetdryKnob.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	wetdryKnob.setReadoutType("percent");

	// Load background from Binary Data
	startTimer (50);
	backgroundGUI = ImageFileFormat::loadFrom (thepilgrimbackground::thepilgrimgui_png, thepilgrimbackground::thepilgrimgui_pngSize);
}

ThePilgrimAudioProcessorEditor::~ThePilgrimAudioProcessorEditor()
{
	filterKnob.removeListener(this);
	wetdryKnob.removeListener(this);
}

//==============================================================================
void ThePilgrimAudioProcessorEditor::paint (Graphics& g)
{
	// Draw background Image
	g.drawImage(backgroundGUI,0,0,155,140,0,0,155,140,false);
}

void ThePilgrimAudioProcessorEditor::resized()
{
	// Set UI components position
	filterKnob.setBounds (29, 45, 40, 40);
	wetdryKnob.setBounds (89, 45, 40, 40);
}

void ThePilgrimAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
	ThePilgrimAudioProcessor* ourProcessor = getProcessor();
	if (slider == &filterKnob)
    {
        getProcessor()->setParameterNotifyingHost (ThePilgrimAudioProcessor::filterFreqParam,				
                                                  (float) filterKnob.getValue()); 
		ourProcessor->filterChanged(); // Tell filter to change frequency
    }
	else if (slider == &wetdryKnob)
    {
        getProcessor()->setParameterNotifyingHost (ThePilgrimAudioProcessor::mixParam,				
                                                  (float) wetdryKnob.getValue());
    }

}

void ThePilgrimAudioProcessorEditor::timerCallback()
{
	ThePilgrimAudioProcessor* ourProcessor = getProcessor();

    filterKnob.setValue (ourProcessor->filterFreq, false);
    wetdryKnob.setValue (ourProcessor->mixAmount, false);
}