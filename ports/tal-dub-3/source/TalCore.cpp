/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-7 by Raw Material Software ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the
   GNU General Public License, as published by the Free Software Foundation;
   either version 2 of the License, or (at your option) any later version.

   JUCE is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with JUCE; if not, visit www.gnu.org/licenses or write to the
   Free Software Foundation, Inc., 59 Temple Place, Suite 330,
   Boston, MA 02111-1307 USA

  ------------------------------------------------------------------------------

   If you'd like to release a closed-source product which uses JUCE, commercial
   licenses are also available: visit www.rawmaterialsoftware.com/juce for
   more information.

  ==============================================================================
*/

#include "includes.h"
#include "TalComponent.h"
#include "TalCore.h"
#include "ProgramChunk.h"
#include "Math.h"

/**
    This function must be implemented to create a new instance of your
    plugin object.
*/
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TalCore();
}

TalCore::TalCore()
{
	// init engine
	if (this->getSampleRate() > 0)
		sampleRate = (float)this->getSampleRate();
	else
		sampleRate = 44100.0f;

    currentBpm = 120;

	engine = new Engine(sampleRate);
	params = engine->param;
	talPresets = new TalPreset*[NUMPROGRAMS];

	for (int i = 0; i < NUMPROGRAMS; i++) talPresets[i] = new TalPreset();
	curProgram = 0;

	// load factory presets
	ProgramChunk *chunk = new ProgramChunk();
	XmlDocument myDocument (chunk->getXmlChunk());
	XmlElement* mainElement = myDocument.getDocumentElement();

	MemoryBlock* destData = new MemoryBlock();
	copyXmlToBinary(*mainElement, *destData);
	setStateInformation(destData->getData(), destData->getSize());
	setCurrentProgram(curProgram);

	// used for midi learn
	lastMovedController = 0;

	peakReductionValue[0] = 0.0f;
	peakReductionValue[1] = 0.0f;
}

TalCore::~TalCore()
{
	if (talPresets) delete[] talPresets;
	if (engine) delete engine;
}

const String TalCore::getName() const
{
    return "Tal-Filter";
}

int TalCore::getNumParameters()
{
	return (int)NUMPARAM;
}

float TalCore::getParameter (int index)
{
	if (index < NUMPARAM)
		return talPresets[curProgram]->programData[index];
	else
		return 0;
}

float* TalCore::getPeakReductionValue()
{
	return peakReductionValue;
}

void TalCore::setParameter (int index, float newValue)
{
	if (index < NUMPARAM)
	{
		params[index] = newValue;
		talPresets[curProgram]->programData[index] = newValue;

		switch(index)
		{
		case DRY:
			engine->setDry(newValue);
			break;
		case WET:
			engine->setWet(newValue);
			break;
		case INPUTDRIVE:
			engine->setInputDrive(newValue);
			break;

		case DELAYTWICE_R:
		case DELAYTWICE_L:
			engine->setDelay(talPresets[curProgram]->programData[DELAYTIME],
				(int)talPresets[curProgram]->programData[DELAYTIMESYNC],
				talPresets[curProgram]->programData[DELAYTWICE_L] > 0.0f,
				talPresets[curProgram]->programData[DELAYTWICE_R] > 0.0f,
				true);
			break;
		case DELAYTIME:
			engine->setDelay(newValue,
				(int)talPresets[curProgram]->programData[DELAYTIMESYNC],
				talPresets[curProgram]->programData[DELAYTWICE_L] > 0.0f,
				talPresets[curProgram]->programData[DELAYTWICE_R] > 0.0f,
				true);
			break;
		case FEEDBACK:
			engine->setFeedback(newValue);
			break;
		case RESONANCE:
			engine->setResonance(newValue);
			break;
		case CUTOFF:
			engine->setCutoff(newValue);
			break;
		case HIGHCUT:
			engine->setHighCut(newValue);
			break;
		case DELAYTIMESYNC:
			engine->setDelay(
				talPresets[curProgram]->programData[DELAYTIME],
				(int)newValue,
				talPresets[curProgram]->programData[DELAYTWICE_L] > 0.0f,
				talPresets[curProgram]->programData[DELAYTWICE_R] > 0.0f,
				true);
			break;
		}

		sendChangeMessage ();

		// for midi learn
		lastMovedController = index;
	}
}

