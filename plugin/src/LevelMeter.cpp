#include "3DAudioPlugin/LevelMeter.h"

namespace juce::Gui
{
    LevelMeter::LevelMeter()
    {
    }

    LevelMeter::~LevelMeter()
    {
    }

    void LevelMeter::paint(juce::Graphics &g)
    {
        // Draw the background
        g.fillAll(juce::Colours::blueviolet.darker(0.7f).darker(0.8f));

        // Draw left and right gain levels
        auto meterWidth = getWidth() / 4; // Width for each meter bar

        float gainRange = 30.0f;

        // *************** left level meter ***************
        leftLevel = 20.0f * log10f(std::max(leftLevel, 1e-6f)); // Avoid log(0)
        // clamp and normalize the left level meter
        if (leftLevel < -gainRange)
        {
            leftLevel = -gainRange;
        }
        else if (leftLevel > 0.0f) // ensure no values go above 0 dB
        {
            leftLevel = 0.0f;
        }
        float normalizedLevel = (leftLevel + gainRange) / gainRange; // normalize to 0-1
                                                                     // green section is (-gain range, -6dB]
        int greenHeight = 0;
        if (leftLevel <= -6.0f)
        {
            greenHeight = (int)(normalizedLevel * getHeight());
        }
        else
        {
            greenHeight = (int)(((-6.f + gainRange) / gainRange) * getHeight());
        }
        g.setColour(juce::Colours::green);
        g.fillRect(10, getHeight() - greenHeight, meterWidth, greenHeight);

        // draw the orange section (-6dB, -3dB]
        int orangeHeight = 0;
        if (leftLevel > -6.0f && leftLevel <= -3.0f)
        {
            orangeHeight = (int)(normalizedLevel * getHeight());
            g.setColour(juce::Colours::orange);
            g.fillRect(10, getHeight() - orangeHeight, meterWidth, orangeHeight - greenHeight);
        }
        else if (leftLevel > -3.f)
        {
            orangeHeight = (int)(((-3.f + gainRange) / gainRange) * getHeight());
            g.setColour(juce::Colours::orange);
            g.fillRect(10, getHeight() - orangeHeight, meterWidth, orangeHeight - greenHeight);
        }

        // draw the red section (-3dB, 0dB]
        int redHeight = 0;
        if (leftLevel > -3.0f)
        {
            redHeight = (int)(normalizedLevel * getHeight());
            g.setColour(juce::Colours::red);
            g.fillRect(10, getHeight() - redHeight, meterWidth, redHeight - orangeHeight);
        }

        // *************** right level meter ***************
        // draw meter background line
        g.setColour(juce::Colours::blueviolet.darker(0.9f).darker(0.9f));
        g.fillRect(20 + meterWidth, 0, meterWidth, getHeight());

        // draw meter level line
        rightLevel = 20.0f * log10f(std::max(rightLevel, 1e-6f)); // avoid log(0)
        // clamp level to gain range
        if (rightLevel < -gainRange)
        {
            rightLevel = static_cast<float>(-gainRange);
        }
        // normalize to 0-1
        normalizedLevel = (rightLevel + gainRange) / gainRange;

        // green section is (-gain range, -6dB]
        greenHeight = 0;
        if (rightLevel <= -6.0f)
        {
            greenHeight = (int)(normalizedLevel * getHeight());
        }
        else
        {
            greenHeight = (int)(((-6.f + gainRange) / gainRange) * getHeight());
        }
        g.setColour(juce::Colours::green);
        g.fillRect(20 + meterWidth, getHeight() - greenHeight, meterWidth, greenHeight);

        // draw the orange section (-6dB, -3dB]
        orangeHeight = 0;
        if (rightLevel > -6.0f && rightLevel <= -3.0f)
        {
            orangeHeight = (int)(normalizedLevel * getHeight());
            g.setColour(juce::Colours::orange);
            g.fillRect(20 + meterWidth, getHeight() - orangeHeight, meterWidth, orangeHeight - greenHeight);
        }
        else if (rightLevel > -3.f)
        {
            orangeHeight = (int)(((-3.f + gainRange) / gainRange) * getHeight());
            g.setColour(juce::Colours::orange);
            g.fillRect(20 + meterWidth, getHeight() - orangeHeight, meterWidth, orangeHeight - greenHeight);
        }

        // draw the red section (-3dB, 0dB]
        redHeight = 0;
        if (rightLevel > -3.0f)
        {
            redHeight = (int)(normalizedLevel * getHeight());
            g.setColour(juce::Colours::red);
            g.fillRect(20 + meterWidth, getHeight() - redHeight, meterWidth, redHeight - orangeHeight);
        }
    }

    void LevelMeter::resized()
    {
        // Set bounds for the fader on the right side of the component
        // volumeFader.setBounds(getWidth() - 40, 10, 30, getHeight() - 20);
    }
}