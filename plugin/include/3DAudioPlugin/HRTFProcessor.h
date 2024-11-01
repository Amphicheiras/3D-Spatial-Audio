#pragma once

#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_audio_formats/juce_audio_formats.h"
#include "juce_core/juce_core.h"

class HRTFProcessor
{
public:
    HRTFProcessor();
    ~HRTFProcessor();

    // Load HRTF for given angles
    const juce::File loadHRTFFile(float azimuth, float elevation, std::vector<juce::String> basePath);

private:
    // Manager for reading audio files
    juce::AudioFormatManager formatManager;
};
