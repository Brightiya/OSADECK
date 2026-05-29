/*
  ==============================================================================

    DeckGUI.cpp
    Created: 23 Feb 2024 11:03:24pm
    Author:  Bright Osahenhen Iyahen

  ==============================================================================
*/

#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 juce::AudioFormatManager& formatManagerToUse,
                 juce::AudioThumbnailCache& cacheToUse,
                 PlaylistComponent* _playlistComponent,
                 int channelInUse)
    : player(_player),
      waveFormDisplay(formatManagerToUse, cacheToUse),
      playlistComponent(_playlistComponent),
      inChannel(channelInUse)
{
    //==============================================================================
    // Buttons
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(trackButton);

    playButton.setButtonText("PLAY");
    stopButton.setButtonText("STOP");
    loadButton.setButtonText("LOAD");
    trackButton.setButtonText("LOAD NEXT");

    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::forestgreen);
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
    loadButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkorange);
    trackButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkcyan);

    //==============================================================================
    // Labels
    addAndMakeVisible(currentTimePositionLabel);
    addAndMakeVisible(volumeLabel);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(forwardBackwardLabel);
    addAndMakeVisible(roomLabel);
    addAndMakeVisible(wetLevelLabel);
    addAndMakeVisible(dryLevelLabel);

    currentTimePositionLabel.setText("00:00:000", juce::dontSendNotification);
    currentTimePositionLabel.setJustificationType(juce::Justification::centred);

    volumeLabel.setText("Volume", juce::dontSendNotification);
    speedLabel.setText("Speed", juce::dontSendNotification);
    forwardBackwardLabel.setText("Position", juce::dontSendNotification);
    roomLabel.setText("Room", juce::dontSendNotification);
    wetLevelLabel.setText("Wet", juce::dontSendNotification);
    dryLevelLabel.setText("Dry", juce::dontSendNotification);

    //==============================================================================
    // Waveform
    addAndMakeVisible(waveFormDisplay);

    //==============================================================================
    // Playlist search
    addAndMakeVisible(playlistEditor);

    playlistEditor.setTextToShowWhenEmpty(
        "Search loaded tracks and press ENTER...",
        juce::Colours::grey);

    playlistEditor.onReturnKey = [this]
    {
        findTrackInPlaylist(playlistEditor.getText());
    };

    //==============================================================================
    // Sliders
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(roomSlider);
    addAndMakeVisible(wetLevelSlider);
    addAndMakeVisible(dryLevelSlider);

    auto configureSlider = [](juce::Slider& slider,
                              double min,
                              double max,
                              double step)
    {
        slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        slider.setRange(min, max, step);
        slider.setColour(juce::Slider::rotarySliderFillColourId,
                         juce::Colours::deepskyblue);
        slider.setColour(juce::Slider::thumbColourId,
                         juce::Colours::white);
    };

    configureSlider(volSlider, 0.0, 1.0, 0.01);
    configureSlider(speedSlider, 0.1, 5.0, 0.01);
    configureSlider(posSlider, 0.0, 1.0, 0.001);
    configureSlider(roomSlider, 0.0, 1.0, 0.01);
    configureSlider(wetLevelSlider, 0.0, 1.0, 0.01);
    configureSlider(dryLevelSlider, 0.0, 1.0, 0.01);

    // Default values
    volSlider.setValue(0.8);
    speedSlider.setValue(1.0);
    posSlider.setValue(0.0);
    roomSlider.setValue(0.5);
    wetLevelSlider.setValue(0.3);
    dryLevelSlider.setValue(0.7);

    //==============================================================================
    // Listeners
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    trackButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    roomSlider.addListener(this);
    wetLevelSlider.addListener(this);
    dryLevelSlider.addListener(this);

    playlistEditor.addListener(this);

    //==============================================================================
    // Drag & Drop
    setInterceptsMouseClicks(true, true);

    //==============================================================================
    // Timer
    startTimerHz(30);
}

//==============================================================================
DeckGUI::~DeckGUI()
{
    stopTimer();
}

