/*
  ==============================================================================

    WaveformDisplay.h
    Created: 23 Feb 2024 11:02:03pm
    Author:  Bright  Osahenhen Iyahen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveFormDisplay  : public juce::Component, 
                         public juce::ChangeListener,
                         public juce::AudioTransportSource
                        
{
public:
    WaveFormDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse);
    ~WaveFormDisplay() override;
    /**Draws the any objects on screen**/
    void paint (juce::Graphics&) override;
    
    /**Resizes and positons objects on screen**/
    void resized() override;
    
    

    /**Receives and broadcasts events**/
    void changeListenerCallback(juce::ChangeBroadcaster *source) override;
    
    /**Loads sound files into Decks**/
    void loadURL(juce::URL audioURL);
    
    /** set the relative position of the playhead */
    void setPositionRelative(double posCallback); // gets its callback from player
    
    /**Sets the text and colour when no file is loaded**/
    void getNoFileLoadedPainted (juce::Graphics& g);
    
    /**Draws the playhead and colour when  a file is loaded into the deck**/
    void getFileLoadedPainted (juce::Graphics& g);
    
    /**Implements the AudioThumbnail class**/
    juce::AudioThumbnail audioThumb;

private:
    
    /**Implements the AudioTransportSource class**/
    juce::AudioTransportSource transportSource;
    
    /**Bool variable thats tracks if track is loaded or not**/
    bool isFileLoaded;
    
    /**Variable that tracks the position of the playhead**/
   double position;
    
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveFormDisplay)
};
