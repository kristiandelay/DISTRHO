﻿/*
  ==============================================================================

    This file was auto-generated by the Jucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "Knob.h"


//==============================================================================
Knob::Knob()
{
	knobImage = ImageFileFormat::loadFrom (knobs::bigknob_png, knobs::bigknob_pngSize);
	smallKnobImage = ImageFileFormat::loadFrom (knobs::smallknob_png, knobs::smallknob_pngSize);

	readoutType = "default";
	//testReadout = "o";
	frameWidth = 100;

}

Knob::~Knob()
{
}

//==============================================================================
void Knob::paint (Graphics& g)
{


   const double div   = 1.0 / 15.0;
   double pos         = (int)(Knob::getValue() / div);

  // if (pos > 0)
     // pos = pos - 1;

   // Little hack to get image centred when knob value at half
   if (pos == 15)
      pos = 14;

   if (Knob::getWidth() == 100)
      g.drawImage (knobImage, 0,0,100,100, 0, (int)(pos*100), 100, 100, false);
   else if (Knob::getWidth() == 40)
      g.drawImage (smallKnobImage, 0,0,40,40, 0, (int)(pos*40), 40, 40, false);

  // g.setColour(juce::Colour(255,255,255));
  // g.drawFittedText(testReadout,0,0,40,40,1,1);

}


/* thanks to anita for the math 

void FilmStripSlider::drawFrame (Graphics &g, int x, int y, int width, int height, Slider &slider)
{
   const double div   = slider.getMaximum() / frameCount;
   double pos         = (int)(slider.getValue() / div);

   if (pos > 0)
      pos = pos - 1;



   if (isVerticalStrip)
   {
      g.drawImage (knobStrip, x, y, width, height, 0, (int)(pos*frameSize), frameSize, frameSize, false);
   }

}*/

const String Knob::getTextFromValue (double value)
{
	String displayText;

	if (readoutType == "default")
		displayText = (String(value));

	else if (readoutType == "pan")
	{
		if (value < 0.5)
		{
			displayText = (String((((value *200.0) -100.0) *-1.0)) + "% L");
		}
		else if (value == 0.5)
			displayText = "C";
		if (value > 0.5)
		{
			displayText = (String((((value *2.0) -1.0) *100.0)) + "% R");
		}
	}

	else if (readoutType == "gain")
	{
		if (value == 0)
		{
			displayText = "-infdB";
			//displayText = "-\u221E";
		}
		else 
		{
			//value = value * value * value;
			displayText = String(20*log(value));
			int rounder = displayText.indexOfChar(0,'.');
			displayText = (displayText.substring(0, rounder+3) + "dB");
			//displayText = (String((((value *2.0) -1.0) *100.0)) + "% R");
		}
	}



	return displayText;
}

void Knob::setReadoutType(String type)
{

	readoutType = type;

}
