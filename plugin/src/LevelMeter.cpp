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
        int rectHeight = static_cast<int>(normalizedLevel * getHeight());
        g.setColour(juce::Colours::blueviolet.darker(0.9f).darker(0.9f));
        g.fillRect(10, 0, meterWidth, getHeight());
        // Change color based on level
        juce::Colour meterColor = juce::Colours::green;
        if (leftLevel > -6.0f)
        {
            meterColor = juce::Colours::orange;
        }
        if (leftLevel > -3.0f)
        {
            meterColor = juce::Colours::red;
        }
        g.setColour(meterColor);
        g.fillRect(10, getHeight() - rectHeight, meterWidth, rectHeight);

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
        rectHeight = (int)(normalizedLevel * getHeight());

        //// ! //////////////////////////////////////////

        // Change color based on level
        meterColor = juce::Colours::green;
        if (rightLevel > -6.0f)
        {
            meterColor = juce::Colours::orange;
        }
        if (rightLevel > -3.0f)
        {
            meterColor = juce::Colours::red;
        }
        g.setColour(meterColor);
        g.fillRect(20 + meterWidth, getHeight() - rectHeight, meterWidth, rectHeight);

        //// ! //////////////////////////////////////////

        // draw green level (up to -6 dB)
        int greenHeight = getHeight();
        // green section is (-gain range, -6 dB]
        greenHeight = (int)(normalizedLevel * getHeight());
        g.setColour(juce::Colours::green);
        g.fillRect(20 + meterWidth, getHeight() - greenHeight, meterWidth, greenHeight);
        // // Draw the orange section (from -6 dB to -3 dB)
        // int orangeHeight = 0;
        // if (rightLevel > -6.0f && rightLevel <= -3.0f)
        // {
        //     orangeHeight = (int)(((-3.0f + gainRange) / gainRange) * getHeight());
        // }
        // g.setColour(juce::Colours::orange);
        // // g.fillRect(20 + meterWidth, getHeight() - orangeHeight, meterWidth, getHeight() - greenHeight - orangeHeight);

        // Draw the red section (above -3 dB)
        int redHeight = 0;
        if (rightLevel > -3.0f)
        {
            redHeight = (int)(((-0.0f + gainRange) / gainRange) * getHeight());
        }
        g.setColour(juce::Colours::red);
        g.fillRect(20 + meterWidth, getHeight() - redHeight, meterWidth, redHeight);
    }

    void LevelMeter::resized()
    {
        // Set bounds for the fader on the right side of the component
        // volumeFader.setBounds(getWidth() - 40, 10, 30, getHeight() - 20);
    }
}