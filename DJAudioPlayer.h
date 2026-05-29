/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 23 Feb 2024 11:04:59pm
    Author:  Bright  Osahenhen Iyahen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
class DJAudioPlayer : public juce::AudioSource
{
public:
    //==============================================================================
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    ~DJAudioPlayer() override;

    //==============================================================================
    // AudioSource overrides
    void prepareToPlay(int samplesPerBlockExpected,
                       double sampleRate) override;

    void getNextAudioBlock(
        const juce::AudioSourceChannelInfo& bufferToFill) override;

    void releaseResources() override;

    //==============================================================================
    // File Loading

    /** Loads an audio file from a URL */
    void loadURL(juce::URL audioURL);

    //==============================================================================
    // Playback Controls

    /** Starts audio playback */
    void start();

    /** Stops audio playback */
    void stop();

    //==============================================================================
    // Audio Controls

    /** Sets playback volume (0.0 - 1.0) */
    void increaseDecreaseSoundVolume(double gain);

    /** Sets playback speed/resampling ratio */
    void fastSlowSound(double ratio);

    /** Sets playback position in seconds */
    void forwardBackwardSound(double posInSecs);

    /** Sets playback position relative to track length (0.0 - 1.0) */
    void forwardBackwardSoundRelative(double pos);

    /** Returns current playback position relative to track length */
    double getPositionRelativeCallback();

    //==============================================================================
    // Reverb Controls

    /** Controls reverb room size */
    void roomSong(float rSize);

    /** Controls wet signal level */
    void wetSong(float wSize);

    /** Controls dry signal level */
    void drySong(float dSize);

private:
    //==============================================================================
    // Audio Management

    /** Handles supported audio file formats */
    juce::AudioFormatManager& formatManager;

    /** Reads audio data from loaded files */
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;

    /** Main transport controller for playback */
    juce::AudioTransportSource transportSource;

    /** Handles playback speed/resampling */
    juce::ResamplingAudioSource resampleSource;

    /** Applies reverb effect processing */
    juce::ReverbAudioSource reverbSource;

    /** Stores reverb effect parameters */
    juce::Reverb::Parameters reverbParameters;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DJAudioPlayer)
};