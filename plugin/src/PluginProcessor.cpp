#include "3DAudioPlugin/PluginProcessor.h"
#include "3DAudioPlugin/PluginEditor.h"

//==============================================================================
PluginProcessor::PluginProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
                         ),
      apvts(*this, nullptr, "APVTS",
            {std::make_unique<juce::AudioParameterFloat>("azimuth", "Azimuth", -180.0f, 180.0f, 0.0f),
             std::make_unique<juce::AudioParameterFloat>("elevation", "Elevation", -20.0f, 20.0f, 0.0f)})
{
}

PluginProcessor::~PluginProcessor() {}

//==============================================================================
const juce::String PluginProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PluginProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool PluginProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool PluginProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double PluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int PluginProcessor::getCurrentProgram()
{
    return 0;
}

void PluginProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String PluginProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void PluginProcessor::changeProgramName(int index, const juce::String &newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.numChannels = getTotalNumOutputChannels();
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    convolutionProcessor.prepare(spec);
    loadImpulseResponseFromSliders(0, 0);

    leftPeakLevel.reset(sampleRate, 0.5);
    rightPeakLevel.reset(sampleRate, 0.5);

    leftPeakLevel.setCurrentAndTargetValue(-60.0f);
    rightPeakLevel.setCurrentAndTargetValue(-60.0f);
}

void PluginProcessor::releaseResources()
{
    convolutionProcessor.reset();
}

bool PluginProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

void PluginProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                   juce::MidiBuffer &midiMessages)
{
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear any output channels that didn't contain input data
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // H  R  T  F
    float azimuth = *apvts.getRawParameterValue("azimuth");
    azimuth = std::round(azimuth / 5.0f) * 5.0f;
    // float distance = *apvts.getRawParameterValue("distance");
    float elevation = *apvts.getRawParameterValue("elevation");
    if (currentAzimuth != azimuth || currentElevation != elevation)
    {
        currentAzimuth = azimuth;
        currentElevation = elevation;
        loadImpulseResponseFromSliders(azimuth, elevation);
    }
    juce::dsp::AudioBlock<float> block(buffer);
    convolutionProcessor.process(juce::dsp::ProcessContextReplacing<float>(block));
    // Distance factor (gain)
    buffer.applyGain(juce::Decibels::decibelsToGain(distanceValue));

    // ! A T T E N T I O N !
    buffer.applyGain(7.5f);
    // ! / ! / ! / ! / ! / !

    // Reset peak levels to find the max peak in the current block
    float maxLeftPeak = 0.0f;
    float maxRightPeak = 0.0f;

    // Loop through each sample in the buffer to find the peak levels
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        const float *channelData = buffer.getReadPointer(channel);

        // Calculate the peak for the current channel
        float channelPeak = 0.0f;
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelPeak = std::max(channelPeak, std::abs(channelData[sample]));
        }

        // Assign peak to the respective channel
        if (channel == 0)
            maxLeftPeak = channelPeak;
        else if (channel == 1)
            maxRightPeak = channelPeak;
    }

    // Update smoothed values
    leftPeakLevel.setTargetValue(maxLeftPeak);
    rightPeakLevel.setTargetValue(maxRightPeak);

    // Advance smoothing to gradually reduce the peaks over time
    leftPeakLevel.skip(buffer.getNumSamples());
    rightPeakLevel.skip(buffer.getNumSamples());
}

//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *PluginProcessor::createEditor()
{
    return new PluginEditor(*this);
}

//==============================================================================
void PluginProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused(destData);
}

void PluginProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused(data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}

void PluginProcessor::loadImpulseResponseFromSliders(float azimuth, float elevation)
{
    convolutionProcessor.loadImpulseResponse(hrtfProcessor.loadHRTFFile(azimuth, elevation),
                                             juce::dsp::Convolution::Stereo::yes,
                                             juce::dsp::Convolution::Trim::yes,
                                             0,
                                             juce::dsp::Convolution::Normalise::yes);
}

float PluginProcessor::getPeakLevel(int channel)
{
    if (channel == 0)
        return leftPeakLevel.getNextValue();
    else if (channel == 1)
        return rightPeakLevel.getNextValue();
    else
        return 0.0f; // if channel index is invalid
}