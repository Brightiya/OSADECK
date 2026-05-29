#pragma once

#include <JuceHeader.h>

#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

//==============================================================================
/*
    Main application component for OSADECK PRO
*/
class MainComponent : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    // Audio Callbacks

    void prepareToPlay(int samplesPerBlockExpected,
                       double sampleRate) override;

    void getNextAudioBlock(
        const juce::AudioSourceChannelInfo& bufferToFill) override;

    void releaseResources() override;

    //==============================================================================
    // GUI Callbacks

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Deck Channel IDs

    enum DeckChannel
    {
        leftDeck   = 0,
        middleDeck = 1,
        rightDeck  = 2
    };

    //==============================================================================
    // Audio System

    /** Handles supported audio formats (.mp3, .wav, .aiff, etc.) */
    juce::AudioFormatManager formatManager;

    /** Shared waveform thumbnail cache */
    juce::AudioThumbnailCache waveformCache{ 200 };

    /** Mixer used to combine all deck audio outputs */
    juce::MixerAudioSource mixerSource;

    //==============================================================================
    // Audio Players

    DJAudioPlayer player1{ formatManager };
    DJAudioPlayer player2{ formatManager };
    DJAudioPlayer player3{ formatManager };

    //==============================================================================
    // Playlist Component

    PlaylistComponent playlistComponent
    {
        formatManager,
        &player1,
        &player2,
        &player3
    };

    //==============================================================================
    // Deck GUIs

    DeckGUI deckGUI1
    {
        &player1,
        formatManager,
        waveformCache,
        &playlistComponent,
        leftDeck
    };

    DeckGUI deckGUI2
    {
        &player2,
        formatManager,
        waveformCache,
        &playlistComponent,
        middleDeck
    };

    DeckGUI deckGUI3
    {
        &player3,
        formatManager,
        waveformCache,
        &playlistComponent,
        rightDeck
    };

    //==============================================================================
    // UI Components

    /** Main application title */
    juce::Label titleLabel;

    /** Subtitle / application description */
    juce::Label subTitleLabel;

    //==============================================================================
    // Tooltip Manager

    /** Enables all component tooltips application-wide */
    juce::TooltipWindow tooltipWindow{ this, 500 };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
