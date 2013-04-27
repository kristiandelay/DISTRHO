// ==================================================================================
// Copyright (c) 2012 HiFi-LoFi
//
// This is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// ==================================================================================

#include "WaveformComponent.h"

#include "CustomLookAndFeel.h"
#include "../DecibelScaling.h"
#include "../Processor.h"

 
WaveformComponent::WaveformComponent() :
  Component(),
  _irAgent(nullptr),
  _maximaDecibels(),
  _irFingerprint(0),
  _sampleRate(0.0),
  _samplesPerPx(0),
  _pxPerDecibel(0),
  _predelayMs(0.0),
  _predelayOffsetX(0),
  _area(),
  _envelope(1.0, 1.0),
  _dragDistance(5.0),
  _indexHighlighted(std::numeric_limits<size_t>::max()),
  _indexDragged(std::numeric_limits<size_t>::max()),
  _dragStartX(-1.0),
  _dragStartY(-1.0),
  _beatsPerMinute(0.0f)
{
}


WaveformComponent::~WaveformComponent()
{
}


void WaveformComponent::resized()
{
  updateArea();
}


void WaveformComponent::updateArea()
{
  const CustomLookAndFeel& customLookAndFeel = CustomLookAndFeel::GetCustomLookAndFeel(this);
  const juce::Font scaleFont = customLookAndFeel.getScaleFont();

  const int marginTop = 4;
  const int widthDbScale = scaleFont.getStringWidth("-XXdB") + 4;
  const int heightTimeLine = static_cast<int>(::ceil(scaleFont.getHeight())) + 5;
  
  _area.setX(widthDbScale);
  _area.setY(marginTop);
  _area.setWidth(static_cast<int>(_maximaDecibels.size()));
  _area.setHeight(getHeight() - (heightTimeLine + marginTop + 1));
  _pxPerDecibel = ::fabs(static_cast<float>(_area.getHeight()) / DecibelScaling::MinScaleDb());
  
  repaint();
}