const String TalCore::getParameterName (int index)
{
	switch(index)
	{
		case CUTOFF: return T("cutoff");
		case RESONANCE: return T("resonance");
		case INPUTDRIVE: return T("inputdrive");
		case DELAYTIME: return T("delaytime");
		case DELAYTIMESYNC: return T("delaytimesync");
		case DELAYTWICE_L: return T("delaytwice_l");
		case DELAYTWICE_R: return T("delaytwice_r");
		case FEEDBACK: return T("feedback");
		case HIGHCUT: return T("highcut");
		case DRY: return T("dry");
		case WET: return T("wet");
		case LIVEMODE: return T("livemode");
	}
    return String::empty;
}

const String TalCore::getParameterText (int index)
{
	if (index < NUMPARAM)
	{
		return String(talPresets[curProgram]->programData[index], 2);
	}
    return String::empty;
}

const String TalCore::getInputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

const String TalCore::getOutputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

bool TalCore::isInputChannelStereoPair (int index) const
{
    return true;
}

bool TalCore::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool TalCore::acceptsMidi() const
{
    return true;
}

bool TalCore::producesMidi() const
{
    return false;
}

void TalCore::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	//engine->clearBuffer();
}

void TalCore::releaseResources()
{
    // when playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
	//engine->clearBuffer();
}

void TalCore::processBlock (AudioSampleBuffer& buffer,
                                   MidiBuffer& midiMessages)
{
	// Change sample rate if required
	if (sampleRate != this->getSampleRate())
	{
		sampleRate = (float)this->getSampleRate();
		engine->setSampleRate(sampleRate);
		setCurrentProgram(curProgram);
	}
    AudioPlayHead::CurrentPositionInfo pos;
    if (getPlayHead() != 0 && getPlayHead()->getCurrentPosition (pos))
    {
        if ((float)pos.bpm != currentBpm)
        {
			currentBpm = (float)pos.bpm;
			engine->setBpm(
				(float)currentBpm,
				talPresets[curProgram]->programData[DELAYTIME],
				(int)talPresets[curProgram]->programData[DELAYTIMESYNC],
				talPresets[curProgram]->programData[DELAYTWICE_L] > 0.0f,
				talPresets[curProgram]->programData[DELAYTWICE_R] > 0.0f);
        }
    }
    const ScopedLock sl (this->getCallbackLock());

    // for each of our input channels, we'll attenuate its level by the
    // amount that our volume parameter is set to.
	int numberOfChannels = getNumInputChannels();
	int bufferSize = buffer.getNumSamples();

	// midi buffer
	MidiMessage controllerMidiMessage (0xF0);
    MidiBuffer::Iterator midiIterator(midiMessages);
	hasMoreMidiMessages = midiIterator.getNextEvent(controllerMidiMessage, midiEventPos);

	if (numberOfChannels == 2)
	{
		float *samples0 = buffer.getSampleData(0, 0);
		float *samples1 = buffer.getSampleData(1, 0);

		int samplePos = 0;
		int numSamples = buffer.getNumSamples();
		while (numSamples > 0)
		{
			processMidiPerSample(&midiIterator, controllerMidiMessage, samplePos);
			engine->process(samples0++, samples1++);

			numSamples--;
			samplePos++;
		}
	}
	if (numberOfChannels == 1)
	{
		float *samples0 = buffer.getSampleData(0, 0);
		float *samples1 = buffer.getSampleData(0, 0);

		int samplePos = 0;
		int numSamples = buffer.getNumSamples();
		while (numSamples > 0)
		{
			processMidiPerSample(&midiIterator, controllerMidiMessage, samplePos);
			engine->process(samples0++, samples1++);

			numSamples--;
			samplePos++;
		}
	}
	// update peak level
	peakReductionValue[0] = engine->getPeakReductionValueL();
	peakReductionValue[1] = engine->getPeakReductionValueR();

    // in case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
}

void TalCore::processMidiPerSample(MidiBuffer::Iterator *midiIterator, MidiMessage controllerMidiMessage, int samplePos)
{
	// There can be more than one event at the same position
	while (hasMoreMidiMessages && midiEventPos == samplePos)
	{
		if (controllerMidiMessage.isController())
		{
			handleController (controllerMidiMessage.getControllerNumber(),
							  controllerMidiMessage.getControllerValue());
		}
		hasMoreMidiMessages = midiIterator->getNextEvent (controllerMidiMessage, midiEventPos);
	}
}

