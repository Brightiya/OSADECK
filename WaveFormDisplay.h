/*
  ==============================================================================

    WaveformDisplay.h
    Created: 23 Feb 2024 11:02:03pm
    Author:  Bright Osahenhen Iyahen

    Professional Waveform Display Component
    Improved structure, naming, comments, and rendering support.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    WaveformDisplay

    Responsibilities:
    - Render waveform thumbnails
    - Display playback position/playhead
    - Handle audio file loading
    - React to thumbnail changes
*/
class WaveformDisplay : public juce::Component,
                        public juce::ChangeListener
{
public:
    //==========================================================================
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                    juce::AudioThumbnailCache& cacheToUse);

    ~WaveformDisplay() override;

    //==========================================================================
    // Component overrides

    void paint(juce::Graphics& g) override;
    void resized() override;

    //==========================================================================
    // ChangeListener override

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    //==========================================================================
    // Public API

    /**
        Loads an audio file into the waveform display.
    */
    void loadURL(const juce::URL& audioURL);

    /**
        Updates the playhead position.

        @param relativePosition Value between 0.0 and 1.0
    */
    void setPositionRelative(double relativePosition);

private:
    //==========================================================================
    // Helper rendering methods

    void drawPlaceholder(juce::Graphics& g);
    void drawWaveform(juce::Graphics& g);
    void drawPlayhead(juce::Graphics& g);

    //==========================================================================
    // Audio thumbnail system

    juce::AudioThumbnail audioThumbnail;

    //==========================================================================
    // State variables

    bool fileLoaded { false };

    // Current playback position (0.0 -> 1.0)
    double playheadPosition { 0.0 };

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};