void WaveformComponent::paint(Graphics& g)
{
  const int width = getWidth();
  const int height = getHeight();
  
  const CustomLookAndFeel& customLookAndFeel = CustomLookAndFeel::GetCustomLookAndFeel(this);
  const juce::Font scaleFont = customLookAndFeel.getScaleFont();
  const juce::Colour scaleColour = customLookAndFeel.getScaleColour();
  
  // Something to paint?
  if (_maximaDecibels.empty() || !_irAgent)
  {
    g.setColour(scaleColour);
    g.drawText("No Impulse Response", 0, 0, width, height, Justification(Justification::centred), false);
    return;
  }

  const float w = static_cast<float>(width);
  
  // Background
  g.setColour(customLookAndFeel.getWaveformBackgroundColour());
  g.fillRect(_area);
  
  // Timeline
  const double secondsPerPx = _samplesPerPx / _sampleRate;
  {
    const float yTime = static_cast<float>(_area.getBottom());
    const int hTick = static_cast<int>(yTime) - 2;
    g.setFont(scaleFont);
    g.setColour(scaleColour);
    g.drawHorizontalLine(static_cast<int>(yTime), static_cast<float>(_area.getX()), w);
    if (_beatsPerMinute > 0.0001f && _irAgent->getProcessor().getSettings().getTimelineUnit() == Settings::Beats)
    {
      const int minPxPerTick = 10;
      int beatPerTick = 16; // Start with 16th as finest
      float pxPerTick = ((60.0f / 4.0f) / _beatsPerMinute) * static_cast<float>(1.0 / secondsPerPx);
      while (pxPerTick < minPxPerTick && beatPerTick > 1)
      {
        beatPerTick /= 2;
        pxPerTick *= 2.0f;
      }
      if (beatPerTick == 2) // Don't display half note beats...
      {
        beatPerTick /= 2;
        pxPerTick *= 2.0f;
      }
      const juce::Justification tickJustification(Justification::topLeft);
      const float tickTop = yTime;
      const float tickBottom = tickTop + 4.0f;
      for (float xTick=0.0; xTick<static_cast<float>(width); xTick+=pxPerTick)
      {
        g.drawVerticalLine(_area.getX() + static_cast<int>(xTick), tickTop, tickBottom);             
      }
      g.drawText(juce::String(_beatsPerMinute, 1) + juce::String(" BPM (1/") + juce::String(beatPerTick) + juce::String(" notes)"),
                 _area.getX(),
                 static_cast<int>(tickBottom)-1,
                 _area.getWidth(),
                 hTick,
                 tickJustification,
                 false);
    }
    else
    { 
      // Seconds
      const int minTickWidth = 2 * scaleFont.getStringWidth("XX.XXs");
      double secondsPerTick = minTickWidth * secondsPerPx;
      double nextPowerOf10 = 0.1;
      while (nextPowerOf10 < secondsPerTick)
      {
        nextPowerOf10 *= 10.0;
      }
      double nextPowerOf10_2 = nextPowerOf10 / 2.0;
      double nextPowerOf10_4 = nextPowerOf10 / 4.0;
      if (secondsPerTick <= nextPowerOf10_4)
      {
        secondsPerTick = nextPowerOf10_4;
      }
      else if (secondsPerTick <= nextPowerOf10_2)
      {
        secondsPerTick = nextPowerOf10_2;
      }
      else
      {
        secondsPerTick = nextPowerOf10;
      }
      const double tickWidth = secondsPerTick / secondsPerPx;
      const Justification tickJustification(Justification::topLeft);
      const float tickTop = yTime;
      const float tickBottom = tickTop + 4.0f;
      double secTick = 0.0;
      for (double xTick=0.0; xTick<static_cast<double>(width); xTick+=tickWidth)
      {
        const int xTickPos = static_cast<int>(xTick);
        g.drawVerticalLine(_area.getX() + xTickPos, tickTop, tickBottom);      
        g.drawText(String(secTick, 2) + String("s"),
                   _area.getX() + xTickPos + 1,
                   static_cast<int>(tickBottom)-1,
                   minTickWidth,
                   hTick,
                   tickJustification,
                   false);
        secTick += secondsPerTick;
      }
    }
  }
  
  // Decibel scale
  {
    g.setFont(scaleFont);
    g.setColour(scaleColour);
    
    const int scaleTextWidth = scaleFont.getStringWidth("-XXdB");
    const int scaleTextHeight = static_cast<int>(::ceil(scaleFont.getHeight()));

    const juce::Justification justificationTopRight(juce::Justification::topRight);

    g.drawVerticalLine(_area.getX(), static_cast<float>(_area.getY()), static_cast<float>(_area.getBottom()));    
    const float tickLeft = static_cast<float>(_area.getX() - 4);
    const float tickRight = static_cast<float>(_area.getX());
    const int steps[] = { 0, -20, -40, -60, -80 };
    for (size_t i=0; i<5; ++i)
    {
      const int db = steps[i];
      const int yTick = static_cast<int>(static_cast<float>(_area.getY()) + (static_cast<float>(::abs(db)) * _pxPerDecibel));
      g.drawHorizontalLine(yTick, tickLeft, tickRight);
      g.drawText(DecibelScaling::DecibelString(db),
                 0,
                 yTick + 1,
                 scaleTextWidth,
                 scaleTextHeight,
                 justificationTopRight,
                 false);      
    }
    g.drawHorizontalLine(_area.getBottom(), tickLeft, tickRight);
  }
  
  // Waveform
  const size_t xLen = std::min(static_cast<size_t>(w), _maximaDecibels.size());
  const float bottom = static_cast<float>(_area.getBottom());
  g.setColour(customLookAndFeel.getWaveformColour());

  for (size_t x=0; x<xLen; ++x)
  {
    const float top = bottom - (_pxPerDecibel * (_maximaDecibels[x]-DecibelScaling::MinScaleDb()));
    g.drawVerticalLine(static_cast<int>(x)+_area.getX()+1, top, bottom);
  }
  g.setColour(Colours::darkgrey);
  g.drawVerticalLine(_area.getRight(),
                     static_cast<float>(_area.getY()),
                     static_cast<float>(_area.getBottom()));
  
  // Predelay
  if (_predelayOffsetX > 0)
  {
    g.setColour(Colours::grey);
    g.drawVerticalLine(_area.getX()+_predelayOffsetX,
                       static_cast<float>(_area.getY()),
                       static_cast<float>(_area.getBottom()));
  }
  
  // Envelope
  const size_t nodeCount = _envelope.getNodeCount();
  if (nodeCount > 0)
  {
    Path path;
    path.startNewSubPath(static_cast<float>(_area.getX()+_predelayOffsetX), static_cast<float>(_area.getY()));
    for (size_t i=0; i<nodeCount; ++i)
    {
      const float x = static_cast<float>(calcEnvelopePosX(_envelope.getX(i)));
      const float y = static_cast<float>(calcEnvelopePosY(_envelope.getY(i)));
      path.lineTo(x, y);
    }
    path.lineTo(static_cast<float>(_area.getRight()), static_cast<float>(_area.getBottom()));
    path.lineTo(static_cast<float>(_area.getRight()), static_cast<float>(_area.getY()));
    path.closeSubPath();
    DrawablePath drawable;
    drawable.setPath(path);
    drawable.setFill(FillType(customLookAndFeel.getEnvelopeRestrictionColour()));
    drawable.draw(g, 1.0f);
  }
  for (size_t i=0; i<nodeCount; ++i)
  {
    const float nodeSize = 8.0f;
    const float nodeSize2 = 0.5f * nodeSize;
    const float posX = static_cast<float>(calcEnvelopePosX(_envelope.getX(i)));
    const float posY = static_cast<float>(calcEnvelopePosY(_envelope.getY(i)));
    g.setColour(customLookAndFeel.getEnvelopeNodeColour(i == _indexHighlighted));
    g.fillEllipse(posX-nodeSize2, posY-nodeSize2, nodeSize, nodeSize);
  }
}


