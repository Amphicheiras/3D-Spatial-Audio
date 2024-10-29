#include "3DAudioPlugin/XYPad.h"

namespace juce::Gui
{
    /*
     * XY Pad Thumb section
     */
    XYPad::Thumb::Thumb()
    {
        constrainer.setMinimumOnscreenAmounts(thumbSize, thumbSize, thumbSize, thumbSize);
    }

    void XYPad::Thumb::paint(Graphics &g)
    {
        g.setColour(Colours::white);
        g.fillEllipse(getLocalBounds().toFloat());
    }

    void XYPad::Thumb::mouseDown(const MouseEvent &event)
    {
        dragger.startDraggingComponent(this, event);
    }

    void XYPad::Thumb::mouseDrag(const MouseEvent &event)
    {
        dragger.dragComponent(this, event, &constrainer);
        if (moveCallback)
            moveCallback(getPosition().toDouble());
    }

    /*
     * XY Pad section
     */
    XYPad::XYPad()
    {
        addAndMakeVisible(thumb);
        thumb.moveCallback = [&](Point<double> position)
        {
            const std::lock_guard<std::mutex> lock(vectorMutex);
            const auto bounds = getLocalBounds().toDouble();
            const auto w = static_cast<double>(thumbSize);
            for (auto *slider : xSliders)
            {
                slider->setValue(jmap(position.getX(), 0.0, bounds.getWidth() - w, slider->getMinimum(), slider->getMaximum()));
            }
            for (auto *slider : ySliders)
            {
                slider->setValue(jmap(position.getY(), bounds.getHeight() - w, 0.0, slider->getMinimum(), slider->getMaximum()));
            }
        };
    }

    void XYPad::resized()
    {
        thumb.setBounds(getLocalBounds().withSizeKeepingCentre(thumbSize, thumbSize));
        if (!xSliders.empty())
            sliderValueChanged(xSliders[0]);
        if (!ySliders.empty())
            sliderValueChanged(ySliders[0]);
    }

    void XYPad::paint(Graphics &g)
    {
        g.setColour(Colours::black);
        g.fillRoundedRectangle(getLocalBounds().toFloat(), 10.f);
    }

    void XYPad::registerSlider(Slider *slider, Axis axis)
    {
        slider->addListener(this);
        const std::lock_guard<std::mutex> lock(vectorMutex);
        if (axis == Axis::X)
            xSliders.push_back(slider);
        if (axis == Axis::Y)
            ySliders.push_back(slider);
    }

    void XYPad::deregisterSlider(Slider *slider)
    {
        slider->removeListener(this);
        const std::lock_guard<std::mutex> lock(vectorMutex);
        // remove/erase idiom
        xSliders.erase(std::remove(xSliders.begin(), xSliders.end(), slider), xSliders.end());
        ySliders.erase(std::remove(ySliders.begin(), ySliders.end(), slider), ySliders.end());
    }

    void XYPad::sliderValueChanged(Slider *slider)
    {
        // Avoid loopback
        if (thumb.isMouseOverOrDragging(false))
            return;

        // Figure out if the slider belongs to xSliders or ySliders
        const auto isXAxisSlider = std::find(xSliders.begin(), xSliders.end(), slider) != xSliders.end();
        const auto bounds = getLocalBounds().toDouble();
        const auto w = static_cast<double>(thumbSize);
        if (isXAxisSlider)
        {
            thumb.setTopLeftPosition(
                static_cast<int>(jmap(slider->getValue(), slider->getMinimum(), slider->getMaximum(), 0.0, bounds.getWidth() - w)),
                thumb.getY());
        }
        else
        {
            thumb.setTopLeftPosition(
                thumb.getX(),
                static_cast<int>(jmap(slider->getValue(), slider->getMinimum(), slider->getMaximum(), bounds.getHeight() - w, 0.0)));
        }
        repaint();
    }
}