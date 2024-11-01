#include "3DAudioPlugin/XYPad.h"

namespace juce::Gui
{
    /*
     * XY Pad Speaker section
     */
    XYPad::Speaker::Speaker()
    {
        speakerImage = loadSpeakerImage();
        constrainer.setMinimumOnscreenAmounts(speakerSize, speakerSize, speakerSize, speakerSize);
    }

    void XYPad::Speaker::paint(Graphics &g)
    {
        if (speakerImage.isValid())
        {
            // get angle from XYPad
            float rotationAngle = static_cast<float>(parentXYPad->angleDegrees) * (MathConstants<float>::pi / 180.0f);
            // apply rotation transformation around the center of the speaker
            AffineTransform transform = AffineTransform::rotation(
                rotationAngle,
                getWidth() / 2.0f,
                getHeight() / 2.0f);

            // draw the rotated image
            g.drawImageTransformed(speakerImage, transform, false);
        }
    }

    void XYPad::Speaker::mouseDown(const MouseEvent &event)
    {
        dragger.startDraggingComponent(this, event);
    }

    void XYPad::Speaker::mouseDrag(const MouseEvent &event)
    {
        dragger.dragComponent(this, event, &constrainer);

        // Get the current position of the speaker
        auto newPosition = getPosition().toDouble();
        auto centerX = parentXYPad->getWidth() / 2.0 - speakerSize / 2;
        auto centerY = parentXYPad->getHeight() / 2.0 - speakerSize / 2;

        // Calculate the distance from the center
        double dx = newPosition.getX() - centerX;
        double dy = newPosition.getY() - centerY;
        double distance = std::sqrt(dx * dx + dy * dy);

        // Check if the distance is greater than the allowed outter radius
        double radius = (parentXYPad->getWidth() / 2.0) - (speakerSize / 2.0); // Adjust for speaker size
        if (distance > radius)
        {
            // Calculate the new position on the circle
            double angle = std::atan2(dy, dx);
            newPosition.setXY(centerX + std::cos(angle) * radius, centerY + std::sin(angle) * radius);
        }
        // Check if the distance is lesser than the allowed inner radius
        double radiusInner = speakerSize; // Adjust for speaker size
        if (distance < radiusInner)
        {
            // Calculate the new position on the circle
            double angle = std::atan2(dy, dx);
            newPosition.setXY(centerX + std::cos(angle) * radiusInner, centerY + std::sin(angle) * radiusInner);
        }

        // Set the speaker position
        setTopLeftPosition(newPosition.toInt());

        if (moveCallback)
            moveCallback(getPosition().toDouble());

        if (parentXYPad != nullptr)
        {
            parentXYPad->speakerPositionChanged(getPosition().toDouble());
        }
    }

    Image XYPad::Speaker::loadSpeakerImage()
    {
        File speakerImageFile = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile("resources/images/speakerIcon.png");
        if (speakerImageFile.existsAsFile())
        {
            speakerImage = ImageFileFormat::loadFrom(speakerImageFile);
            return speakerImage;
        }
        DBG("Speaker image not found: " + speakerImageFile.getFullPathName());
        jassertfalse;
        return Image();
    }

    /*
     * XY Pad section
     */
    XYPad::XYPad() : speaker()
    {
        headImage = loadHeadImage();

        speaker.setXYPad(this);

        addAndMakeVisible(speaker);
        speaker.moveCallback = [&](Point<double> position)
        {
            const std::lock_guard<std::mutex> lock(vectorMutex);
            const auto bounds = getLocalBounds().toDouble();
            const auto w = static_cast<double>(speakerSize);
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
        speaker.setBounds(getLocalBounds().withSizeKeepingCentre(speakerSize, speakerSize).withY(getHeight() / 2 - getHeight() / 2));
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
        if (speaker.isMouseOverOrDragging(false))
            return;

        // Figure out if the slider belongs to xSliders or ySliders
        const auto isXAxisSlider = std::find(xSliders.begin(), xSliders.end(), slider) != xSliders.end();
        const auto bounds = getLocalBounds().toDouble();
        const auto w = static_cast<double>(speakerSize);
        if (isXAxisSlider)
        {
            speaker.setTopLeftPosition(
                static_cast<int>(jmap(slider->getValue(), slider->getMinimum(), slider->getMaximum(), 0.0, bounds.getWidth() - w)),
                speaker.getY());
        }
        else
        {
            speaker.setTopLeftPosition(
                speaker.getX(),
                static_cast<int>(jmap(slider->getValue(), slider->getMinimum(), slider->getMaximum(), bounds.getHeight() - w, 0.0)));
        }
        repaint();
    }

    void XYPad::speakerPositionChanged(Point<double> position)
    {
        const double speakerX = position.getX() - (getWidth() / 2.0) + speakerSize / 2;
        const double speakerY = position.getY() - (getHeight() / 2.0) + speakerSize / 2;

        // Calculate angle in radians
        double angle = std::atan2(speakerY, speakerX); // Angle in radians

        // Convert the angle to degrees
        angleDegrees = angle * (180.0 / MathConstants<double>::pi); // Using M_PI for better precision

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
        double distance = std::sqrt(speakerX * speakerX + speakerY * speakerY);
        distance = std::min(distance, radius); // Clamp the distance to the radius
        distance = jmap(static_cast<float>(distance), (float)speakerSize, static_cast<float>(radius), 0.0f, -16.0f);

        if (onDistanceChanged)
            onDistanceChanged(distance);

        repaint();
    }

    Image XYPad::loadHeadImage()
    {
        File headImageFile = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile("resources/images/headIcon.png");
        if (headImageFile.existsAsFile())
        {
            headImage = ImageFileFormat::loadFrom(headImageFile);
            return headImage;
        }
        DBG("Head image not found: " + headImageFile.getFullPathName());
        jassertfalse;
        return Image();
    }
}
