#include "3DAudioPlugin/PluginProcessor.h"
#include "3DAudioPlugin/PluginEditor.h"

//==============================================================================
PluginEditor::PluginEditor(PluginProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setupSliders();
    setupXYPad();

    // GAIN METER
    addAndMakeVisible(levelMeter);

    setSize(appWidth, appHeight);
    // setResizable(true, true);
    startTimer(24);

    audioProcessor.apvts.addParameterListener("azimuth", this);
    audioProcessor.apvts.addParameterListener("elevation", this);
    audioProcessor.apvts.addParameterListener("distance", this);
}

PluginEditor::~PluginEditor()
{
    audioProcessor.apvts.removeParameterListener("azimuth", this);
    audioProcessor.apvts.removeParameterListener("elevation", this);
    audioProcessor.apvts.removeParameterListener("distance", this);
}

//==============================================================================
void PluginEditor::paint(juce::Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colours::black);

    // draw the purple circle speaker space
    g.setColour(juce::Colours::blueviolet);
    g.drawEllipse(ellipseX, ellipseY, ellipseDiameter, ellipseDiameter, lineWidth);
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
    azimuthSlider.setBounds(container.removeFromLeft(knobWidth).withHeight(knobHeight + 20).reduced(padding));
    elevationSlider.setBounds(container.removeFromLeft(knobWidth).withHeight(knobHeight).reduced(padding));
    distanceSlider.setBounds(container.removeFromLeft(knobWidth).withHeight(knobHeight + 20).reduced(padding));

    // adjust XY pad bounds to fit inside the ellipse
    float xyPadX = ellipseX + lineOffset;
    float xyPadY = ellipseY + lineOffset;
    float xyPadDiameter = ellipseDiameter - lineWidth; // Subtract line width for inner bounds
    xyPad.setBounds((int)xyPadX, (int)xyPadY, (int)xyPadDiameter, (int)xyPadDiameter);
    levelMeter.setBounds(appWidth - meterWidth, 0, meterWidth, getHeight());
}

void PluginEditor::timerCallback()
{
    levelMeter.leftLevel = audioProcessor.getPeakLevel(0);
    levelMeter.rightLevel = audioProcessor.getPeakLevel(1);
    levelMeter.repaint();
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

void PluginEditor::setupSliders()
{
    // AZIMUTH SLIDER
    customLookAndFeel = std::make_unique<CustomLookAndFeel>();
    azimuthSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::wheat);
    azimuthSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    azimuthSlider.setLookAndFeel(customLookAndFeel.get());
    azimuthSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    azimuthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 20);
    azimuthSlider.setRange(-180.0f, 180.0f, 5.0f);
    azimuthSlider.setValue(*audioProcessor.apvts.getRawParameterValue("azimuth"));
    azimuthSlider.addMouseListener(this, false);
    azimuthSlider.onValueChange = [this]
    {
        audioProcessor.apvts.getParameter("azimuth")->setValueNotifyingHost((float)(azimuthSlider.getValue() + 180.0f) / 360.0f);
    };
    addAndMakeVisible(azimuthSlider);
    // AZIMUTH LABEL
    azimuthLabel.setColour(juce::Label::textColourId, juce::Colours::wheat);
    azimuthLabel.setJustificationType(juce::Justification::centred);
    azimuthLabel.attachToComponent(&azimuthSlider, false);
    addAndMakeVisible(azimuthLabel);

    // ELEVATION SLIDER
    elevationSlider.setLookAndFeel(customLookAndFeel.get());
    elevationSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::wheat);
    elevationSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    elevationSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    elevationSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 20);
    elevationSlider.setRange(-20.0f, 20.0f, 10.0f);
    elevationSlider.setValue(*audioProcessor.apvts.getRawParameterValue("elevation"));
    elevationSlider.addMouseListener(this, false);
    elevationSlider.onValueChange = [this]
    {
        audioProcessor.apvts.getParameter("elevation")->setValueNotifyingHost((float)(elevationSlider.getValue() + 20.0f) / 40.0f);
    };
    addAndMakeVisible(elevationSlider);
    // ELEVATION LABEL
    elevationLabel.setColour(juce::Label::textColourId, juce::Colours::wheat);
    elevationLabel.setJustificationType(juce::Justification::centred);
    elevationLabel.attachToComponent(&elevationSlider, false);
    addAndMakeVisible(elevationLabel);

    // DISTANCE SLIDER
    distanceSlider.setLookAndFeel(customLookAndFeel.get());
    distanceSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::wheat);
    distanceSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    distanceSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    distanceSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 20);
    distanceSlider.setRange(-13.0, 0.0, 0.1);
    distanceSlider.setValue(-13.0);
    distanceSlider.addListener(this);
    distanceSlider.addMouseListener(this, false);
    elevationSlider.onValueChange = [this]
    {
        audioProcessor.apvts.getParameter("elevation")->setValueNotifyingHost((float)(elevationSlider.getValue() + 20.0f) / 40.0f);
    };
    addAndMakeVisible(distanceSlider);
    // DISTANCE LABEL
    distanceLabel.setColour(juce::Label::textColourId, juce::Colours::wheat);
    distanceLabel.setJustificationType(juce::Justification::centred);
    distanceLabel.attachToComponent(&distanceSlider, false);
    addAndMakeVisible(distanceLabel);
}

void PluginEditor::setupXYPad()
{
    xyPad.registerSlider(&azimuthSlider, juce::Gui::XYPad::Axis::X);
    xyPad.registerSlider(&distanceSlider, juce::Gui::XYPad::Axis::Y);
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
}