void TalCore::handleController (const int controllerNumber,
								const int controllerValue)
{
	if (params[MIDILEARN] > 0.0f)
	{
		for (int i = 0; i < NUMPROGRAMS; i++)
		{
			talPresets[i]->midiMap[controllerNumber] = lastMovedController;
		}
	}
	if (talPresets[curProgram]->midiMap[controllerNumber] > 0)
	{
		setParameter (talPresets[curProgram]->midiMap[controllerNumber], controllerValue / 127.0f);
	}
}

AudioProcessorEditor* TalCore::createEditor()
{
	// Work around -> otherwise the combo box values will be overwritten
	//TalComponent *tal = new TalComponent (this);
	//sendChangeMessage (this);
    return new TalComponent (this);
}

void TalCore::getStateInformation (MemoryBlock& destData)
{
    // you can store your parameters as binary data if you want to or if you've got
    // a load of binary to put in there, but if you're not doing anything too heavy,
    // XML is a much cleaner way of doing it - here's an example of how to store your
    // params as XML..

	// header
	XmlElement tal("tal");
	tal.setAttribute (T("curprogram"), curProgram);
	tal.setAttribute (T("version"), 1);

	// programs
    XmlElement *programList = new XmlElement ("programs");
	for (int i = 0; i < NUMPROGRAMS; i++)
	{
		XmlElement* program = new XmlElement ("program");
		program->setAttribute (T("programname"), talPresets[i]->name);
		program->setAttribute (T("cutoff"), talPresets[i]->programData[CUTOFF]);
		program->setAttribute (T("resonance"), talPresets[i]->programData[RESONANCE]);
		program->setAttribute (T("inputdrive"), talPresets[i]->programData[INPUTDRIVE]);
		program->setAttribute (T("delaytime"), talPresets[i]->programData[DELAYTIME]);
		program->setAttribute (T("delaytimesync"), talPresets[i]->programData[DELAYTIMESYNC]);
		program->setAttribute (T("delaytwice_l"), talPresets[i]->programData[DELAYTWICE_L]);
		program->setAttribute (T("delaytwice_r"), talPresets[i]->programData[DELAYTWICE_R]);
		program->setAttribute (T("feedback"), talPresets[i]->programData[FEEDBACK]);
		program->setAttribute (T("highcut"), talPresets[i]->programData[HIGHCUT]);
		program->setAttribute (T("dry"), talPresets[i]->programData[DRY]);
		program->setAttribute (T("wet"), talPresets[i]->programData[WET]);
		program->setAttribute (T("livemode"), talPresets[i]->programData[LIVEMODE]);
		programList->addChildElement(program);
	}
	tal.addChildElement(programList);

	// midi params
	XmlElement *midiMapList = new XmlElement ("midimap");
	for (int i = 0; i < 255; i++)
	{
		if (talPresets[0]->midiMap[i] != 0)
		{
			XmlElement* map = new XmlElement ("map");
			map->setAttribute (T("param"), talPresets[0]->midiMap[i]);
			map->setAttribute (T("controllernumber"), i);
			midiMapList->addChildElement(map);
		}
	}
	tal.addChildElement(midiMapList);

	// Reset midi learn
	setParameter(MIDILEARN, 0.0f);
	//sendChangeMessage (this);

    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (tal, destData);

	// use this for new factory presets
	//#ifdef _DEBUG && WIN32
	//File *file = new File("d:/presets.txt");
	//String myXmlDoc = tal.createDocument (String::empty);
	//file->replaceWithText(myXmlDoc);
	//#endif
}

