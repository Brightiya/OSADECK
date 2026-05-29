/*
  ==============================================================================

    DeckGUI.h
    Created: 23 Feb 2024 11:03:24pm
    Author:  Bright  Osahenhen Iyahen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include "DJAudioPlayer.h"

#include "WaveFormDisplay.h"
#include "TrackList.h"





//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
                 public juce::Button::Listener,
                 public juce::Slider::Listener,
                 public juce::FileDragAndDropTarget,
                 public juce::Timer,
                 public juce::TableListBoxModel,
                 public juce::TextEditor::Listener
                
                 
                 
                
                 
              
            
                
              

                
{
public:
    DeckGUI(DJAudioPlayer* _player, juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse,  PlaylistComponent* playlistComponent, int channelInUse);
    
    ~DeckGUI() override;
/**IDraws objects on screen**/
    void paint (juce::Graphics& g) override;
    /**Resizes and positions object on screen**/
    void resized() override;
   
  
    
/** Implement  Button::Listerner */
    void buttonClicked(juce::Button*) override;
    /** Implement  Slider::Listerner */
    void sliderValueChanged(juce::Slider* slider) override;
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    /**Implement the Timer class**/
    void timerCallback() override;
   
   
    
    int getNumRows() override;
      
    void paintRowBackground(juce::Graphics&,
           int rowNumber,
           int width,
           int height,
           bool rowIsSelected) override;
      
   
    void paintCell(juce::Graphics&,
           int rowNumber,
           int columnId,
           int width,
           int height,
           bool rowIsSelected) override;
    
    
    /**Gets the  tracks file ful pathl**/
    std::string getFileFullPath(const std::string &fromFilepath);

    /**Loads tracks into Decks**/
    juce::TextButton trackButton{ "LOAD TRACK" };
    
   
   /**Finds the title text of the track if any */
    void findTrackInPlaylist(juce::String text);
    
    /**Extracts and highlights  the tract title aqua colour as  found by  findTrackInPlaylist function**/
    int getTrackInPlaylist(juce::String text);
   
    
    
private:
    /**Plays tracks loaded into Deck**/
    juce::TextButton playButton{"PLAY"};
    
    /**Stops tracks loaded into Deck**/
    juce::TextButton stopButton{"STOP"};
   /**Loads track files from the system into the Deck**/
    juce::TextButton loadButton{"LOAD FILE"};
    
   
    /**Controls the track volume**/
    juce::Slider volSlider;
    
    /**Attaches text to the left of the volumeSlider**/
    juce::Label volumeLabel;
    
    /**Controls the speed of the sound track**/
    juce::Slider speedSlider;
    
    /**Attaches text to the left of the speedSlider**/
    juce::Label speedLabel;
    
    /**Forwards or backwards the track **/
    juce::Slider posSlider;
    
    /**Attaches text to the left of the posSlider**/
    juce::Label forwardBackwardLabel;
    
    /**Sets the room sound of tracks**/
    juce::Slider roomSlider;
    
    /**Attaches text to the left of the roomSlider**/
    juce::Label roomLabel;
    
    /**Sets the wet sound of tracks**/
    juce::Slider wetLevelSlider;
    
    /**Attaches text to the left of the wetLevelSlider**/
    juce::Label wetLevelLabel;
    
    /**Sets the dry sound of tracks**/
    juce::Slider dryLevelSlider;
    
    /**Attaches text to the left of the dryLevelSlider**/
    juce::Label dryLevelLabel;
  
    /**Implements the selection of  a track file from the system**/
    juce::FileChooser fChooser{"Select a file..."};
    
    /**Pointer to the DJAudioPlayer class to implement some of its functions**/
    DJAudioPlayer* player;
    
    /**Connects to the WaveFormDisplay class to implement some of its functions**/
    WaveFormDisplay waveFormDisplay;
    
    /**Points to the playlistComponent class to implement some of its functions**/
    PlaylistComponent* playlistComponent;
    
    
    
    /**variable indicating channel associated with the GUI (0=Left, 1=Middle, 2=Right)**/
    int inChannel;
    
    /**Displays text for the time when the time hasn't started**/
    juce::Label currentTimePositionLabel;
    
    /**Vector of type TrackList stores tracks**/
    std::vector<TrackList> titledTracks;
    
    /**Enables text entry for searching the playlist**/
    juce::TextEditor playlistEditor;
  
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
