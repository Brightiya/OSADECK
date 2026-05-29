#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
: playlistComponent(formatManager, &player1, &player2, &player3),
deckGUI1(&player1, formatManager, waveformCache, &playlistComponent, 0),
deckGUI2(&player2, formatManager, waveformCache, &playlistComponent, 1),
deckGUI3(&player3, formatManager, waveformCache, &playlistComponent, 2)
{
    setSize(1500, 1050);

    //==============================================================================
    // Audio permissions
    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
        && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request(
            juce::RuntimePermissions::recordAudio,
            [this](bool granted)
            {
                setAudioChannels(granted ? 2 : 0, 2);
            });
    }
    else
    {
        setAudioChannels(2, 2);
    }

    //==============================================================================
    // Register audio formats
    formatManager.registerBasicFormats();

    //==============================================================================
    // Visible Components
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(deckGUI3);
    addAndMakeVisible(playlistComponent);

    addAndMakeVisible(titleLabel);
    addAndMakeVisible(subTitleLabel);

    //==============================================================================
    // Title Label
    titleLabel.setText("OSADECK PRO", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(32.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);

    //==============================================================================
    // Subtitle Label
    subTitleLabel.setText("Professional Multi-Deck DJ Mixing Console",
                          juce::dontSendNotification);

    subTitleLabel.setFont(juce::Font(16.0f));
    subTitleLabel.setJustificationType(juce::Justification::centred);
    subTitleLabel.setColour(juce::Label::textColourId,
                            juce::Colours::lightgrey);

    //==============================================================================
    // Background
    setOpaque(true);
}

MainComponent::~MainComponent()
{
    deviceManager.removeAllChangeListeners();

    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected,
                                  double sampleRate)
{
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
    mixerSource.addInputSource(&player3, false);
}

//==============================================================================
void MainComponent::getNextAudioBlock(
    const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

//==============================================================================
void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
    player3.releaseResources();

    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    //==============================================================================
    // Main background gradient
    juce::ColourGradient backgroundGradient(
        juce::Colour(18, 18, 24),
        0,
        0,
        juce::Colour(32, 32, 44),
        0,
        static_cast<float>(getHeight()),
        false);

    g.setGradientFill(backgroundGradient);
    g.fillAll();

    //==============================================================================
    // Top banner
    g.setColour(juce::Colour(25, 25, 35));
    g.fillRoundedRectangle(bounds.removeFromTop(80.0f).reduced(10.0f), 15.0f);

    //==============================================================================
    // Playlist area background
    auto playlistArea = juce::Rectangle<float>(
        10.0f,
        static_cast<float>(getHeight() / 2) + 10.0f,
        static_cast<float>(getWidth()) - 20.0f,
        static_cast<float>(getHeight() / 2) - 20.0f);

    g.setColour(juce::Colour(28, 28, 38));
    g.fillRoundedRectangle(playlistArea, 15.0f);

    //==============================================================================
    // Borders
    g.setColour(juce::Colours::grey.withAlpha(0.4f));
    g.drawRoundedRectangle(
        playlistArea,
        15.0f,
        1.5f);

    //==============================================================================
    // Section Label
    g.setColour(juce::Colours::white.withAlpha(0.9f));
    g.setFont(18.0f);

    g.drawText(
        "TRACK PLAYLIST",
        30,
        getHeight() / 2 + 18,
        250,
        30,
        juce::Justification::left);
}

//==============================================================================
//==============================================================================
void MainComponent::resized()
{
    auto area = getLocalBounds().reduced(10);

    //==========================================================
    // HEADER

    auto headerArea = area.removeFromTop(80);

    titleLabel.setBounds(
        headerArea.removeFromTop(40));

    subTitleLabel.setBounds(headerArea);

    area.removeFromTop(10);

    //==========================================================
    // DECK SECTION
    // Increased height so all controls fit properly

    auto deckArea = area.removeFromTop(520);

    const int spacing = 10;

    auto deckWidth =
        (deckArea.getWidth() - (spacing * 2)) / 3;

    deckGUI1.setBounds(
        deckArea.removeFromLeft(deckWidth));

    deckArea.removeFromLeft(spacing);

    deckGUI2.setBounds(
        deckArea.removeFromLeft(deckWidth));

    deckArea.removeFromLeft(spacing);

    deckGUI3.setBounds(deckArea);

    area.removeFromTop(10);

    //==========================================================
    // PLAYLIST SECTION

    playlistComponent.setBounds(area);
}
