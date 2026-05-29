#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  :  public juce::AudioAppComponent
                       
                        

{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    
    void resized() override;
    
  
    
   
      
    
    

private:
    
  
    
  /**Default Decks number**/
    int leftDeck = 0;
    int midDeck = 1;
    int rightDeck = 2;
   
   
    /**Processes the various audio formats such mp3, .wav etc**/
    juce::AudioFormatManager formatManager;
    
    /**Sets the AudioThumbnailCache**/
    juce::AudioThumbnailCache thumbCache{100};
    
    /**Formats the audio file in the Left Deck**/
    DJAudioPlayer player1{formatManager};
   
    /**Calls the DeckGUI constructor to process audio files on the Left Deck**/
    DeckGUI deckGUI1{&player1, formatManager, thumbCache, &playlistComponent, leftDeck};
    
    /**Formats the audio file in the Middle Deck**/
    DJAudioPlayer player2{formatManager};
   
    
    /**Calls the DeckGUI constructor to process audio files on the Middle Deck**/
    DeckGUI deckGUI2{&player2, formatManager, thumbCache, &playlistComponent, midDeck};
    
    /**Formats the audio file in the Right Deck**/
    DJAudioPlayer player3{formatManager};
    
    /**Calls the DeckGUI constructor to process audio files on the Right Deck**/
    DeckGUI deckGUI3{&player3, formatManager, thumbCache, &playlistComponent, rightDeck};
    
    /**Implement mixing of the audioSource**/
    juce::MixerAudioSource mixerSource;
    
    /**Calls the PlaylistComponent constructor for audio processing on the playlistComponent**/
    PlaylistComponent playlistComponent{formatManager, &player1, &player2, &player3};
    
    /**attaches text to the playlistComponent**/
    juce::Label labelPlaylist;
    
    /**attaches text to the DeckGUI component**/
    juce::Label labelGUI;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
