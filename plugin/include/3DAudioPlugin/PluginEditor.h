#pragma once

#include "juce_gui_basics/juce_gui_basics.h"
#include "PluginProcessor.h"
#include "XYPad.h"
#include "LevelMeter.h"
#include "CustomLookAndFeel.h"

//==============================================================================
class PluginEditor final : public juce::AudioProcessorEditor,
                           public juce::AudioProcessorValueTreeState::Listener,
                           public juce::Slider::Listener,
                           public juce::Timer
{
public:
    explicit PluginEditor(PluginProcessor &);
    ~PluginEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;
    void timerCallback() override;

    // Listener method
    void parameterChanged(const juce::String &parameterID, float newValue) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor &audioProcessor;

    std::unique_ptr<CustomLookAndFeel> customLookAndFeel;

    int appWidth = 440;
    int appHeight = 510;

    juce::Slider azimuthSlider;
    juce::Label azimuthLabel{"azimuthLabel", "AZIMUTH"};

    juce::Slider elevationSlider;
    juce::Label elevationLabel{"elevationLabel", "ELEVATION"};

    juce::Slider distanceSlider;
    juce::Label distanceLabel{"distanceLabel", "DISTANCE"};

    int meterWidth = 60;
    juce::Gui::LevelMeter levelMeter;

    // purple circle (ellipse) around XY pad
    float lineWidth = 3.f;
    float lineOffset = 3.f / 2.f;
    float ellipseX = 0.f + lineOffset;
    float ellipseY = 130.0f + lineOffset;
    float ellipseDiameter = float(appWidth - meterWidth - lineWidth);

    juce::Gui::XYPad xyPad;

    void setupSliders();
    void setupXYPad();

    void sliderValueChanged(juce::Slider *slider) override;
    void mouseDoubleClick(const juce::MouseEvent &event) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
