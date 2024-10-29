#pragma once

#include "juce_gui_basics/juce_gui_basics.h"
#include "PluginProcessor.h"
#include "XYPad.h"

//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor,
                                              public juce::AudioProcessorValueTreeState::Listener,
                                              public juce::Slider::Listener
{
public:
    explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

    // Listener method
    void parameterChanged(const juce::String &parameterID, float newValue) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor &processorRef;

    void sliderValueChanged(juce::Slider *slider) override;
    void mouseDoubleClick(const juce::MouseEvent &event) override;

    juce::Slider azimuthSlider;
    juce::Label azimuthLabel{"azimuthLabel", "AZIMUTH"};

    juce::Slider elevationSlider;
    juce::Label elevationLabel{"elevationLabel", "ELEVATION"};

    juce::Slider distanceSlider;
    juce::Label distanceLabel{"distanceLabel", "DISTANCE"};

    juce::Gui::XYPad xyPad;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
