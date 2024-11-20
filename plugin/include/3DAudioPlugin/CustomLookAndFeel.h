#include <juce_gui_basics/juce_gui_basics.h>

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel()
    {
        // Optionally, set some default colors or properties
        setColour(juce::Slider::thumbColourId, juce::Colours::wheat);
        setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::blueviolet.darker(0.9f));
        setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::blueviolet.darker(0.9f));
    }

    void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height,
                          float sliderPosProportional, float rotaryStartAngle,
                          float rotaryEndAngle, juce::Slider &slider) override
    {
        rotaryStartAngle -= juce::MathConstants<float>::pi / 2.0f;
        rotaryEndAngle -= juce::MathConstants<float>::pi / 2.0f;

        auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
        auto centerX = (float)x + (float)width * 0.5f;
        auto centerY = (float)y + (float)height * 0.5f;
        auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

        // Background circle
        g.setColour(slider.findColour(juce::Slider::rotarySliderOutlineColourId));
        g.fillEllipse(centerX - radius, centerY - radius, radius * 2.0f, radius * 2.0f);

        // Rotary arc
        g.setColour(slider.findColour(juce::Slider::rotarySliderFillColourId));
        juce::Path arc;
        arc.addPieSegment(centerX - radius, centerY - radius, radius * 2.0f, radius * 2.0f,
                          rotaryStartAngle, angle, 0.9f);
        g.fillPath(arc);

        // Thumb
        juce::Point<float> thumbPoint(centerX + radius * std::cos(angle),
                                      centerY + radius * std::sin(angle));
        g.setColour(slider.findColour(juce::Slider::thumbColourId));
        g.drawLine(centerX, centerY, thumbPoint.getX(), thumbPoint.getY(), 2.0f);
    }
};
