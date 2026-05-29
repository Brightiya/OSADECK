/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 23 Feb 2024 11:02:03pm
    Author:  Bright Osahenhen Iyahen

    Professional Waveform Display Implementation

  ==============================================================================
*/

#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                                 juce::AudioThumbnailCache& cacheToUse)
    : audioThumbnail(1000, formatManagerToUse, cacheToUse)
{
    // Listen for waveform thumbnail changes
    audioThumbnail.addChangeListener(this);
}

//==============================================================================
WaveformDisplay::~WaveformDisplay()
{
    audioThumbnail.removeChangeListener(this);
}

//==============================================================================
void WaveformDisplay::paint(juce::Graphics& g)
{
    // Background
    g.fillAll(juce::Colour(20, 20, 20));

    // Draw waveform or placeholder
    if (fileLoaded)
        drawWaveform(g);
    else
        drawPlaceholder(g);

    // Draw border
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);
}

//==============================================================================
void WaveformDisplay::resized()
{
    // Reserved for future responsive layout logic
}

//==============================================================================
void WaveformDisplay::drawPlaceholder(juce::Graphics& g)
{
    g.setColour(juce::Colours::darkred.withAlpha(0.8f));
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 8.0f);

    g.setColour(juce::Colours::white);
    g.setFont(18.0f);

    g.drawFittedText("No Audio File Loaded",
                     getLocalBounds(),
                     juce::Justification::centred,
                     1);
}

//==============================================================================
void WaveformDisplay::drawWaveform(juce::Graphics& g)
{
    auto bounds = getLocalBounds();

    // Background panel
    g.setColour(juce::Colour(30, 30, 30));
    g.fillRoundedRectangle(bounds.toFloat(), 8.0f);

    // Draw waveform
    g.setColour(juce::Colours::deepskyblue);

    audioThumbnail.drawChannels(
        g,
        bounds.reduced(4),
        0.0,
        audioThumbnail.getTotalLength(),
        1.0f
    );

    // Draw playhead
    drawPlayhead(g);
}

//==============================================================================
void WaveformDisplay::drawPlayhead(juce::Graphics& g)
{
    auto playheadX = static_cast<int>(playheadPosition * getWidth());

    playheadX = juce::jlimit(0, getWidth(), playheadX);

    g.setColour(juce::Colours::yellow);

    g.drawLine(
        static_cast<float>(playheadX),
        0.0f,
        static_cast<float>(playheadX),
        static_cast<float>(getHeight()),
        2.0f
    );
}

//==============================================================================
void WaveformDisplay::loadURL(const juce::URL& audioURL)
{
    // Clear previous waveform
    audioThumbnail.clear();

    // Load new source
    fileLoaded = audioThumbnail.setSource(
        new juce::URLInputSource(audioURL)
    );

    if (!fileLoaded)
    {
        juce::AlertWindow::showMessageBoxAsync(
            juce::AlertWindow::WarningIcon,
            "Waveform Display",
            "Failed to load audio waveform.",
            "OK"
        );
    }

    repaint();
}

//==============================================================================
void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    // Thumbnail updated
    if (source == &audioThumbnail)
    {
        repaint();
    }
}

//==============================================================================
void WaveformDisplay::setPositionRelative(double relativePosition)
{
    // Prevent invalid values
    if (std::isnan(relativePosition) || std::isinf(relativePosition))
        return;

    // Clamp between 0.0 and 1.0
    playheadPosition = juce::jlimit(0.0, 1.0, relativePosition);

    repaint();
}
