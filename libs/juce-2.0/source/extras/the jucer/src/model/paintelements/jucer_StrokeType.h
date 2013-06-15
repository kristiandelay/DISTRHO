/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2013 - Raw Material Software Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/

#ifndef __JUCER_STROKETYPE_JUCEHEADER__
#define __JUCER_STROKETYPE_JUCEHEADER__

#include "jucer_FillType.h"


//==============================================================================
/**
    Defines the attributes of a stroke to use around a shape.
*/
class StrokeType
{
public:
    //==============================================================================
    StrokeType();
    ~StrokeType();

    //==============================================================================
    const String getPathStrokeCode() const;

    const String toString() const;
    void restoreFromString (const String& s);

    bool isOpaque() const;
    bool isInvisible() const;

    //==============================================================================
    PathStrokeType stroke;
    JucerFillType fill;

    //==============================================================================
    bool operator== (const StrokeType& other) const throw();
    bool operator!= (const StrokeType& other) const throw();


private:
    void reset();
};


#endif   // __JUCER_STROKETYPE_JUCEHEADER__
