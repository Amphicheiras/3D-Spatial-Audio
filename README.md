# HRTF 3D Spatial Audio Plugin

An audio plugin for creating immersive 3D spatial sound experiences using Head-Related Transfer Function (HRTF) processing. This plugin allows audio positioning in three-dimensional space through azimuth and elevation control, enabling enhanced realism for spatial audio applications such as VR, games, and music production.

<p align="center">
  <img src="https://github.com/Amphicheiras/3D-Spatial-Audio/blob/development-amphicheiras/github/spatialAudioC.png?raw=true" alt="center" width="250">
  <img src="https://github.com/Amphicheiras/3D-Spatial-Audio/blob/development-amphicheiras/github/spatialAudioL.png?raw=true" alt="left" width="250">
  <img src="https://github.com/Amphicheiras/3D-Spatial-Audio/blob/development-amphicheiras/github/spatialAudioR.png?raw=true" alt="right" width="250">
</p>

## Features

- Azimuth & Elevation Controls: Real-time control over sound source direction using azimuth (horizontal angle) and elevation (vertical angle).
- Distance-Based Attenuation: Simulates distance by controlling gain levels, allowing dynamic volume adjustments based on the source's proximity.
- HRTF Convolution Processing: Utilizes impulse responses (IRs) based on HRTF data for accurate spatialization of audio.
- Real-Time Parameter Updates: Changes to azimuth, elevation, and distance are applied in real-time with low latency.
- Stereo Peak Level Metering: Instant monitoring of the plugin's effect on the left and right channel color-coded volumes.
- User-Friendly Interface: Intuitive controls for adjusting spatial parameters within the plugin editor.

## Getting Started
### Prerequisites

- JUCE Framework: Required for building and running the plugin. Download from JUCE.
- Supported DAWs: Any Digital Audio Workstation that supports VST, AU, or standalone plugins.
- Compiler: Ensure a compatible C++ compiler is installed. For Windows, MSVC is recommended, while Xcode works well on macOS.

### Installation

1. Clone the repository:
    ```sh
     git clone https://github.com/Amphicheiras/3D-Spatial-Audio.git
     cd 3D-Spatial-Audio 
    ```

2. Have a look at the /root and /plugin CMakeLists.txt

3. Configure with CMake:
   ```sh
   cmake -S . -B build
   ```

4. Compile with CMake:
   ```sh
   cmake --build build
   ```
   
5. Place "UF0/3DSA/resources" in your OS's "Documents" folder

### Usage

1. Load the Plugin:
   After compiling, load the plugin in a supported DAW or run as a standalone application if built as such.
2. Control Spatial Parameters:
   - Azimuth: Adjust horizontal angle of the sound source (-180° to 180°).
   - Elevation: Adjust vertical angle of the sound source (-20° to 20°).
   - Distance: Control gain to simulate sound proximity.
3. HRTF Files:
   The plugin loads HRTF impulse responses based on the current azimuth and elevation values. Ensure HRTF files are available in the resources/hrtf/ directory.

### Code Overview

- PluginProcessor: Main processor class, handles the audio processing pipeline, HRTF convolution, and parameter updates.
- PluginEditor: User interface setup, handles slider controls for spatial parameters and connects UI elements with the audio processor.
- HRTFProcessor: Responsible for loading HRTF files based on azimuth/elevation and preparing IRs for convolution.

### HRTF Processing Details

The plugin uses HRTF (Head-Related Transfer Function) data to model sound source positioning in 3D space. Each HRTF impulse response represents the sound as perceived from a specific direction, allowing the plugin to create a natural, immersive audio experience.

### Parameters
Parameter	Description	Range
- Azimuth	Horizontal angle	-180° to 180°
- Elevation	Vertical angle	-20° to 20°
- Distance	Distance-based attenuation	-60 dB to 0 dB

### Contributing

Contributions are welcome! Please fork this repository, create a branch, and submit a pull request for any improvements or bug fixes.

### Issues

If you encounter bugs or have feature requests, please report them in the Issues section of this repository.

### License

This project is licensed under the GNU GPL-3.0 License - see the LICENSE file for details.

### Acknowledgments

Special thanks to the open-source audio community and JUCE for their continuous support and resources.

Super special thanks to Christos Konstantas and Jan Wilczek!

# Useful Links
- [3D Audio Demo](https://www.youtube.com/watch?v=a4mpK_2koR4&t=425s)

- [XY Pad](https://www.youtube.com/watch?v=6aRu9RKkx8E)

- [Example Plugin](https://www.youtube.com/watch?time_continue=50&v=BuyEh67dDNs&embeds_referring_euri=https%3A%2F%2Fwww.google.com%2F&source_ve_path=MjM4NTE)

- [KD-Trees](https://en.wikipedia.org/wiki/K-d_tree)

- [JUCE with CMake](https://www.youtube.com/watch?v=Uq7Hwt18s3s)
