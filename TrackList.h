/*
  ==============================================================================

    TrackList.h
    Created: 1 Mar 2024 12:31:35pm
    Author:  Bright  Osahenhen Iyahen

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

 class TrackList
{
public:
    
    
    TrackList(juce::File getFile);
    
    /** Gets the  full track file from playlist or device */
    juce::File trackFile;
    /** Gets the URL root of the track file.*/
    juce::URL getURL;
    /** Gets the track tile*/
    juce::String trackTitle;
         
   
};
