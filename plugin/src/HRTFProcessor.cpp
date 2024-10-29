#include "3DAudioPlugin/HRTFProcessor.h"

HRTFProcessor::HRTFProcessor()
{
    formatManager.registerBasicFormats();
}

HRTFProcessor::~HRTFProcessor() {}

const juce::File HRTFProcessor::loadHRTFFile(float azimuth, float elevation)
{
    juce::String azimuthString = "";
    azimuthString = juce::String((azimuth < 0) ? -azimuth : azimuth).paddedLeft('0', 3);
    azimuthString = (azimuth < 0) ? "-" + azimuthString : azimuthString;
    juce::String elevationString = juce::String(elevation);
    juce::String fileName = "H" + elevationString + "e" + azimuthString + "a.wav";
    juce::String filePath = juce::File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory().getFullPathName() + "/plugin/resources/hrtf/elev" + elevationString + "/" + fileName;
    const juce::File hrtfFile = filePath;
    DBG(hrtfFile.getFullPathName());
    if (!hrtfFile.existsAsFile())
    {
        DBG("HRTF file not found: " + hrtfFile.getFullPathName());
        jassertfalse;
    }
    return hrtfFile;
}
