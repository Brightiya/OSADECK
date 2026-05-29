#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
    : playlistComponent(formatManager,
                        &player1,
                        &player2,
                        &player3),

      deckGUI1(&player1,
               formatManager,
               waveformCache,
               &playlistComponent,
               leftDeck),

      deckGUI2(&player2,
               formatManager,
               waveformCache,
               &playlistComponent,
               middleDeck),

      deckGUI3(&player3,
               formatManager,
               waveformCache,
               &playlistComponent,
               rightDeck)
{
    setSize(1500, 1050);

    //==============================================================================
    // Register Supported Audio Formats

    formatManager.registerBasicFormats();

    //==============================================================================
    // Audio Permissions

    if (juce::RuntimePermissions::isRequired(
            juce::RuntimePermissions::recordAudio)
        &&
        !juce::RuntimePermissions::isGranted(
            juce::RuntimePermissions::recordAudio))
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
    // Visible Components

    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(deckGUI3);

    addAndMakeVisible(playlistComponent);

    addAndMakeVisible(titleLabel);
    addAndMakeVisible(subTitleLabel);

    //==============================================================================
    // Title Label

    titleLabel.setText(
        "OSADECK PRO",
        juce::dontSendNotification);

    titleLabel.setFont(
        juce::Font(32.0f,
                   juce::Font::bold));

    titleLabel.setJustificationType(
        juce::Justification::centred);

    titleLabel.setColour(
        juce::Label::textColourId,
        juce::Colours::white);

    //==============================================================================
    // Subtitle Label

    subTitleLabel.setText(
        "Professional Multi-Deck DJ Mixing Console",
        juce::dontSendNotification);

    subTitleLabel.setFont(
        juce::Font(16.0f));

    subTitleLabel.setJustificationType(
        juce::Justification::centred);

    subTitleLabel.setColour(
        juce::Label::textColourId,
        juce::Colours::lightgrey);

    //==============================================================================
    // Component Rendering

    setOpaque(true);

    //==============================================================================
    // Onboarding Popup

    juce::AlertWindow::showMessageBoxAsync(
        juce::AlertWindow::InfoIcon,
        "Welcome to OSADECK PRO",

        "Quick Start Guide:\n\n"
        "1. Press LOAD to import audio files.\n"
        "2. Press PLAY to start playback.\n"
        "3. Adjust volume, speed, and effects.\n"
        "4. Use LOAD NEXT to load queued playlist tracks.\n"
        "5. Drag and drop audio files directly onto a deck.\n\n"
        "Enjoy mixing with OSADECK PRO!",
        "START MIXING");
}

//==============================================================================
MainComponent::~MainComponent()
{
    deviceManager.removeAllChangeListeners();

    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(
    int samplesPerBlockExpected,
    double sampleRate)
{
    mixerSource.prepareToPlay(
        samplesPerBlockExpected,
        sampleRate);

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
    auto bounds =
        getLocalBounds().toFloat();

    //==============================================================================
    // Main Background Gradient

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
    // Header Banner

    g.setColour(
        juce::Colour(25, 25, 35));

    g.fillRoundedRectangle(
        bounds.removeFromTop(80.0f).reduced(10.0f),
        15.0f);

    //==============================================================================
    // Playlist Background Area

    auto playlistArea =
        juce::Rectangle<float>(
            10.0f,
            static_cast<float>(getHeight() / 2) + 10.0f,
            static_cast<float>(getWidth()) - 20.0f,
            static_cast<float>(getHeight() / 2) - 20.0f);

    g.setColour(
        juce::Colour(28, 28, 38));

    g.fillRoundedRectangle(
        playlistArea,
        15.0f);

    //==============================================================================
    // Playlist Border

    g.setColour(
        juce::Colours::grey.withAlpha(0.4f));

    g.drawRoundedRectangle(
        playlistArea,
        15.0f,
        1.5f);

    //==============================================================================
    // Playlist Title

    g.setColour(
        juce::Colours::white.withAlpha(0.9f));

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
void MainComponent::resized()
{
    auto area =
        getLocalBounds().reduced(10);

    //==============================================================================
    // Header Section

    auto headerArea =
        area.removeFromTop(80);

    titleLabel.setBounds(
        headerArea.removeFromTop(40));

    subTitleLabel.setBounds(
        headerArea);

    area.removeFromTop(10);

    //==============================================================================
    // Deck Section

    auto deckArea =
        area.removeFromTop(520);

    constexpr int spacing = 10;

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

    //==============================================================================
    // Playlist Section

    playlistComponent.setBounds(area);
}