void TalCore::setStateInformation (const void* data, int sizeInBytes)
{
	// use this helper function to get the XML from this binary blob..
    XmlElement* const xmlState = getXmlFromBinary (data, sizeInBytes);

	curProgram = 0;
	if (xmlState != 0 && xmlState->hasTagName(T("tal")))
	{
		curProgram = (int)xmlState->getIntAttribute (T("curprogram"), 1);
		XmlElement* programs = xmlState->getFirstChildElement();
		if (programs->hasTagName(T("programs")))
		{
			int i = 0;
			forEachXmlChildElement (*programs, e)
			{
				if (e->hasTagName(T("program")) && i < NUMPROGRAMS)
				{
					talPresets[i]->name = e->getStringAttribute (T("programname"), T("Not Saved"));
					talPresets[i]->programData[CUTOFF] = (float) e->getDoubleAttribute (T("cutoff"), 0.8f);
					talPresets[i]->programData[RESONANCE] = (float) e->getDoubleAttribute (T("resonance"), 0.8f);
					talPresets[i]->programData[INPUTDRIVE] = (float) e->getDoubleAttribute (T("inputdrive"), 0.8f);
					talPresets[i]->programData[DELAYTIME] = (float) e->getDoubleAttribute (T("delaytime"), 0.8f);
					talPresets[i]->programData[DELAYTIMESYNC] = (float) e->getDoubleAttribute (T("delaytimesync"), 1.0f);
					talPresets[i]->programData[DELAYTWICE_L] = (float) e->getDoubleAttribute (T("delaytwice_l"), 0.8f);
					talPresets[i]->programData[DELAYTWICE_R] = (float) e->getDoubleAttribute (T("delaytwice_r"), 0.8f);
					talPresets[i]->programData[FEEDBACK] = (float) e->getDoubleAttribute (T("feedback"), 0.8f);
					talPresets[i]->programData[HIGHCUT] = (float) e->getDoubleAttribute (T("highcut"), 0.8f);
					talPresets[i]->programData[DRY] = (float) e->getDoubleAttribute (T("dry"), 0.8f);
					talPresets[i]->programData[WET] = (float) e->getDoubleAttribute (T("wet"), 0.8f);
					talPresets[i]->programData[LIVEMODE] = (float) e->getDoubleAttribute (T("livemode"), 0.0f);
					i++;
				}
			}
		}
		// restore midi mapping
		XmlElement* midiMap = xmlState->getChildByName(T("midimap"));
		if (midiMap != 0 && midiMap->hasTagName(T("midimap")))
		{
			forEachXmlChildElement (*midiMap, e)
			{
				for (int j = 0; j < NUMPROGRAMS; j++)
				{
					int controller = e->getIntAttribute(T("controllernumber"), 0);
					if (controller < 255 && controller > 0)
					{
						talPresets[j]->midiMap[controller] = e->getIntAttribute(T("param"), 0);
					}
				}
			}
		}
		delete xmlState;
		setCurrentProgram(curProgram);
		sendChangeMessage ();
	}
}

//==============================================================================
String TalCore::getStateInformationString ()
{
    // header
    XmlElement tal("tal");
    tal.setAttribute (T("curprogram"), curProgram);
    tal.setAttribute (T("version"), 1);

    // programs
    XmlElement *programList = new XmlElement ("programs");

    for (int i = 0; i < NUMPROGRAMS; i++)
    {
            XmlElement* program = new XmlElement ("program");
            program->setAttribute (T("programname"), talPresets[i]->name);
            program->setAttribute (T("cutoff"), talPresets[i]->programData[CUTOFF]);
            program->setAttribute (T("resonance"), talPresets[i]->programData[RESONANCE]);
            program->setAttribute (T("inputdrive"), talPresets[i]->programData[INPUTDRIVE]);
            program->setAttribute (T("delaytime"), talPresets[i]->programData[DELAYTIME]);
            program->setAttribute (T("delaytimesync"), talPresets[i]->programData[DELAYTIMESYNC]);
            program->setAttribute (T("delaytwice_l"), talPresets[i]->programData[DELAYTWICE_L]);
            program->setAttribute (T("delaytwice_r"), talPresets[i]->programData[DELAYTWICE_R]);
            program->setAttribute (T("feedback"), talPresets[i]->programData[FEEDBACK]);
            program->setAttribute (T("highcut"), talPresets[i]->programData[HIGHCUT]);
            program->setAttribute (T("dry"), talPresets[i]->programData[DRY]);
            program->setAttribute (T("wet"), talPresets[i]->programData[WET]);
            program->setAttribute (T("livemode"), talPresets[i]->programData[LIVEMODE]);
            programList->addChildElement(program);
    }
    tal.addChildElement(programList);

    // midi params
    XmlElement *midiMapList = new XmlElement ("midimap");
    for (int i = 0; i < 255; i++)
    {
            if (talPresets[0]->midiMap[i] != 0)
            {
                    XmlElement* map = new XmlElement ("map");
                    map->setAttribute (T("param"), talPresets[0]->midiMap[i]);
                    map->setAttribute (T("controllernumber"), i);
                    midiMapList->addChildElement(map);
            }
    }
    tal.addChildElement(midiMapList);

    // Reset midi learn
    setParameter(MIDILEARN, 0.0f);

    return tal.createDocument (String::empty);
}

