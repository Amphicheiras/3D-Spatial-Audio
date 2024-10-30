#include "3DAudioPlugin/XYPad.h"

namespace juce::Gui
{
    /*
     * XY Pad Thumb section
     */
    XYPad::Thumb::Thumb()
    {
        speakerImage = ImageFileFormat::loadFrom(File(juce::File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory().getParentDirectory().getParentDirectory().getParentDirectory().getFullPathName() + "/plugin/resources/images/speakerIcon.png"));
        constrainer.setMinimumOnscreenAmounts(thumbSize, thumbSize, thumbSize, thumbSize);
    }

    void XYPad::Thumb::paint(Graphics &g)
    {
        if (speakerImage.isValid())
        {
            // float scaledFactor = 0.5f;
            // auto scaledWidth = static_cast<int>(speakerImage.getWidth() * scaledFactor);
            // auto scaledHeight = static_cast<int>(speakerImage.getHeight() * scaledFactor);
            // auto centerX = (getWidth() - scaledWidth) / 2.0f - 32;
            // auto centerY = (getHeight() - scaledHeight) / 2.0f - 32;

            // Get the angle from the XYPad
            float rotationAngle = static_cast<float>(parentXYPad->angleDegrees) * (juce::MathConstants<float>::pi / 180.0f);

            // Apply rotation transformation around the center of the thumb
            AffineTransform transform = AffineTransform::rotation(
                rotationAngle,
                getWidth() / 2.0f,
                getHeight() / 2.0f);

            // Draw the rotated image
            g.drawImageTransformed(speakerImage, transform, false);
        }
    }

    void XYPad::Thumb::mouseDown(const MouseEvent &event)
    {
        dragger.startDraggingComponent(this, event);
    }

    void XYPad::Thumb::mouseDrag(const MouseEvent &event)
    {
        dragger.dragComponent(this, event, &constrainer);

        // Get the current position of the thumb
        auto newPosition = getPosition().toDouble();
        auto centerX = parentXYPad->getWidth() / 2.0 - thumbSize / 2;
        auto centerY = parentXYPad->getHeight() / 2.0 - thumbSize / 2;

        // Calculate the distance from the center
        double dx = newPosition.getX() - centerX;
        double dy = newPosition.getY() - centerY;
        double distance = std::sqrt(dx * dx + dy * dy);

        // Check if the distance is greater than the allowed radius
        double radius = (parentXYPad->getWidth() / 2.0) - (thumbSize / 2.0); // Adjust for thumb size
        if (distance > radius)
        {
            // Calculate the new position on the circle
            double angle = std::atan2(dy, dx);
            newPosition.setXY(centerX + std::cos(angle) * radius, centerY + std::sin(angle) * radius);
        }

        // Set the thumb position
        setTopLeftPosition(newPosition.toInt());

        if (moveCallback)
            moveCallback(getPosition().toDouble());

        if (parentXYPad != nullptr)
        {
            parentXYPad->thumbPositionChanged(getPosition().toDouble());
        }

        auto newPositionThumb = getPosition().toDouble();
        DBG("Thumb Position: " << newPositionThumb.toString());

        // Get current mouse position
        auto mousePosition = event.getPosition().toDouble();
        DBG("Mouse Position: " << mousePosition.toString());
    }

    /*
     * XY Pad section
     */
    XYPad::XYPad()
    {
        headImage = ImageFileFormat::loadFrom(File(juce::File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory().getParentDirectory().getParentDirectory().getParentDirectory().getFullPathName() + "/plugin/resources/images/head_above_view.png"));
        thumb.setXYPad(this);

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
        g.fillEllipse(getLocalBounds().toFloat());
        if (headImage.isValid())
        {
            auto imageWidth = headImage.getWidth();
            auto imageHeight = headImage.getHeight();

            float scaledFactor = 0.5f;
            auto scaledWidth = static_cast<int>(imageWidth * scaledFactor);
            auto scaledHeight = static_cast<int>(imageHeight * scaledFactor);
            auto centerX = (getWidth() - scaledWidth) / 2;
            auto centerY = (getHeight() - scaledHeight) / 2;

            g.drawImage(headImage, centerX, centerY, scaledWidth, scaledHeight, 0, 0, imageWidth, imageHeight);
        }
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

    void XYPad::thumbPositionChanged(Point<double> position)
    {
        const double thumbX = position.getX() - (getWidth() / 2.0) + thumbSize / 2;
        const double thumbY = position.getY() - (getHeight() / 2.0) + thumbSize / 2;
        // const double thumbX = position.getX() - (getWidth() / 2.0 + thumbSize);
        // const double thumbY = position.getY() - (getHeight() / 2.0 + thumbSize);

        // Calculate angle in radians
        double angle = std::atan2(thumbY, thumbX); // Angle in radians

        // Convert the angle to degrees
        angleDegrees = angle * (180.0 / juce::MathConstants<double>::pi); // Using M_PI for better precision

        // Normalize angle to [-180, 180]
        if (angleDegrees < -180.0)
            angleDegrees += 360.0;
        else if (angleDegrees > 180.0)
            angleDegrees -= 360.0;

        // Adjust the angle so that 0 degrees is at the top center
        angleDegrees = std::fmod(angleDegrees + 90.0, 360.0); // Shift by 90 degrees

        // Map the angle to [-180, 0, 180]
        if (angleDegrees > 180.0)
            angleDegrees -= 360.0; // Reflect to get the [-180, 0, 180] range

        if (onAngleChanged)
            onAngleChanged(angleDegrees);

        // Calculate distance
        double radius = getWidth() / 2.0;
        double distance = std::sqrt(thumbX * thumbX + thumbY * thumbY);
        distance = std::min(distance, radius); // Clamp the distance to the radius
        distance = jmap(static_cast<float>(distance), 0.0f, static_cast<float>(radius), 0.0f, -60.0f);

        if (onDistanceChanged)
            onDistanceChanged(distance);

        DBG(" Distance! " + (juce::String)distance);

        repaint();
    }
}