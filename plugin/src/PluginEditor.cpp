#include "3DAudioPlugin/PluginProcessor.h"
#include "3DAudioPlugin/PluginEditor.h"

//==============================================================================
PluginEditor::PluginEditor(PluginProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // AZIMUTH SLIDER
    azimuthSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    azimuthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
    azimuthSlider.setRange(-180.0f, 180.0f, 5.0f);
    azimuthSlider.setValue(*audioProcessor.apvts.getRawParameterValue("azimuth"));
    azimuthSlider.addMouseListener(this, false);
    azimuthSlider.onValueChange = [this]
    {
        audioProcessor.apvts.getParameter("azimuth")->setValueNotifyingHost((float)(azimuthSlider.getValue() + 180.0f) / 360.0f);
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
    elevationSlider.setValue(*audioProcessor.apvts.getRawParameterValue("elevation"));
    elevationSlider.addMouseListener(this, false);
    elevationSlider.onValueChange = [this]
    {
        audioProcessor.apvts.getParameter("elevation")->setValueNotifyingHost((float)(elevationSlider.getValue() + 20.0f) / 40.0f);
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
        audioProcessor.apvts.getParameter("azimuth")->setValueNotifyingHost((float)(angleDegrees + 180.0f) / 360.0f);
    };
    addAndMakeVisible(xyPad);

    // GAIN METER
    addAndMakeVisible(gainMeter);

    setSize(440, 510);
    // setResizable(true, true);

    startTimer(24);

    audioProcessor.apvts.addParameterListener("azimuth", this);
    audioProcessor.apvts.addParameterListener("elevation", this);
}

PluginEditor::~PluginEditor()
{
    audioProcessor.apvts.removeParameterListener("azimuth", this);
    audioProcessor.apvts.removeParameterListener("elevation", this);
}

//==============================================================================
void PluginEditor::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    // Set the background color to a predefined color from juce::Colours
    g.fillAll(juce::Colours::blueviolet);
}

void PluginEditor::resized()
{
    // Get the local bounds and reduce them for padding
    auto container = getLocalBounds().reduced(20); // Remove 20 pixels from each side for padding

    // Set the height for the knobs (set to a reasonable value)
    const int knobHeight = 100; // Fixed height for the knobs
    const int padding = 10;     // Space between the knobs and the XY pad

    // Calculate the width for each knob
    auto knobWidth = (container.getWidth() - 60) / 3; // Divide the width into three equal parts

    // Set bounds for each slider (knob) in the specified order
    azimuthSlider.setBounds(container.removeFromLeft(knobWidth).withHeight(knobHeight).reduced(padding));
    distanceSlider.setBounds(container.removeFromLeft(knobWidth).withHeight(knobHeight).reduced(padding));
    elevationSlider.setBounds(container.removeFromLeft(knobWidth).withHeight(knobHeight).reduced(padding));

    // Set the bounds for the XY pad below the knobs
    // Position the XY pad directly below the knobs, taking into account the height and padding
    xyPad.setBounds(0, knobHeight + padding + 20, getWidth() - 60, getWidth() - 60);
    auto meterWidth = 60;
    gainMeter.setBounds(380, 0, meterWidth, getHeight());
}

void PluginEditor::timerCallback()
{
    gainMeter.leftLevel = audioProcessor.getRMSValue(0);
    gainMeter.rightLevel = audioProcessor.getRMSValue(1);
    gainMeter.repaint();
}

void PluginEditor::parameterChanged(const juce::String &parameterID, float newValue)
{
    if (parameterID == "elevation")
        elevationSlider.setValue(newValue);
    else if (parameterID == "azimuth")
        azimuthSlider.setValue(newValue);
}

void PluginEditor::sliderValueChanged(juce::Slider *slider)
{
    if (slider == &distanceSlider)
    {
        audioProcessor.distanceValue = (float)distanceSlider.getValue();
    }
}

void PluginEditor::mouseDoubleClick(const juce::MouseEvent &event)
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