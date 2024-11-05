#include "3DAudioPlugin/HRTFProcessor.h"

HRTFProcessor::HRTFProcessor()
{

    formatManager.registerBasicFormats();
}

HRTFProcessor::~HRTFProcessor() {}

const juce::File HRTFProcessor::loadHRTFFile(float azimuth, float elevation)
{
    if (std::abs(azimuth) < 1e-6) // small threshold to capture near-zero values (or it brakes -0)
    {
        azimuth = 0.0f;
    }
    juce::String azimuthString = "";
    azimuthString = juce::String((azimuth < 0) ? -azimuth : azimuth).paddedLeft('0', 3);
    azimuthString = (azimuth < 0) ? "-" + azimuthString : azimuthString;
    juce::String elevationString = juce::String(static_cast<int>(elevation));
    juce::String fileName = "H" + elevationString + "e" + azimuthString + "a.wav";
    juce::File hrtfFile = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("UF0/3DSA/resources/hrtf/elev" + elevationString + "/" + fileName);
    if (hrtfFile.existsAsFile())
    {
        return hrtfFile;
    }
    DBG("HRTF file not found: " + hrtfFile.getFullPathName());
    jassertfalse;
    return juce::File();
}