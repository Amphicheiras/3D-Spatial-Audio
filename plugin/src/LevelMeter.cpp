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
        g.fillAll(juce::Colours::black);

        // Draw left and right gain levels
        g.setColour(juce::Colours::green);
        auto meterWidth = getWidth() / 4; // Width for each meter bar

        int gainRange = 30;

        // left meter
        if (leftLevel < -gainRange)
        {
            leftLevel = static_cast<float>(-gainRange);
        }
        double normalizedLevel = (leftLevel + gainRange) / gainRange; // normalize to 0-1
        int rectHeight = (int)(normalizedLevel * getHeight());
        g.fillRect(10, getHeight() - rectHeight, meterWidth, rectHeight);

        // right meter
        if (rightLevel < -gainRange)
        {
            rightLevel = static_cast<float>(-gainRange);
        }
        normalizedLevel = (rightLevel + gainRange) / gainRange; // normalize to 0-1
        rectHeight = (int)(normalizedLevel * getHeight());
        g.fillRect(20 + meterWidth, getHeight() - rectHeight, meterWidth, rectHeight);
    }

    void LevelMeter::resized()
    {
        // Set bounds for the fader on the right side of the component
        // volumeFader.setBounds(getWidth() - 40, 10, 30, getHeight() - 20);
    }
}