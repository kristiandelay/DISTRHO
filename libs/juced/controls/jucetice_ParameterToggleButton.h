/*
 ==============================================================================

 This file is part of the JUCETICE project - Copyright 2009 by Lucio Asnaghi.

 JUCETICE is based around the JUCE library - "Jules' Utility Class Extensions"
 Copyright 2007 by Julian Storer.

 ------------------------------------------------------------------------------

 JUCE and JUCETICE can be redistributed and/or modified under the terms of
 the GNU General Public License, as published by the Free Software Foundation;
 either version 2 of the License, or (at your option) any later version.

 JUCE and JUCETICE are distributed in the hope that they will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with JUCE and JUCETICE; if not, visit www.gnu.org/licenses or write to
 Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 Boston, MA 02111-1307 USA

 ==============================================================================
*/

#ifndef __JUCETICE_PARAMETERTOGGLEBUTTON_HEADER__
#define __JUCETICE_PARAMETERTOGGLEBUTTON_HEADER__

#include "../base/jucetice_AudioParameter.h"


//==============================================================================
/**
     A parameter listener toggle button
*/
class ParameterToggleButton  : public ToggleButton,
                               public AudioParameterListener
{
public:

    //==============================================================================
    ParameterToggleButton (const String& buttonText)
        : ToggleButton (buttonText)
    {
    }

    //==============================================================================
    virtual ~ParameterToggleButton()
    {
    }

    //==============================================================================
    void parameterChanged (AudioParameter* parameter, const int index)
    {
        setToggleState ((parameter->getValueMapped () > 0.5f ? true : false), false);
    }

};


#endif // __JUCETICE_PARAMETERTOGGLEBUTTON_HEADER__