void WaveformComponent::init(IRAgent* irAgent, double sampleRate, size_t samplesPerPx)
{
  FloatBuffer::Ptr ir;
  Processor* processor = nullptr;
  double predelayMs = 0.0;
  float beatsPerMinute = 0.0f;
  if (irAgent)
  {
    ir = irAgent->getImpulseResponse();
    processor = &irAgent->getProcessor();
    predelayMs = processor->getPredelayMs();
    beatsPerMinute = processor->getBeatsPerMinute();
  }
  
  // Let's rely here on the immutability of the IR buffer (i.e. there's always
  // a new allocated buffer if the IR changes, so we can check for changes of the IR
  // just by comparing a kind of "fingerprint for the poor" consisting of the pointer
  // and its length in order to prevent unnecessary recalculations of the maxima for
  // the waveform which is quite expensive)
  size_t irFingerprint = 0;
  if (ir && ir->getSize() > 0)
  {
    irFingerprint = reinterpret_cast<size_t>(ir.get()) ^ ir->getSize();
  }
                                                                            
  if (_irAgent != irAgent ||
      ::fabs(_sampleRate-sampleRate) < 0.00001 ||
      ::fabs(_predelayMs-predelayMs) < 0.00001 ||
      ::fabs(_beatsPerMinute-beatsPerMinute) < 0.00001f ||
      _samplesPerPx != samplesPerPx ||
      _irFingerprint != irFingerprint)
  { 
    _irAgent = irAgent;
    _sampleRate = sampleRate;
    _samplesPerPx = std::max(static_cast<size_t>(1), samplesPerPx);
    _predelayMs = predelayMs;
    _predelayOffsetX = static_cast<int>((sampleRate / 1000.0) * _predelayMs) / _samplesPerPx;
    _beatsPerMinute = beatsPerMinute;
    
    if (processor)
    {
      _envelope = processor->getEnvelope();
    }
    else
    {
      _envelope.reset();
    }

    if (_irFingerprint != irFingerprint)
    {
      _irFingerprint = irFingerprint;
      _maximaDecibels.clear();
      if (ir)
      {
        const float* data = ir->data();
        const size_t len = ir->getSize();
        _maximaDecibels.reserve(len/samplesPerPx + 1);
        size_t processed = 0;
        while (processed < len)
        {
          const size_t processing = std::min(_samplesPerPx, len-processed);
          float maximum = 0.0f;
          const float* block = data + processed;
          for (size_t i=0; i<processing; ++i)
          {
            const float current = ::fabs(block[i]);
            if (current > maximum)
            {
              maximum = current;
            }
          }
          const float db = std::min(0.0f, std::max(DecibelScaling::MinScaleDb(), DecibelScaling::Gain2Db(maximum)));
          _maximaDecibels.push_back(db);
          processed += _samplesPerPx;
        }
      }
    }
  }
  updateArea();
}


void WaveformComponent::clear()
{
  if (_irAgent || !_maximaDecibels.empty() || _sampleRate > 0.0000001 || _samplesPerPx > 0)
  {
    _irAgent = nullptr;
    _maximaDecibels.clear();
    _irFingerprint = 0;
    _sampleRate = 0.0;
    _samplesPerPx = 0;
    _predelayMs = 0.0;
    _envelope.reset();
    updateArea();
  }
}


