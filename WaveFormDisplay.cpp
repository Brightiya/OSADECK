/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 23 Feb 2024 11:02:03pm
    Author:  Bright  Osahenhen Iyahen

  ==============================================================================
*/


#include "WaveFormDisplay.h"


//==============================================================================
WaveFormDisplay::WaveFormDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse): audioThumb(1000, formatManagerToUse, cacheToUse), isFileLoaded(false), position(0.0)
{
    
    
    /**Enables event listerner on the audioThumbnail class**/
    audioThumb.addChangeListener(this);
    
  

}
/**destroys the constructor on app exit**/
WaveFormDisplay::~WaveFormDisplay()
{
    

}




void WaveFormDisplay::paint (juce::Graphics& g)
{
  
    
    /**Checks if channels number is 0 and if so colours the waveFormComponent darkRed else green**/
    if (audioThumb.getNumChannels() == 0)
    {
        g.fillAll(juce::Colours::darkred);
        
        /**Displays text:  'No File Loaded' on the waveFormComponent**/
        getNoFileLoadedPainted (g);
       
        
    }
    
    else{
        
        g.fillAll(juce::Colours::green);
        /**Sets the waveform to skyblue when a track file is loaded and draws a yellow-filled rectangle playhead**/
        getFileLoadedPainted (g);
        
    }
    
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);
 

}


void WaveFormDisplay::getNoFileLoadedPainted(juce::Graphics &g)
{
    
          
           g.setColour (juce::Colours::white);
           g.setFont (20.0f);
           g.drawText ("No File Loaded", getLocalBounds(), juce::Justification::centred, true);
}

void WaveFormDisplay::getFileLoadedPainted(juce::Graphics &g)
{
    
  
    
           g.setColour (juce::Colours::skyblue);                               
    /**Stores the total length of the sound file**/
        auto fullAudioLength = (float) audioThumb.getTotalLength();
    
    /**Draws the waveForm**/
           audioThumb.drawChannels (g,
                                   getLocalBounds(),
                                   0.0,
                                  fullAudioLength,
                                   1.0f);
   
          g.setColour (juce::Colours::yellow);
   /**Draws a yellow-filled rectangle that moves along when the tracks play**/
          int playHeadX = (int)(position * getWidth()); playHeadX = juce::jlimit(0, getWidth(), playHeadX); int playHeadWidth = juce::jmax(1, getWidth() / 40); g.fillRect(playHeadX, 0, playHeadWidth, getHeight());
   
}

void WaveFormDisplay::resized(){}

/**Waveform is loaded , so take in the source from the audioThumbnail else display 'Waveform not loaded'**/

void WaveFormDisplay::loadURL(juce::URL audioURL) { audioThumb.clear();
    isFileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
    if (!isFileLoaded)
    { juce::AlertWindow::showMessageBoxAsync( juce::AlertWindow::InfoIcon, "WaveformDisplay on Deck:", "Waveform not loaded! Please try again", "OK", nullptr ); } }


void WaveFormDisplay::changeListenerCallback(juce::ChangeBroadcaster *source)
{
   /**Draws the waveform of the sound file because of the souce file that have been received from the audioThumbnail**/
    
    if (source == &audioThumb) {
        repaint();
    }
    
  
}

void WaveFormDisplay::setPositionRelative(double posCallback)
{
    /**Draws the waveform of the track till the end as the  track is loaded and the playhead position is calculated  **/
    
    // Prevent invalid waveform positions
    if (std::isnan(posCallback) || std::isinf(posCallback))
        return;
    
    // Clamp position between 0 and 1
    position = juce::jlimit(0.0, 1.0, posCallback);
    repaint();
    
    
   
}



