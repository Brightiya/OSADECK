/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 23 Feb 2024 11:04:59pm
    Author:  Bright  Osahenhen Iyahen

  ==============================================================================
*/

#include "DJAudioPlayer.h"

//==============================================================================
DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager)
    : formatManager(_formatManager),
      resampleSource(&transportSource, false, 2),
      reverbSource(&resampleSource, false)
{
    // Default reverb settings
    reverbParameters.roomSize = 0.2f;
    reverbParameters.damping = 0.5f;
    reverbParameters.wetLevel = 0.0f;
    reverbParameters.dryLevel = 1.0f;
    reverbParameters.width = 1.0f;
    reverbParameters.freezeMode = 0.0f;

    reverbSource.setParameters(reverbParameters);
}

DJAudioPlayer::~DJAudioPlayer()
{
    transportSource.stop();
    transportSource.setSource(nullptr);
}

//==============================================================================
void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    reverbSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (readerSource.get() == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    reverbSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
    reverbSource.releaseResources();
}

//==============================================================================
//==============================================================================
void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    transportSource.stop();
    transportSource.setPosition(0);

    auto inputStream =
        audioURL.createInputStream(
            juce::URL::InputStreamOptions(
                juce::URL::ParameterHandling::inAddress)
                .withConnectionTimeoutMs(3000)
        );

    if (inputStream == nullptr)
    {
        juce::AlertWindow::showMessageBoxAsync(
            juce::AlertWindow::WarningIcon,
            "Load Error",
            "Unable to open audio stream.",
            "OK"
        );

        return;
    }

    std::unique_ptr<juce::AudioFormatReader> reader(
        formatManager.createReaderFor(std::move(inputStream))
    );

    if (reader == nullptr)
    {
        juce::AlertWindow::showMessageBoxAsync(
            juce::AlertWindow::WarningIcon,
            "Unsupported File",
            "The selected audio format is not supported.",
            "OK"
        );

        return;
    }

    // IMPORTANT:
    // Get sample rate BEFORE releasing reader ownership
    double sampleRate = reader->sampleRate;

    auto newSource =
        std::make_unique<juce::AudioFormatReaderSource>(
            reader.release(),
            true
        );

    transportSource.setSource(
        newSource.get(),
        0,
        nullptr,
        sampleRate
    );

    readerSource.reset(newSource.release());
}

//==============================================================================
void DJAudioPlayer::start()
{
    if (readerSource.get() != nullptr)
    {
        transportSource.start();
    }
}

void DJAudioPlayer::stop()
{
    transportSource.stop();
}

//==============================================================================
void DJAudioPlayer::increaseDecreaseSoundVolume(double gain)
{
    gain = juce::jlimit(0.0, 1.0, gain);

    transportSource.setGain(gain);
}

void DJAudioPlayer::fastSlowSound(double ratio)
{
    ratio = juce::jlimit(0.25, 4.0, ratio);

    if (std::isnan(ratio) || std::isinf(ratio))
        return;

    resampleSource.setResamplingRatio(ratio);
}

//==============================================================================
void DJAudioPlayer::forwardBackwardSound(double posInSecs)
{
    if (posInSecs < 0.0)
        posInSecs = 0.0;

    auto trackLength = transportSource.getLengthInSeconds();

    if (trackLength <= 0.0)
        return;

    posInSecs = juce::jlimit(0.0, trackLength, posInSecs);

    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::forwardBackwardSoundRelative(double pos)
{
    pos = juce::jlimit(0.0, 1.0, pos);

    auto trackLength = transportSource.getLengthInSeconds();

    if (trackLength <= 0.0)
        return;

    forwardBackwardSound(trackLength * pos);
}

//==============================================================================
double DJAudioPlayer::getPositionRelativeCallback()
{
    auto trackLength = transportSource.getLengthInSeconds();

    if (trackLength <= 0.0)
        return 0.0;

    auto currentPosition = transportSource.getCurrentPosition();

    return juce::jlimit(0.0, 1.0, currentPosition / trackLength);
}

//==============================================================================
void DJAudioPlayer::roomSong(float rSize)
{
    reverbParameters.roomSize = juce::jlimit(0.0f, 1.0f, rSize);

    reverbSource.setParameters(reverbParameters);
}

void DJAudioPlayer::wetSong(float wSize)
{
    reverbParameters.wetLevel = juce::jlimit(0.0f, 1.0f, wSize);

    reverbSource.setParameters(reverbParameters);
}

void DJAudioPlayer::drySong(float dSize)
{
    reverbParameters.dryLevel = juce::jlimit(0.0f, 1.0f, dSize);

    reverbSource.setParameters(reverbParameters);
}
