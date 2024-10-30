#include "3DAudioPlugin/GainMeter.h"

namespace juce::Gui
{
    GainMeter::GainMeter()
    {
        // Configure volume fader
        volumeFader.setSliderStyle(juce::Slider::LinearVertical);
        volumeFader.setRange(0.0f, 1.0f, 0.01f);
        volumeFader.setValue(1.0f); // Set initial value
        addAndMakeVisible(volumeFader);

        volumeLabel.setText("Volume", juce::dontSendNotification);
        volumeLabel.attachToComponent(&volumeFader, false);
        addAndMakeVisible(volumeLabel);
    }

    GainMeter::~GainMeter()
    {
    }

    void GainMeter::paint(juce::Graphics &g)
    {
        // Draw the background
        g.fillAll(juce::Colours::black);

        // Draw left and right gain levels
        g.setColour(juce::Colours::green);
        auto meterWidth = getWidth() / 4; // Width for each meter bar

        // left meter
        double normalizedLevel = (leftLevel + 100) / 100; // normalize to 0-1
        int rectHeight = (int)(normalizedLevel * getHeight());
        g.fillRect(10, getHeight() - rectHeight, meterWidth, rectHeight);

        // right meter
        normalizedLevel = (rightLevel + 100) / 100; // normalize to 0-1
        rectHeight = (int)(normalizedLevel * getHeight());
        g.fillRect(20 + meterWidth, getHeight() - rectHeight, meterWidth, rectHeight);
    }

    void GainMeter::resized()
    {
        // Set bounds for the fader on the right side of the component
        volumeFader.setBounds(getWidth() - 40, 10, 30, getHeight() - 20);
    }
}