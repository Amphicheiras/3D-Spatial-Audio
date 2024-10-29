HRTF 3D Spatial Audio Plugin

An audio plugin for creating immersive 3D spatial sound experiences using Head-Related Transfer Function (HRTF) processing. This plugin allows audio positioning in three-dimensional space through azimuth and elevation control, enabling enhanced realism for spatial audio applications such as VR, games, and music production.
Features

    Azimuth & Elevation Controls: Real-time control over sound source direction using azimuth (horizontal angle) and elevation (vertical angle).
    Distance-Based Attenuation: Simulates distance by controlling gain levels, allowing dynamic volume adjustments based on the source's proximity.
    HRTF Convolution Processing: Utilizes impulse responses (IRs) based on HRTF data for accurate spatialization of audio.
    Real-Time Parameter Updates: Changes to azimuth, elevation, and distance are applied in real-time with low latency.
    User-Friendly Interface: Intuitive controls for adjusting spatial parameters within the plugin editor.

Getting Started
Prerequisites

    JUCE Framework: Required for building and running the plugin. Download from JUCE.
    Supported DAWs: Any Digital Audio Workstation that supports VST, AU, or standalone plugins.
    Compiler: Ensure a compatible C++ compiler is installed. For Windows, MSVC is recommended, while Xcode works well on macOS.

Installation

    Clone the repository:

    sh

    git clone https://github.com/yourusername/your-repo-name.git
    cd your-repo-name

    Open in JUCE Projucer: Open the .jucer project file using JUCE Projucer, configure settings, and export for your target platform (e.g., Visual Studio for Windows or Xcode for macOS).

    Compile the Plugin: Open the generated project in your chosen IDE, build the plugin, and ensure it compiles without errors.

Directory Structure

graphql

your-repo-name/
├── Source/                         # Main source code for plugin
│   ├── PluginProcessor.cpp         # Audio processing and DSP logic
│   ├── PluginEditor.cpp            # GUI elements and controls
│   ├── HRTFProcessor.cpp           # HRTF loading and IR management
│   └── XYPad.cpp                   # Interactive XY pad for spatial control
├── resources/                      # Resources including HRTF files
│   └── hrtf/                       # Folder containing HRTF .wav files
├── CMakeLists.txt                  # CMake build configuration (if using CMake)
└── README.md                       # Project documentation

Usage

    Load the Plugin: After compiling, load the plugin in a supported DAW or run as a standalone application if built as such.
    Control Spatial Parameters:
        Azimuth: Adjust horizontal angle of the sound source (-180° to 180°).
        Elevation: Adjust vertical angle of the sound source (-20° to 20°).
        Distance: Control gain to simulate sound proximity.
    HRTF Files: The plugin loads HRTF impulse responses based on the current azimuth and elevation values. Ensure HRTF files are available in the resources/hrtf/ directory.

Code Overview

    AudioPluginAudioProcessor: Main processor class, handles the audio processing pipeline, HRTF convolution, and parameter updates.
    HRTFProcessor: Responsible for loading HRTF files based on azimuth/elevation and preparing IRs for convolution.
    AudioPluginAudioProcessorEditor: User interface setup, handles slider controls for spatial parameters and connects UI elements with the audio processor.

HRTF Processing Details

The plugin uses HRTF (Head-Related Transfer Function) data to model sound source positioning in 3D space. Each HRTF impulse response represents the sound as perceived from a specific direction, allowing the plugin to create a natural, immersive audio experience.
Parameters
Parameter	Description	Range
Azimuth	Horizontal angle	-180° to 180°
Elevation	Vertical angle	-20° to 20°
Distance	Distance-based attenuation	-60 dB to 0 dB
Contributing

Contributions are welcome! Please fork this repository, create a branch, and submit a pull request for any improvements or bug fixes.
Issues

If you encounter bugs or have feature requests, please report them in the Issues section of this repository.
License

This project is licensed under the MIT License - see the LICENSE file for details.
Acknowledgments

Special thanks to the open-source audio community and JUCE for their continuous support and resources.

# TODO
  - SOFA HRTF (libmysofa?)
  - 3D scene (OpenGL?)

# Features
  - XY(Z?) Pad
  - HRTF Audio
  - ?

# Useful Links
- [3D Audio Demo](https://www.youtube.com/watch?v=a4mpK_2koR4&t=425s)

- [XY Pad](https://www.youtube.com/watch?v=6aRu9RKkx8E)

- [Example Plugin](https://www.youtube.com/watch?time_continue=50&v=BuyEh67dDNs&embeds_referring_euri=https%3A%2F%2Fwww.google.com%2F&source_ve_path=MjM4NTE)

- [KD-Trees](https://en.wikipedia.org/wiki/K-d_tree)