void TalCore::setStateInformationString (const String& data)
{
    XmlElement* const xmlState = XmlDocument::parse(data);

    curProgram = 0;
    if (xmlState != 0 && xmlState->hasTagName(T("tal")))
    {
            curProgram = (int)xmlState->getIntAttribute (T("curprogram"), 1);
            XmlElement* programs = xmlState->getFirstChildElement();
            if (programs->hasTagName(T("programs")))
            {
                    int i = 0;
                    forEachXmlChildElement (*programs, e)
                    {
                            if (e->hasTagName(T("program")) && i < NUMPROGRAMS)
                            {
                                    talPresets[i]->name = e->getStringAttribute (T("programname"), T("Not Saved"));
                                    talPresets[i]->programData[CUTOFF] = (float) e->getDoubleAttribute (T("cutoff"), 0.8f);
                                    talPresets[i]->programData[RESONANCE] = (float) e->getDoubleAttribute (T("resonance"), 0.8f);
                                    talPresets[i]->programData[INPUTDRIVE] = (float) e->getDoubleAttribute (T("inputdrive"), 0.8f);
                                    talPresets[i]->programData[DELAYTIME] = (float) e->getDoubleAttribute (T("delaytime"), 0.8f);
                                    talPresets[i]->programData[DELAYTIMESYNC] = (float) e->getDoubleAttribute (T("delaytimesync"), 1.0f);
                                    talPresets[i]->programData[DELAYTWICE_L] = (float) e->getDoubleAttribute (T("delaytwice_l"), 0.8f);
                                    talPresets[i]->programData[DELAYTWICE_R] = (float) e->getDoubleAttribute (T("delaytwice_r"), 0.8f);
                                    talPresets[i]->programData[FEEDBACK] = (float) e->getDoubleAttribute (T("feedback"), 0.8f);
                                    talPresets[i]->programData[HIGHCUT] = (float) e->getDoubleAttribute (T("highcut"), 0.8f);
                                    talPresets[i]->programData[DRY] = (float) e->getDoubleAttribute (T("dry"), 0.8f);
                                    talPresets[i]->programData[WET] = (float) e->getDoubleAttribute (T("wet"), 0.8f);
                                    talPresets[i]->programData[LIVEMODE] = (float) e->getDoubleAttribute (T("livemode"), 0.0f);
                                    i++;
                            }
                    }
            }
            // restore midi mapping
            XmlElement* midiMap = xmlState->getChildByName(T("midimap"));
            if (midiMap != 0 && midiMap->hasTagName(T("midimap")))
            {
                    forEachXmlChildElement (*midiMap, e)
                    {
                            for (int j = 0; j < NUMPROGRAMS; j++)
                            {
                                    int controller = e->getIntAttribute(T("controllernumber"), 0);
                                    if (controller < 255 && controller > 0)
                                    {
                                            talPresets[j]->midiMap[controller] = e->getIntAttribute(T("param"), 0);
                                    }
                            }
                    }
            }
            delete xmlState;
            setCurrentProgram(curProgram);
            sendChangeMessage ();
    }
}

int TalCore::getNumPrograms ()
{
	return NUMPROGRAMS;
}

int TalCore::getCurrentProgram ()
{
	return curProgram;
}

void TalCore::setCurrentProgram (int index)
{
	if (index < NUMPROGRAMS)
	{
		curProgram = index;
		for (int i = 0; i < NUMPARAM; i++)
		{
			setParameter(i, talPresets[index]->programData[i]);
		}
		sendChangeMessage ();
	}
}

const String TalCore::getProgramName (int index)
{
	if (index < NUMPROGRAMS)
		return talPresets[index]->name;
	else
		return T("Invalid");
}

void TalCore::changeProgramName (int index, const String& newName)
{
	if (index < NUMPROGRAMS)
		talPresets[index]->name = newName;
}
