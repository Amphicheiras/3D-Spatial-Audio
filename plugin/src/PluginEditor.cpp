#include "3DAudioPlugin/PluginProcessor.h"
#include "3DAudioPlugin/PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    // AZIMUTH SLIDER
    azimuthSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    azimuthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
    azimuthSlider.setRange(-180.0f, 180.0f, 5.0f);
    azimuthSlider.setValue(*processorRef.apvts.getRawParameterValue("azimuth"));
    azimuthSlider.addMouseListener(this, false);
    azimuthSlider.onValueChange = [this]
    {
        processorRef.apvts.getParameter("azimuth")->setValueNotifyingHost((float)(azimuthSlider.getValue() + 180.0f) / 360.0f);
    };
    addAndMakeVisible(azimuthSlider);
    // AZIMUTH LABEL
    azimuthLabel.setJustificationType(juce::Justification::centred);
    azimuthLabel.attachToComponent(&azimuthSlider, false);
    addAndMakeVisible(azimuthLabel);

    // ELEVATION SLIDER
    elevationSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    elevationSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
    elevationSlider.setRange(-20.0f, 20.0f, 10.0f);
    elevationSlider.setValue(*processorRef.apvts.getRawParameterValue("elevation"));
    elevationSlider.addMouseListener(this, false);
    elevationSlider.onValueChange = [this]
    {
        processorRef.apvts.getParameter("elevation")->setValueNotifyingHost((float)(elevationSlider.getValue() + 20.0f) / 40.0f);
    };
    addAndMakeVisible(elevationSlider);
    // ELEVATION LABEL
    elevationLabel.setJustificationType(juce::Justification::centred);
    elevationLabel.attachToComponent(&elevationSlider, false);
    addAndMakeVisible(elevationLabel);

    // DISTANCE SLIDER
    distanceSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    distanceSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
    distanceSlider.setRange(-13.0, 0.0, 0.1);
    distanceSlider.setValue(-13.0);
    distanceSlider.addListener(this);
    distanceSlider.addMouseListener(this, false);
    addAndMakeVisible(distanceSlider);
    // DISTANCE LABEL
    distanceLabel.setJustificationType(juce::Justification::centred);
    distanceLabel.attachToComponent(&distanceSlider, false);
    addAndMakeVisible(distanceLabel);

    // XY PAD
    // xyPad.registerSlider(&elevationSlider, juce::Gui::XYPad::Axis::Y);
    xyPad.registerSlider(&azimuthSlider, juce::Gui::XYPad::Axis::X);
    xyPad.onDistanceChanged = [this](double distance)
    {
        distanceSlider.setValue(distance, juce::sendNotification);
    };
    xyPad.onAngleChanged = [this](double angleDegrees)
    {
        azimuthSlider.setValue(angleDegrees, juce::sendNotification);
        processorRef.apvts.getParameter("azimuth")->setValueNotifyingHost((float)(angleDegrees + 180.0f) / 360.0f);
    };
    addAndMakeVisible(xyPad);

    setSize(380, 510);
    // setResizable(true, true);

    processorRef.apvts.addParameterListener("azimuth", this);
    processorRef.apvts.addParameterListener("elevation", this);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    processorRef.apvts.removeParameterListener("azimuth", this);
    processorRef.apvts.removeParameterListener("elevation", this);
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    // Set the background color to a predefined color from juce::Colours
    g.fillAll(juce::Colours::blueviolet);
}

void AudioPluginAudioProcessorEditor::resized()
{
    // Get the local bounds and reduce them for padding
    auto container = getLocalBounds().reduced(20); // Remove 20 pixels from each side for padding

    // Set the height for the knobs (set to a reasonable value)
    const int knobHeight = 100; // Fixed height for the knobs
    const int padding = 10;     // Space between the knobs and the XY pad

    // Calculate the width for each knob
    auto knobWidth = container.getWidth() / 3; // Divide the width into three equal parts

    // Set bounds for each slider (knob) in the specified order
    azimuthSlider.setBounds(container.removeFromLeft(knobWidth).withHeight(knobHeight).reduced(padding));   // Center knob (Azimuth)
    distanceSlider.setBounds(container.removeFromLeft(knobWidth).withHeight(knobHeight).reduced(padding));  // Right knob (Distance)
    elevationSlider.setBounds(container.removeFromLeft(knobWidth).withHeight(knobHeight).reduced(padding)); // Left knob (Elevation)

    // Set the bounds for the XY pad below the knobs
    // Position the XY pad directly below the knobs, taking into account the height and padding
    xyPad.setBounds(0, knobHeight + padding + 20, getWidth(), getWidth()); // Set XY pad below the knobs with extra space
}

void AudioPluginAudioProcessorEditor::parameterChanged(const juce::String &parameterID, float newValue)
{
    if (parameterID == "elevation")
        elevationSlider.setValue(newValue);
    else if (parameterID == "azimuth")
        azimuthSlider.setValue(newValue);
}

void AudioPluginAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    if (slider == &distanceSlider)
    {
        processorRef.distanceValue = (float)distanceSlider.getValue();
    }
}

void AudioPluginAudioProcessorEditor::mouseDoubleClick(const juce::MouseEvent &event)
{
    if (event.eventComponent == &distanceSlider)
    {
        distanceSlider.setValue(-60.0f, juce::sendNotification);
    }
    else if (event.eventComponent == &elevationSlider)
    {
        elevationSlider.setValue(0.0f, juce::sendNotification);
    }
    else if (event.eventComponent == &azimuthSlider)
    {
        azimuthSlider.setValue(0.0f, juce::sendNotification);
    }
}