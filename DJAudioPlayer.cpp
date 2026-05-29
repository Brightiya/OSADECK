/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 23 Feb 2024 11:04:59pm
    Author:  Bright  Osahenhen Iyahen

  ==============================================================================
*/

#include "DJAudioPlayer.h"


//==============================================================================

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager): formatManager(_formatManager)
{
    
    /** Setting the Defaults values*/
       
        reverbParameters.damping = 0;
        reverbParameters.wetLevel = 0;
        reverbParameters.dryLevel = 1.0;
        reverbSource.setParameters(reverbParameters);
    
}
DJAudioPlayer::~DJAudioPlayer(){}


void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate){
    
    /** Preparing, resampling and reverbing the songs to be played sample by sample at each sampleRate respectively*/
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    reverbSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
   
}

void DJAudioPlayer::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill){
    
    /** The  reverbSource  function gets the nextAudioBlock to be played*/
    reverbSource.getNextAudioBlock(bufferToFill);
}
void DJAudioPlayer::releaseResources(){
    /** transportSource,  resampleSource and reverbSource functions release their held  audioSource resources for memory management */
    transportSource.releaseResources();
    resampleSource.releaseResources();
    reverbSource.releaseResources();

}

//==============================================================================

void DJAudioPlayer::loadURL(juce:: URL audioURL){
    /**loaURL function creates a reader to read the audioURL**/
    auto* audioReader = formatManager.createReaderFor(audioURL.createInputStream(juce::URL::InputStreamOptions(juce::URL::ParameterHandling::inAddress).withConnectionTimeoutMs(1000).withNumRedirectsToFollow(0)));
     
   
    /** The file is successfully read*/
    if (audioReader != nullptr) // good file
    {
        std::unique_ptr<juce::AudioFormatReaderSource>newSource(new juce::AudioFormatReaderSource(audioReader,true) );
        transportSource.setSource(newSource.get(), 0, nullptr, audioReader->sampleRate);
        readerSource.reset(newSource.release());
       
    }
}
void DJAudioPlayer::increaseDecreaseSoundVolume(double gain){
    /** Checks that the gain volume is not less than 0.0 or greater than 1.0 and if so transportSource sets the gain volume value*/
    if (gain < 0.0 || gain > 1.0) {
     
        
        juce::AlertWindow::showMessageBoxAsync (juce::AlertWindow::AlertIconType::InfoIcon,
                   "increaseDecreaseSoundVolume on this Deck:",
                   "Gain should be between 0 and 1",
                   "OK",
                   nullptr
               );
    }
    else{
        transportSource.setGain(gain);
        
    }
    
    
}
void DJAudioPlayer::fastSlowSound(double ratio)
{
    /** Checks that the ratio is greater than 0.0 and within safe limits **/

    if (ratio <= 0.0 || ratio > 100.0 || std::isnan(ratio) || std::isinf(ratio))
    {
        juce::AlertWindow::showMessageBoxAsync(
            juce::AlertWindow::AlertIconType::InfoIcon,
            "fastSlowSound on this Deck:",
            "Speed ratio should be greater than 0 and less than or equal to 100",
            "OK",
            nullptr
        );
    }
    else
    {
        // Clamp to safe range
        ratio = juce::jlimit(0.01, 100.0, ratio);

        resampleSource.setResamplingRatio(ratio);
    }
}
void DJAudioPlayer::forwardBackwardSound(double posInSecs){
    
    /**The transportSource function sets the position of the forward/backward slider**/
   transportSource.setPosition(posInSecs);
    
}

void DJAudioPlayer::forwardBackwardSoundRelative(double pos)
{
    /** Checks that the relative position is between 0.0 and 1.0 **/

    if (pos < 0.0 || pos > 1.0)
    {
        juce::AlertWindow::showMessageBoxAsync(
            juce::AlertWindow::AlertIconType::InfoIcon,
            "forwardBackwardSoundRelative on this Deck:",
            "Position should be between 0.0 and 1.0",
            "OK",
            nullptr
        );
    }
    else
    {
        auto posInSecs = transportSource.getLengthInSeconds() * pos;

        forwardBackwardSound(posInSecs);
    }
}

void DJAudioPlayer::start(){
    /**transportSource function starts the audioSource**/
    transportSource.start();
    
}
void DJAudioPlayer::stop(){
    /**transportSource function stops the audioSource**/
    transportSource.stop();
    
}

double DJAudioPlayer::getPositionRelativeCallback()
{
    /** Safely calculates current playback position **/

    double trackLength = transportSource.getLengthInSeconds();

    if (trackLength <= 0.0)
        return 0.0;

    auto pos = transportSource.getCurrentPosition() / trackLength;

    return juce::jlimit(0.0, 1.0, pos);
}


void DJAudioPlayer::roomSong(float rSize)
{
    /** Checks that the room size is not less than 0.0 or greater than 1.0 and if so the room size is set by the reverbSource function with the help of the reverbParameters function*/
    
    if (rSize < 0 || rSize > 1.0)
    {
        juce::AlertWindow::showMessageBoxAsync (juce::AlertWindow::AlertIconType::InfoIcon,
                   "RoomSong on this Deck:",
                   "Size should be between 0 and 1.0",
                   "OK",
                   nullptr
               );
    }
    else {
        reverbParameters.roomSize = rSize;
        reverbSource.setParameters(reverbParameters);
    }
}




void DJAudioPlayer::wetSong(float wSize)
{
    /** Checks that the wet size is not less than 0.0 or greater than 1.0 and if so the room size is set by the reverbSource function with the help of the reverbParameters function*/
  
    if (wSize < 0 || wSize > 1.0)
    {
        juce::AlertWindow::showMessageBoxAsync (juce::AlertWindow::AlertIconType::InfoIcon,
                   "WetSong on this Deck:",
                   "Size should be between 0 and 1.0",
                   "OK",
                   nullptr
               );
    }
    else {
        reverbParameters.wetLevel = wSize;
        reverbSource.setParameters(reverbParameters);
    }
}

void DJAudioPlayer::drySong(float dSize)
{
    /** Checks that the dry size is not less than 0.0 or greater than 1.0 and if so the room size is set by the reverbSource function with the help of the reverbParameters function*/
   
    if (dSize < 0 || dSize > 1.0)
    {
       
        juce::AlertWindow::showMessageBoxAsync (juce::AlertWindow::AlertIconType::InfoIcon,
                   "DrySong on this Deck:",
                   "Size should be between 0 and 1.0",
                   "OK",
                   nullptr
               );
    }
    else {
        reverbParameters.dryLevel = dSize;
        reverbSource.setParameters(reverbParameters);
    }
}