//==============================================================================
void DeckGUI::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Background gradient
    juce::ColourGradient gradient(
        juce::Colour(20, 20, 30),
        0.0f,
        0.0f,
        juce::Colour(45, 45, 60),
        0.0f,
        (float)getHeight(),
        false);

    g.setGradientFill(gradient);
    g.fillAll();

    // Outer border
    g.setColour(juce::Colours::grey.withAlpha(0.5f));
    g.drawRoundedRectangle(bounds.reduced(4.0f), 12.0f, 2.0f);

    // Header
    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(20.0f, juce::Font::bold));

    juce::String deckName;

    if (inChannel == 0)
        deckName = "LEFT DECK";
    else if (inChannel == 1)
        deckName = "MID DECK";
    else
        deckName = "RIGHT DECK";

    g.drawText(deckName,
               15,
               10,
               getWidth() - 30,
               30,
               juce::Justification::centredLeft);

    // Divider line
    g.setColour(juce::Colours::darkgrey);
    g.drawLine(10.0f, 45.0f, (float)getWidth() - 10.0f, 45.0f, 1.0f);
}

//==============================================================================
//==============================================================================
void DeckGUI::resized()
{
    auto area = getLocalBounds().reduced(12);

    //==========================================================
    // Header

    area.removeFromTop(45);

    //==========================================================
    // Waveform

    auto waveformArea = area.removeFromTop(120);

    waveFormDisplay.setBounds(waveformArea);

    area.removeFromTop(8);

    //==========================================================
    // Time Label

    currentTimePositionLabel.setBounds(
        area.removeFromTop(28));

    area.removeFromTop(10);

    //==========================================================
    // FIRST SLIDER ROW

    auto sliderRow1 = area.removeFromTop(120);

    auto sliderWidth1 =
        sliderRow1.getWidth() / 3;

    volSlider.setBounds(
        sliderRow1.removeFromLeft(sliderWidth1).reduced(8));

    speedSlider.setBounds(
        sliderRow1.removeFromLeft(sliderWidth1).reduced(8));

    posSlider.setBounds(
        sliderRow1.reduced(8));

    //==========================================================
    // SECOND SLIDER ROW

    auto sliderRow2 = area.removeFromTop(120);

    auto sliderWidth2 =
        sliderRow2.getWidth() / 3;

    roomSlider.setBounds(
        sliderRow2.removeFromLeft(sliderWidth2).reduced(8));

    wetLevelSlider.setBounds(
        sliderRow2.removeFromLeft(sliderWidth2).reduced(8));

    dryLevelSlider.setBounds(
        sliderRow2.reduced(8));

    area.removeFromTop(10);

    //==========================================================
    // BUTTON ROW

    auto buttonRow = area.removeFromTop(45);

    auto buttonWidth =
        buttonRow.getWidth() / 4;

    playButton.setBounds(
        buttonRow.removeFromLeft(buttonWidth).reduced(4));

    stopButton.setBounds(
        buttonRow.removeFromLeft(buttonWidth).reduced(4));

    loadButton.setBounds(
        buttonRow.removeFromLeft(buttonWidth).reduced(4));

    trackButton.setBounds(
        buttonRow.reduced(4));

    area.removeFromTop(8);

    //==========================================================
    // SEARCH BOX

    playlistEditor.setBounds(
        area.removeFromTop(32));
}

//==============================================================================
void DeckGUI::buttonClicked(juce::Button* button)
{
    //==============================================================================
    // PLAY
    if (button == &playButton)
    {
        player->start();
        return;
    }

    //==============================================================================
    // STOP
    if (button == &stopButton)
    {
        player->stop();
        return;
    }

    //==============================================================================
    // LOAD
    if (button == &loadButton)
    {
        auto fileChooserFlags =
            juce::FileBrowserComponent::canSelectFiles;

        fChooser.launchAsync(
            fileChooserFlags,
            [this](const juce::FileChooser& chooser)
            {
                auto chosenFile = chooser.getResult();

                if (chosenFile.existsAsFile())
                {
                    juce::URL fileURL{ chosenFile };

                    player->loadURL(fileURL);
                    waveFormDisplay.loadURL(fileURL);
                }
            });

        return;
    }

    //==============================================================================
    // QUEUE / LOAD TRACK FROM PLAYLIST
    if (button == &trackButton)
    {
        std::vector<std::string>* selectedStore = nullptr;

        if (inChannel == 0)
            selectedStore = &playlistComponent->leftStore;
        else if (inChannel == 1)
            selectedStore = &playlistComponent->midStore;
        else
            selectedStore = &playlistComponent->rightStore;

        if (selectedStore != nullptr && !selectedStore->empty())
        {
            juce::File audioFile{ selectedStore->front() };
            juce::URL trackURL{ audioFile };

            player->loadURL(trackURL);
            waveFormDisplay.loadURL(trackURL);

            titledTracks.push_back(TrackList{ audioFile });

            selectedStore->erase(selectedStore->begin());
        }

        return;
    }
}

