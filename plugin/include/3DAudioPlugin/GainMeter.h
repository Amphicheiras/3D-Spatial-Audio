#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace juce::Gui
{
    class GainMeter : public Component
    {
    public:
        GainMeter();
        ~GainMeter() override;

        void resized() override;
        void paint(Graphics &g) override;

        float leftLevel = 0.0f;
        float rightLevel = 0.0f;

    private:
        Slider volumeFader;
        Label volumeLabel;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainMeter)
    };
}