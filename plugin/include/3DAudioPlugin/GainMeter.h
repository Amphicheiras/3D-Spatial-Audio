#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace juce::Gui
{
    class GainMeter : public Component, public Timer
    {
    public:
        GainMeter();
        ~GainMeter() override;

        void resized() override;
        void paint(Graphics &g) override;

        // void setGain(float newGain);     // Method to set gain for the fader
        void startMeterUpdates();        // Start meter updates with a timer
        void stopMeterUpdates();         // Stop the timer updates for the meter

    private:
        void timerCallback() override;
        Slider volumeFader;
        Label volumeLabel;

        float leftLevel = 0.0f;
        float rightLevel = 0.0f;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainMeter)
    };
}