//==============================================================================
void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volSlider)
    {
        player->increaseDecreaseSoundVolume(slider->getValue());
    }
    else if (slider == &speedSlider)
    {
        player->fastSlowSound(slider->getValue());
    }
    else if (slider == &posSlider)
    {
        player->forwardBackwardSoundRelative(slider->getValue());
    }
    else if (slider == &roomSlider)
    {
        player->roomSong(slider->getValue());
    }
    else if (slider == &wetLevelSlider)
    {
        player->wetSong(slider->getValue());
    }
    else if (slider == &dryLevelSlider)
    {
        player->drySong(slider->getValue());
    }
}

//==============================================================================
bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
    return files.size() > 0;
}

//==============================================================================
void DeckGUI::filesDropped(const juce::StringArray& files,
                           int /*x*/,
                           int /*y*/)
{
    for (const auto& filename : files)
    {
        juce::File file{ filename };

        if (file.existsAsFile())
        {
            juce::URL fileURL{ file };

            player->loadURL(fileURL);
            waveFormDisplay.loadURL(fileURL);
        }
    }
}

//==============================================================================
void DeckGUI::timerCallback()
{
    auto relativePosition = player->getPositionRelativeCallback();

    waveFormDisplay.setPositionRelative(relativePosition);

    // Prevent invalid values
    if (relativePosition <= 0.0)
    {
        currentTimePositionLabel.setText("00:00:000",
                                         juce::dontSendNotification);
        return;
    }

    juce::RelativeTime relativeTime(relativePosition);

    auto minutes =
        ((int)relativeTime.inMinutes()) % 60;

    auto seconds =
        ((int)relativeTime.inSeconds()) % 60;

    auto milliseconds =
        ((int)relativeTime.inMilliseconds()) % 1000;

    auto timeText =
        juce::String::formatted("%02d:%02d:%03d",
                                minutes,
                                seconds,
                                milliseconds);

    currentTimePositionLabel.setText(timeText,
                                     juce::dontSendNotification);
}

//==============================================================================
int DeckGUI::getNumRows()
{
    return static_cast<int>(titledTracks.size());
}

//==============================================================================
void DeckGUI::paintRowBackground(juce::Graphics&,
                                 int,
                                 int,
                                 int,
                                 bool)
{
}

//==============================================================================
void DeckGUI::paintCell(juce::Graphics&,
                        int,
                        int,
                        int,
                        int,
                        bool)
{
}

//==============================================================================
std::string DeckGUI::getFileFullPath(const std::string& fromFilepath)
{
    juce::File audioFile(fromFilepath);

    return audioFile
        .getFileNameWithoutExtension()
        .toStdString();
}

//==============================================================================
void DeckGUI::findTrackInPlaylist(juce::String text)
{
    if (text.isNotEmpty())
    {
        int rowNumber = getTrackInPlaylist(text);

        if (rowNumber >= 0)
        {
            playlistComponent->tableComponent.selectRow(rowNumber);
        }
    }
    else
    {
        playlistComponent->tableComponent.deselectAllRows();
    }
}

//==============================================================================
int DeckGUI::getTrackInPlaylist(juce::String text)
{
    auto findIndex =
        std::find_if(
            titledTracks.begin(),
            titledTracks.end(),
            [&text](const TrackList& list)
            {
                return list.trackTitle.containsIgnoreCase(text);
            });

    if (findIndex != titledTracks.end())
    {
        return static_cast<int>(
            std::distance(titledTracks.begin(), findIndex));
    }

    juce::AlertWindow::showMessageBoxAsync(
        juce::AlertWindow::InfoIcon,
        "Track Search",
        "No matching track found.",
        "OK");

    return -1;
}
