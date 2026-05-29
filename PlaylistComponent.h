/*
  ==============================================================================

    PlaylistComponent.h
    Created: 23 Feb 2024 11:01:05pm
    Author:  Bright Osahenhen Iyahen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"

#include <vector>
#include <string>

//==============================================================================
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener,
                           public juce::AudioSource,
                           public juce::FileDragAndDropTarget
{
public:
    PlaylistComponent(juce::AudioFormatManager& formatManager,
                      DJAudioPlayer* player1,
                      DJAudioPlayer* player2,
                      DJAudioPlayer* player3);

    ~PlaylistComponent() override;

    //==============================================================================
    // Component
    void paint(juce::Graphics&) override;
    void resized() override;

    //==============================================================================
    // TableListBoxModel
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

    juce::Component* refreshComponentForCell(int rowNumber,
                                             int columnId,
                                             bool isRowSelected,
                                             juce::Component* existingComponentToUpdate) override;

    //==============================================================================
    // Button Listener
    void buttonClicked(juce::Button* button) override;

    //==============================================================================
    // AudioSource
    void prepareToPlay(int samplesPerBlockExpected,
                       double sampleRate) override;

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    void releaseResources() override;

    //==============================================================================
    // Drag & Drop
    bool isInterestedInFileDrag(const juce::StringArray& files) override;

    void filesDropped(const juce::StringArray& files,
                      int x,
                      int y) override;

    //==============================================================================
    // Playlist Logic
    void loadTrackToDeck(const std::string& filePath,
                         int deckIndex);

    void loadAudioFile(const juce::URL& audioURL);

    double extractFileLengthInSeconds() const;

    //==============================================================================
    // File Helpers
    std::string getFilePath(const juce::String& fromFilename);

    std::string getFileNameWithoutExtension(const juce::String& fromFilename);

    //==============================================================================
    // Deck Queues
    std::vector<std::string> leftStore;
    std::vector<std::string> midStore;
    std::vector<std::string> rightStore;

    //==============================================================================
    // Table Component
    juce::TableListBox tableComponent;

private:
    //==============================================================================
    struct TrackInfo
    {
        std::string filePath;
        std::string title;
        double duration = 0.0;
    };

    //==============================================================================
    // Helpers
    void setupTable();

    juce::String formatTime(double seconds) const;

    void addTrack(const juce::File& file);

    void removeTrack(int index);

    bool isValidRow(int row) const;

    //==============================================================================
    // Audio
    juce::AudioFormatManager& formatManager;

    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;

    juce::AudioTransportSource transportSource;

    //==============================================================================
    // Track Storage
    std::vector<TrackInfo> tracks;

    //==============================================================================
    // Players
    DJAudioPlayer* player1 = nullptr;
    DJAudioPlayer* player2 = nullptr;
    DJAudioPlayer* player3 = nullptr;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};