void WaveformComponent::mouseUp(const MouseEvent& mouseEvent)
{
  if (_indexDragged < _envelope.getNodeCount())
  {
    _indexDragged = std::numeric_limits<size_t>::max();
  }
  else if (_irAgent)
  {
    if (mouseEvent.x > _area.getX() && mouseEvent.y > _area.getBottom())
    {
      Settings& settings = _irAgent->getProcessor().getSettings();
      settings.setTimelineUnit((settings.getTimelineUnit() == Settings::Seconds) ? Settings::Beats : Settings::Seconds);
      repaint();
    }
  }
}


void WaveformComponent::mouseDoubleClick(const MouseEvent& mouseEvent)
{
  if (_area.contains(mouseEvent.x, mouseEvent.y))
  {
    size_t index = getEnvelopeNode(mouseEvent.x, mouseEvent.y);
    if (index < _envelope.getNodeCount())
    {
      _envelope.removeNode(index);
      _indexHighlighted = std::numeric_limits<size_t>::max();
    }
    else
    {
      const double x = calcEnvelopeValueX(mouseEvent.x);
      const double y = calcEnvelopeValueY(mouseEvent.y);
      _indexHighlighted = _envelope.insertNode(x, y);
    }
    repaint();
    envelopeChanged();
  }
}


void WaveformComponent::mouseDown(const MouseEvent& mouseEvent)
{
  const size_t index = getEnvelopeNode(mouseEvent.x, mouseEvent.y);
  if (index < _envelope.getNodeCount())
  {
    _indexDragged = index;
    _dragStartX = _envelope.getX(index);
    _dragStartY = _envelope.getY(index);
  }
}


void WaveformComponent::mouseDrag(const MouseEvent& mouseEvent)
{
  if (_indexDragged < _envelope.getNodeCount())
  {
    const double x = _dragStartX + (calcEnvelopeValueX(mouseEvent.x) - _dragStartX);
    const double y = _dragStartY + (calcEnvelopeValueY(mouseEvent.y) - _dragStartY);
    _envelope.setX(_indexDragged, x);
    _envelope.setY(_indexDragged, y);
    repaint();
    envelopeChanged();
  }
}


void WaveformComponent::mouseMove(const MouseEvent& mouseEvent)
{
  const size_t highlightedBefore = _indexHighlighted;
  _indexHighlighted = getEnvelopeNode(mouseEvent.x, mouseEvent.y);
  if (highlightedBefore != _indexHighlighted)
  {
    repaint();
  }
}


void WaveformComponent::mouseExit(const MouseEvent&)
{
  _indexDragged = std::numeric_limits<size_t>::max();
  if (_indexHighlighted < _envelope.getNodeCount())
  {
    _indexHighlighted = std::numeric_limits<size_t>::max();
    repaint();
  }
}


size_t WaveformComponent::getEnvelopeNode(int posX, int posY) const
{
  size_t nearest = std::numeric_limits<size_t>::max();
  double nearestDist = std::numeric_limits<double>::max();
  for (size_t i=0; i<_envelope.getNodeCount(); ++i)
  {
    const double distX = posX - calcEnvelopePosX(_envelope.getX(i));
    const double distY = posY - calcEnvelopePosY(_envelope.getY(i));
    const double dist = ::sqrt(distX*distX + distY*distY);
    if (dist <= _dragDistance && dist < nearestDist)
    {
      nearest = i;
      nearestDist = dist;
    }
  }
  return nearest;
}


int WaveformComponent::calcEnvelopePosX(double x) const
{
  return _area.getX() + static_cast<int>(x * static_cast<double>(_area.getWidth()-_predelayOffsetX)) + _predelayOffsetX;
}


int WaveformComponent::calcEnvelopePosY(double y) const
{
  const double db = Decibels::gainToDecibels(y);
  return _area.getBottom() - static_cast<int>(_pxPerDecibel * (db-DecibelScaling::MinScaleDb()));
}


double WaveformComponent::calcEnvelopeValueX(int posX) const
{
  const double x = static_cast<double>((posX-_area.getX())-_predelayOffsetX) / static_cast<double>(_area.getWidth()-_predelayOffsetX);
  return std::min(1.0, std::max(0.0, x));
}


double WaveformComponent::calcEnvelopeValueY(int posY) const
{
  const double db = DecibelScaling::MinScaleDb() + (static_cast<double>(_area.getBottom()-posY) / _pxPerDecibel);
  const double y = static_cast<double>(DecibelScaling::Db2Gain(static_cast<float>(db)));
  return std::min(1.0, std::max(0.0, y));
}


void WaveformComponent::envelopeChanged()
{
  if (_irAgent)
  {
    _irAgent->getProcessor().setEnvelope(_envelope);
  }
}