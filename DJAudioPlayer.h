/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 23 Feb 2024 11:04:59pm
    Author:  Bright  Osahenhen Iyahen

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DJAudioPlayer : public juce::AudioSource{
public:
    
    
    //==============================================================================
    DJAudioPlayer(juce::AudioFormatManager& _formatManager );
    ~DJAudioPlayer();
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    /**Loads the tracks URL**/
    void loadURL(juce:: URL audioURL);
    
    /** Increase or decrease the track volume*/
    void increaseDecreaseSoundVolume(double gain);
    
    /** Fast or slow the music*/
    void fastSlowSound(double ratio);
    
    /** Fast-forward or fast-bacward the music*/
    void forwardBackwardSound(double posInSecs);
    
    /** Sets the forwardBackwardSound function relative position*/
    void forwardBackwardSoundRelative(double pos);
    
    /**Calls the transportSouce to start the music**/
    void start();
    
    /**Calls the transportSouce to stop the music**/
    void stop();
    
    /** playhead relative position*/
    double getPositionRelativeCallback();
    
    
    /**Sets the room song**/
    void roomSong(float rSize);
    
    /**Sets the wet song*/
    void wetSong(float wSize);
    
    /**Sets the dry song*/
    void drySong(float dSize);
  
private:
    /**Processes the various audio formats such mp3, .wav etc**/
    juce::AudioFormatManager& formatManager;
    
    /**The function that  reads the audio formats **/
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    
    /**The function that processes, start , stop, etc ,the audio source**/
    juce::AudioTransportSource transportSource;
    
    /**It resamples the audio source for a better sound production**/
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 3};
    
    /**It helps to give the sound music different reverbs for different music boost**/
    juce::ReverbAudioSource reverbSource{ &resampleSource, false };
    
    /**It helps to give the sound music different reverbs for different music boost being used by ReverbAudioSource function **/
    juce::Reverb::Parameters reverbParameters;
   
   
};

