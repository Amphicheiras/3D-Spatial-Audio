cmake_minimum_required(VERSION 3.5)

project(3DAudioPlugin VERSION 0.1.0)

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/3DAudioPlugin")

# find_package(mysofa CONFIG REQUIRED)

juce_add_plugin(${PROJECT_NAME}
    COMPANY_NAME UF0
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    PLUGIN_MANUFACTURER_CODE UF0
    PLUGIN_CODE 3DSA
    FORMATS Standalone VST3
    PRODUCT_NAME "3D-SPATIAL-AUDIO"
)

target_sources(${PROJECT_NAME}
    PRIVATE
        src/PluginEditor.cpp
        src/PluginProcessor.cpp
        src/XYPad.cpp
        src/HRTFProcessor.cpp
        ${INCLUDE_DIR}/PluginEditor.h
        ${INCLUDE_DIR}/PluginProcessor.h
        ${INCLUDE_DIR}/XYPad.h
        ${INCLUDE_DIR}/HRTFProcessor.h
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        juce::juce_audio_utils
        juce::juce_gui_basics
        juce::juce_core
        juce::juce_audio_formats
        juce::juce_audio_basics
        juce::juce_dsp
        # mysofa::mysofa-shared 
        # mysofa::mysofa-static depending on your preference
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/..)