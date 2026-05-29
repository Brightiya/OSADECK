/*
  ==============================================================================

    TrackList.h
    Created: 1 Mar 2024 12:31:35pm
    Author:  Bright Osahenhen Iyahen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
class TrackList
{
public:
    //==============================================================================
    explicit TrackList(juce::File file);

    //==============================================================================
    // Track Information

    /** Full audio file reference */
    juce::File trackFile;

    /** URL representation of the audio file */
    juce::URL getURL;

    /** Track title without extension */
    juce::String trackTitle;

    //==============================================================================
    // Utility Methods

    /** Returns true if the file exists */
    bool fileExists() const
    {
        return trackFile.existsAsFile();
    }

    /** Returns file extension (mp3, wav, aiff, etc.) */
    juce::String getFileExtension() const
    {
        return trackFile.getFileExtension();
    }

    /** Returns full file path */
    juce::String getFullPathName() const
    {
        return trackFile.getFullPathName();
    }

    /** Returns file size in MB */
    double getFileSizeMB() const
    {
        return static_cast<double>(trackFile.getSize()) / (1024.0 * 1024.0);
    }

private:
    JUCE_LEAK_DETECTOR(TrackList)
};