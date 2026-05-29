/*
  ==============================================================================

    DeckGUI.h
    Created: 23 Feb 2024 11:03:24pm
    Author:  Bright Osahenhen Iyahen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "DJAudioPlayer.h"
#include "PlaylistComponent.h"
#include "TrackList.h"
#include "WaveFormDisplay.h"

//==============================================================================
/*
    DeckGUI

    Professional DJ deck interface component for:
    - Audio playback control
    - Waveform visualisation
    - FX control
    - Playlist interaction
    - Drag & drop loading
    - Track searching
*/
//==============================================================================
class DeckGUI : public juce::Component,
                public juce::Button::Listener,
                public juce::Slider::Listener,
                public juce::FileDragAndDropTarget,
                public juce::Timer,
                public juce::TableListBoxModel,
                public juce::TextEditor::Listener
{
public:
    //==============================================================================
    // Constructor / Destructor

    DeckGUI(DJAudioPlayer* player,
            juce::AudioFormatManager& formatManagerToUse,
            juce::AudioThumbnailCache& cacheToUse,
            PlaylistComponent* playlistComponent,
            int channelInUse);

    ~DeckGUI() override;

    //==============================================================================
    // Component Rendering

    void paint(juce::Graphics& g) override;
    void resized() override;

    //==============================================================================
    // Button Listener

    void buttonClicked(juce::Button* button) override;

    //==============================================================================
    // Slider Listener

    void sliderValueChanged(juce::Slider* slider) override;

    //==============================================================================
    // File Drag & Drop

    bool isInterestedInFileDrag(
        const juce::StringArray& files) override;

    void filesDropped(const juce::StringArray& files,
                      int x,
                      int y) override;

    //==============================================================================
    // Timer

    void timerCallback() override;

    //==============================================================================
    // TableListBoxModel

    int getNumRows() override;

    void paintRowBackground(juce::Graphics& g,
                            int rowNumber,
                            int width,
                            int height,
                            bool rowIsSelected) override;

    void paintCell(juce::Graphics& g,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected) override;

    //==============================================================================
    // Playlist Search

    void findTrackInPlaylist(juce::String text);

    int getTrackInPlaylist(juce::String text);

    //==============================================================================
    // Utilities

    std::string getFileFullPath(
        const std::string& fromFilepath);

    //==============================================================================
    // Public Components

    juce::TextButton trackButton{ "QUEUE" };

private:
    //==============================================================================
    // Helper Methods

    void configureSlider(juce::Slider& slider,
                         double min,
                         double max,
                         double step);

    void configureButton(juce::TextButton& button,
                         const juce::Colour& colour);

    void loadTrackToDeck(const juce::File& file);

    //==============================================================================
    // Playback Buttons

    juce::TextButton playButton{ "PLAY" };
    juce::TextButton stopButton{ "STOP" };
    juce::TextButton loadButton{ "LOAD" };

    //==============================================================================
    // Sliders

    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;
    juce::Slider roomSlider;
    juce::Slider wetLevelSlider;
    juce::Slider dryLevelSlider;

    //==============================================================================
    // Labels

    juce::Label volumeLabel;
    juce::Label speedLabel;
    juce::Label forwardBackwardLabel;
    juce::Label roomLabel;
    juce::Label wetLevelLabel;
    juce::Label dryLevelLabel;
    juce::Label currentTimePositionLabel;

    //==============================================================================
    // File Chooser

    juce::FileChooser fChooser{ "Select an audio file..." };

    //==============================================================================
    // Audio / Playlist References

    DJAudioPlayer* player = nullptr;

    WaveformDisplay waveFormDisplay;

    PlaylistComponent* playlistComponent = nullptr;

    //==============================================================================
    // Deck Information

    /*
        Deck Channel Mapping:
        0 = Left Deck
        1 = Middle Deck
        2 = Right Deck
    */
    int inChannel = 0;

    //==============================================================================
    // Track Management

    std::vector<TrackList> titledTracks;

    //==============================================================================
    // Playlist Search Editor

    juce::TextEditor playlistEditor;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
