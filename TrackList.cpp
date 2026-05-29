/*
  ==============================================================================

    TrackList.cpp
    Created: 1 Mar 2024 12:31:35pm
    Author:  Bright Osahenhen Iyahen

  ==============================================================================
*/

#include "TrackList.h"

#include <utility>

//==============================================================================
TrackList::TrackList(juce::File file)
    : trackFile(std::move(file)),
      getURL(trackFile),
      trackTitle(trackFile.getFileNameWithoutExtension())
{
    // Ensure track title is never empty
    if (trackTitle.trim().isEmpty())
    {
        trackTitle = "Unknown Track";
    }
}