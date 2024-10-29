#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace juce::Gui
{
    class XYPad : public Component, Slider::Listener
    {
    public:
        enum class Axis
        {
            X,
            Y
        };

        class Thumb : public Component
        {
        public:
            Thumb();
            void paint(Graphics &g) override;
            void mouseDown(const MouseEvent &event) override;
            void mouseDrag(const MouseEvent &event) override;
            std::function<void(Point<double>)> moveCallback;

        private:
            ComponentDragger dragger;
            ComponentBoundsConstrainer constrainer;

            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Thumb)
        };

        XYPad();
        void resized() override;
        void paint(Graphics &g) override;
        void registerSlider(Slider *slider, Axis axis);
        void deregisterSlider(Slider *slider);

    private:
        void sliderValueChanged(Slider *slider) override;

        std::vector<Slider *> xSliders, ySliders;
        Thumb thumb;
        std::mutex vectorMutex;

        static constexpr int thumbSize = 40;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(XYPad)
    };
}