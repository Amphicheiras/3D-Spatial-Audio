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
            void setXYPad(XYPad *xyPad) { parentXYPad = xyPad; }
            void paint(Graphics &g) override;
            void mouseDown(const MouseEvent &event) override;
            void mouseDrag(const MouseEvent &event) override;
            std::function<void(Point<double>)> moveCallback;

        private:
            XYPad *parentXYPad = nullptr;
            ComponentDragger dragger;
            ComponentBoundsConstrainer constrainer;
            juce::Image speakerImage;

            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Thumb)
        };

        XYPad();
        void resized() override;
        void paint(Graphics &g) override;
        void registerSlider(Slider *slider, Axis axis);
        void deregisterSlider(Slider *slider);
        std::function<void(double)> onDistanceChanged;
        std::function<void(double)> onAngleChanged;

        double angleDegrees = 0.0;
        static constexpr int thumbSize = 64;

    private:
        void sliderValueChanged(Slider *slider) override;

        std::vector<Slider *> xSliders, ySliders;
        Thumb thumb;
        std::mutex vectorMutex;

        juce::Image headImage;
        void thumbPositionChanged(Point<double> position);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(XYPad)
    };
}