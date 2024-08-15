cmake_minimum_required(VERSION 3.24.1 FATAL_ERROR)

# ================================================================================
# foleys_license_client juce plugin example
# ================================================================================

include(juce)


juce_add_plugin(FoleysJuceLicenseExample
        PRODUCT_NAME LicenseExample
        COMPANY_NAME Manufacturer
        PLUGIN_MANUFACTURER_CODE Manu
        PLUGIN_CODE Lics
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        COPY_PLUGIN_AFTER_BUILD FALSE
        FORMATS AU VST3 Standalone
        BUNDLE_ID com.example.foleyslicenseexample)

target_sources(FoleysJuceLicenseExample
        PRIVATE
        LicensingDemoProcessor.cpp
        LicensingDemoProcessor.h
        LicensingDemoEditor.cpp
        LicensingDemoEditor.h
        LicensePanel.cpp
        LicensePanel.h
        LicenseData.cpp)

target_compile_definitions(FoleysJuceLicenseExample
        PRIVATE
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(FoleysJuceLicenseExample
        PRIVATE
        licensing
        juce::juce_audio_processors
        juce::juce_audio_devices
        juce::juce_audio_utils)
