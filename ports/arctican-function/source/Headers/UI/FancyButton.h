/*
  ==============================================================================

    This file was auto-generated by the Jucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef __PHASEBUTTON_H_CFF4EBB1__
#define __PHASEBUTTON_H_CFF4EBB1__

#include "JuceHeader.h"
#include "JucePluginCharacteristics.h"

// Binary Image
#include "../Binary Data/UI/button.h"


//==============================================================================
/**
*/
class FancyButton  : public TextButton

{
public:
    //==============================================================================
    FancyButton();
    ~FancyButton();

    void paint (Graphics& g);

private:
    //==============================================================================

	Image buttonImage;
	int frameWidth;

};

#endif  // __PHASEBUTTON_H_CFF4EBB1__
