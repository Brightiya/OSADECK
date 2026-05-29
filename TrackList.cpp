/*
  ==============================================================================

    TrackList.cpp
    Created: 1 Mar 2024 12:31:35pm
    Author:  Bright  Osahenhen Iyahen

  ==============================================================================
*/

#include "TrackList.h"
#include <filesystem>

TrackList::TrackList(juce::File getFile):trackFile(getFile),getURL(juce::URL{ getFile }),
trackTitle(getFile.getFileNameWithoutExtension()){}


