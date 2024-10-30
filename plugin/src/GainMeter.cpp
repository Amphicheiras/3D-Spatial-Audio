#include "3DAudioPlugin/GainMeter.h"

namespace juce::Gui
{
    GainMeter::GainMeter()
    {
        // Configure volume fader
        volumeFader.setSliderStyle(juce::Slider::LinearVertical);
        volumeFader.setRange(0.0f, 1.0f, 0.01f);
        volumeFader.setValue(1.0f);  // Set initial value
        addAndMakeVisible(volumeFader);

        volumeLabel.setText("Volume", juce::dontSendNotification);
        volumeLabel.attachToComponent(&volumeFader, false);
        addAndMakeVisible(volumeLabel);

        startMeterUpdates();
    }

    GainMeter::~GainMeter()
    {
        stopMeterUpdates();
    }

    void GainMeter::paint(juce::Graphics& g)
    {
        // Draw the background
        g.fillAll(juce::Colours::black);

        // Draw left and right gain levels
        g.setColour(juce::Colours::green);
        auto meterWidth = getWidth() / 4;  // Width for each meter bar

        // Left meter
        g.fillRect(10, getHeight() - (int)(leftLevel * getHeight()), meterWidth, (int)(leftLevel * getHeight()));

        // Right meter
        g.fillRect(20 + meterWidth, getHeight() - (int)(rightLevel * getHeight()), meterWidth, (int)(rightLevel * getHeight()));
    }

    void GainMeter::resized()
    {
        // Set bounds for the fader on the right side of the component
        volumeFader.setBounds(getWidth() - 40, 10, 30, getHeight() - 20);
    }

    void GainMeter::timerCallback()
    {
        // Update leftLevel and rightLevel based on real-time audio processing values
        // (Typically, you get these values from your audio processor)
        // leftLevel = processorRef.getLeftChannelLevel();
        // rightLevel = processorRef.getRightChannelLevel();

        repaint();  // Repaint to show updated levels
    }

    void GainMeter::startMeterUpdates()
    {
        startTimerHz(30);  // Adjust timer frequency as needed
    }

    void GainMeter::stopMeterUpdates()
    {
        stopTimer();
    }

    // void GainMeter::setGain(float newGain)
    // {
    //     // Set the gain value in your audio processor
    //     // processorRef.setGain(newGain);
    // }
}