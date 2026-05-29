/*
  ==============================================================================

    PlaylistComponent.h
    Created: 23 Feb 2024 11:01:05pm
    Author:  Bright  Osahenhen Iyahen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include <vector>
#include <string>





//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,  
                           public juce::TableListBoxModel,
                           public juce::Button::Listener,
                           public juce::AudioSource,
                           public juce::FileDragAndDropTarget
                          
                          
{
public:
    PlaylistComponent(juce::AudioFormatManager& formatManager, DJAudioPlayer* _player1, DJAudioPlayer* _player2, DJAudioPlayer* _player3);
    
    /**Destroys the PlaylistComponent when the app exits**/
    ~PlaylistComponent() override;
    
    /**Draws objects on the screen**/
    void paint (juce::Graphics&) override;
    
    /**Resizes and positions objects on the screen**/
    void resized() override;
    
    /**Returns the number of  rows of the playlistComponent**/
    int getNumRows() override;
    
    /**Highlights the background rows of the playlistComponent**/
    void paintRowBackground(juce::Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;
    /**Draws each cell of the Component**/
    void paintCell(juce::Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    
    /**Updates the component**/
    juce::Component * refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component *existingComponentToUpdate) override;
    /**Implement the bottom listeners**/
    void buttonClicked(juce::Button* button) override;
    
   
    /**Makes the  audio sample ready to play**/
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    /**Gets the next audio sample**/
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    /**Releases resources after  use**/
     void releaseResources() override;
    
    /**Implement when a file is dragged**/
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    
    /**Implement when  a file is dropped **/
    void filesDropped(const juce::StringArray& files, int x, int y) override;


    /**Gets the file path of the sound file **/
    std::string getFilePath(const juce::String &fromFilename);
    
    /**Gets the slash from the sound file**/
    std::size_t getSlashInFile(const juce::String &fromFilename);
    
    /**Gets the dot from the sound file**/
    std::size_t getDotInFile(const juce::String &fromFilename);
    
    /**Gets the dot extension from the sound file**/
    std::string getDotExtension(const juce::String &fromFilename);
    
    /**Gets the full path from the sound file**/
    std::string getFileFullPath(const juce::String &fromFilename);
    
    /**Loads tracks into Decks**/
    void loadTrackToDeck(std::string useFilePath, int useDeck);
    
    /**Gets the file length in seconds**/
    double extractFileLengthInSeconds();
    
    /**Gets the audio sound file**/
    void loadAudioFile(juce::URL fromAudioURL);
    
    /**Vectors to hold the audio files in each respective Decks**/
    std::vector<std::string> leftStore;
    std::vector<std::string> midStore;
    std::vector<std::string> rightStore;
    
    /**Implement the table list of the playlistComponent**/
    juce::TableListBox tableComponent;

private:
    /**Processes the various audio formats such mp3, .wav etc**/
    juce::AudioFormatManager& formatManager;
    
    /**The function that  reads the audio formats **/
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    
    /**The function that processes, start , stop, etc ,the audio source**/
    juce::AudioTransportSource transportSource;
   
   
    /**Hold the audio files length**/
    std::vector<std::string> filesStore;
    std::vector<std::string> fullFilesStore;
    std::vector<int> fileLengthStore;
    
    /**Gets an audio player for each Deck*/
    DJAudioPlayer* player1;
    DJAudioPlayer* player2;
    DJAudioPlayer* player3;
    
    
    
   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
