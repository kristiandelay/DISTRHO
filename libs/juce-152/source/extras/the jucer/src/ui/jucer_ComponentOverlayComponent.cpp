/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-10 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

#include "../jucer_Headers.h"
#include "jucer_ComponentLayoutEditor.h"


//==============================================================================
ComponentOverlayComponent::ComponentOverlayComponent (Component* const target_,
                                                      ComponentLayout& layout_)
    : target (target_),
      borderThickness (4),
      layout (layout_),
      selected (false),
      dragging (false),
      originalAspectRatio (1.0)
{
    setMinimumOnscreenAmounts (0, 0, 0, 0);
    setSizeLimits (borderThickness * 2 + 2, borderThickness * 2 + 2, 8192, 8192);

    addChildComponent (border = new ResizableBorderComponent (this, this));

    border->setBorderThickness (BorderSize (borderThickness));

    target->addComponentListener (this);

    changeListenerCallback (0);
    layout.getSelectedSet().addChangeListener (this);

    setRepaintsOnMouseActivity (true);
    border->setRepaintsOnMouseActivity (true);
}

ComponentOverlayComponent::~ComponentOverlayComponent()
{
    layout.getSelectedSet().removeChangeListener (this);

    if (target != 0)
        target->removeComponentListener (this);

    delete border;
}

void ComponentOverlayComponent::changeListenerCallback (ChangeBroadcaster*)
{
    const bool nowSelected = layout.getSelectedSet().isSelected (target);

    if (selected != nowSelected)
    {
        selected = nowSelected;
        border->setVisible (nowSelected);
        repaint();
    }
}

void ComponentOverlayComponent::paint (Graphics& g)
{
    jassert (target != 0);

    if (selected)
    {
        const BorderSize borderSize (border->getBorderThickness());

        drawResizableBorder (g, getWidth(), getHeight(), borderSize, (isMouseOverOrDragging() || border->isMouseOverOrDragging()));
    }
    else if (isMouseOverOrDragging())
    {
        drawMouseOverCorners (g, getWidth(), getHeight());
    }
}

void ComponentOverlayComponent::resized()
{
    jassert (target != 0);

    border->setBounds (0, 0, getWidth(), getHeight());
}

void ComponentOverlayComponent::mouseDown (const MouseEvent& e)
{
    dragging = false;
    mouseDownSelectStatus = layout.getSelectedSet().addToSelectionOnMouseDown (target, e.mods);

    if (e.mods.isPopupMenu())
    {
        showPopupMenu();
        return; // this may be deleted now..
    }
}

void ComponentOverlayComponent::mouseDrag (const MouseEvent& e)
{
    if (! e.mods.isPopupMenu())
    {
        if (selected && ! dragging)
        {
            dragging = ! e.mouseWasClicked();

            if (dragging)
                layout.startDragging();
        }

        if (dragging)
        {
            layout.dragSelectedComps (e.getDistanceFromDragStartX(),
                                      e.getDistanceFromDragStartY());
        }
    }
}

void ComponentOverlayComponent::mouseUp (const MouseEvent& e)
{
    if (dragging)
        layout.endDragging();

    layout.getSelectedSet().addToSelectionOnMouseUp (target, e.mods, dragging, mouseDownSelectStatus);
}

void ComponentOverlayComponent::componentMovedOrResized (Component& component, bool wasMoved, bool wasResized)
{
    updateBoundsToMatchTarget();
}

void ComponentOverlayComponent::updateBoundsToMatchTarget()
{
    Component* const parent = target->getParentComponent();
    jassert (parent != 0);

    if (parent != 0)
    {
        const int dx = parent->getX();
        const int dy = parent->getY();

        setBounds (dx + target->getX() - borderThickness,
                   dy + target->getY() - borderThickness,
                   target->getWidth() + borderThickness * 2,
                   target->getHeight() + borderThickness * 2);
    }

    if (border->isMouseButtonDown())
        layout.changed();
}

void ComponentOverlayComponent::resizeStart()
{
    if (getHeight() > 0)
        originalAspectRatio = getWidth() / (double) getHeight();
    else
        originalAspectRatio = 1.0;

    layout.getDocument()->getUndoManager().beginNewTransaction (T("Resize components"));
}

void ComponentOverlayComponent::resizeEnd()
{
    layout.getDocument()->getUndoManager().beginNewTransaction();
}

void ComponentOverlayComponent::checkBounds (Rectangle<int>& bounds,
                                             const Rectangle<int>& previousBounds,
                                             const Rectangle<int>& limits,
                                             const bool isStretchingTop,
                                             const bool isStretchingLeft,
                                             const bool isStretchingBottom,
                                             const bool isStretchingRight)
{
    if (ModifierKeys::getCurrentModifiers().isShiftDown())
        setFixedAspectRatio (originalAspectRatio);
    else
        setFixedAspectRatio (0.0);

    ComponentBoundsConstrainer::checkBounds (bounds, previousBounds, limits, isStretchingTop, isStretchingLeft, isStretchingBottom, isStretchingRight);

    if (layout.getDocument()->isSnapActive (true))
    {
        Component* const parent = target->getParentComponent();
        jassert (parent != 0);

        if (parent != 0)
        {
            const int dx = parent->getX();
            const int dy = parent->getY();

            int x = bounds.getX();
            int y = bounds.getY();
            int w = bounds.getWidth();
            int h = bounds.getHeight();

            x += borderThickness - dx;
            y += borderThickness - dy;
            w -= borderThickness * 2;
            h -= borderThickness * 2;

            int right = x + w;
            int bottom = y + h;

            if (isStretchingRight)
                right = layout.getDocument()->snapPosition (right);

            if (isStretchingBottom)
                bottom = layout.getDocument()->snapPosition (bottom);

            if (isStretchingLeft)
                x = layout.getDocument()->snapPosition (x);

            if (isStretchingTop)
                y = layout.getDocument()->snapPosition (y);

            w = (right - x) + borderThickness * 2;
            h = (bottom - y) + borderThickness * 2;
            x -= borderThickness - dx;
            y -= borderThickness - dy;

            bounds = Rectangle<int> (x, y, w, h);
        }
    }
}

void ComponentOverlayComponent::applyBoundsToComponent (Component* component, const Rectangle<int>& bounds)
{
    if (component->getBounds() != bounds)
    {
        layout.getDocument()->getUndoManager().undoCurrentTransactionOnly();

        component->setBounds (bounds);

        Component* const parent = target->getParentComponent();
        jassert (parent != 0);

        if (parent != 0)
        {
            target->setBounds (bounds.getX() + borderThickness - parent->getX(),
                               bounds.getY() + borderThickness - parent->getY(),
                               bounds.getWidth() - borderThickness * 2,
                               bounds.getHeight() - borderThickness * 2);
        }

        layout.updateStoredComponentPosition (target, true);
    }
}

void ComponentOverlayComponent::showPopupMenu()
{
    ComponentTypeHandler::getHandlerFor (*target)->showPopupMenu (target, layout);
}
