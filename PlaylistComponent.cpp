/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 23 Feb 2024 11:01:05pm
    Author:  Bright Osahenhen Iyahen

  ==============================================================================
*/

#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(
    juce::AudioFormatManager& _formatManager,
    DJAudioPlayer* _player1,
    DJAudioPlayer* _player2,
    DJAudioPlayer* _player3)
    : formatManager(_formatManager),
      player1(_player1),
      player2(_player2),
      player3(_player3)
{
    setupTable();

    addAndMakeVisible(tableComponent);
}

PlaylistComponent::~PlaylistComponent()
{
    tableComponent.setModel(nullptr);
}

//==============================================================================
// Component

void PlaylistComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(20, 20, 20));

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);

    g.setColour(juce::Colours::white);
    g.setFont(18.0f);

    g.drawText(
        "PLAYLIST",
        10,
        5,
        getWidth(),
        30,
        juce::Justification::centredLeft
    );
}

void PlaylistComponent::resized()
{
    tableComponent.setBounds(
        10,
        40,
        getWidth() - 20,
        getHeight() - 50
    );
}

//==============================================================================
// Table Setup

void PlaylistComponent::setupTable()
{
    tableComponent.getHeader().addColumn("Track", 1, 300);
    tableComponent.getHeader().addColumn("Length", 2, 120);
    tableComponent.getHeader().addColumn("Left Deck", 3, 120);
    tableComponent.getHeader().addColumn("Middle Deck", 4, 120);
    tableComponent.getHeader().addColumn("Right Deck", 5, 120);
    tableComponent.getHeader().addColumn("Remove", 6, 80);

    tableComponent.setModel(this);

    tableComponent.setColour(
        juce::ListBox::backgroundColourId,
        juce::Colours::black
    );

    tableComponent.getHeader().setColour(
        juce::TableHeaderComponent::backgroundColourId,
        juce::Colours::darkslategrey
    );

    tableComponent.getHeader().setColour(
        juce::TableHeaderComponent::textColourId,
        juce::Colours::white
    );
}

//==============================================================================
// TableListBoxModel

int PlaylistComponent::getNumRows()
{
    return static_cast<int>(tracks.size());
}

void PlaylistComponent::paintRowBackground(
    juce::Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::darkcyan);
    }
    else if (rowNumber % 2 == 0)
    {
        g.fillAll(juce::Colour(30, 30, 30));
    }
    else
    {
        g.fillAll(juce::Colour(45, 45, 45));
    }
}

void PlaylistComponent::paintCell(
    juce::Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected)
{
    if (!isValidRow(rowNumber))
        return;

    g.setColour(juce::Colours::white);
    g.setFont(14.0f);

    const auto& track = tracks[rowNumber];

    //==============================================================================
    // Track Title

    if (columnId == 1)
    {
        g.drawText(
            track.title,
            8,
            0,
            width - 10,
            height,
            juce::Justification::centredLeft,
            true
        );
    }

    //==============================================================================
    // Track Duration

    else if (columnId == 2)
    {
        g.drawText(
            formatTime(track.duration),
            0,
            0,
            width,
            height,
            juce::Justification::centred,
            true
        );
    }
}

//==============================================================================
juce::Component* PlaylistComponent::refreshComponentForCell(
    int rowNumber,
    int columnId,
    bool isRowSelected,
    juce::Component* existingComponentToUpdate)
{
    juce::ignoreUnused(isRowSelected);

    //==============================================================================
    // LEFT DECK

    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            auto* button = new juce::TextButton("Load");

            button->setComponentID(
                "L_" + juce::String(rowNumber));

            button->addListener(this);

            button->setColour(
                juce::TextButton::buttonColourId,
                juce::Colours::orange);

            existingComponentToUpdate = button;
        }
    }

    //==============================================================================
    // MIDDLE DECK

    else if (columnId == 4)
    {
        if (existingComponentToUpdate == nullptr)
        {
            auto* button = new juce::TextButton("Load");

            button->setComponentID(
                "M_" + juce::String(rowNumber));

            button->addListener(this);

            button->setColour(
                juce::TextButton::buttonColourId,
                juce::Colours::deepskyblue);

            existingComponentToUpdate = button;
        }
    }

    //==============================================================================
    // RIGHT DECK

    else if (columnId == 5)
    {
        if (existingComponentToUpdate == nullptr)
        {
            auto* button = new juce::TextButton("Load");

            button->setComponentID(
                "R_" + juce::String(rowNumber));

            button->addListener(this);

            button->setColour(
                juce::TextButton::buttonColourId,
                juce::Colours::mediumseagreen);

            existingComponentToUpdate = button;
        }
    }

    //==============================================================================
    // DELETE BUTTON

    else if (columnId == 6)
    {
        if (existingComponentToUpdate == nullptr)
        {
            auto* button = new juce::TextButton("X");

            button->setComponentID(
                "D_" + juce::String(rowNumber));

            button->addListener(this);

            button->setColour(
                juce::TextButton::buttonColourId,
                juce::Colours::red);

            existingComponentToUpdate = button;
        }
    }

    return existingComponentToUpdate;
}

//==============================================================================
// Button Listener

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    auto id = button->getComponentID();

    if (id.isEmpty())
        return;

    auto parts =
        juce::StringArray::fromTokens(id, "_", "");

    if (parts.size() != 2)
        return;

    auto command = parts[0];

    int row = parts[1].getIntValue();

    if (!isValidRow(row))
        return;

    auto filePath = tracks[row].filePath;

    //==============================================================================
    // LEFT DECK

    if (command == "L")
    {
        loadTrackToDeck(filePath, 0);
    }

    //==============================================================================
    // MIDDLE DECK

    else if (command == "M")
    {
        loadTrackToDeck(filePath, 1);
    }

    //==============================================================================
    // RIGHT DECK

    else if (command == "R")
    {
        loadTrackToDeck(filePath, 2);
    }

    //==============================================================================
    // DELETE

    else if (command == "D")
    {
        removeTrack(row);
    }
}

//==============================================================================
// AudioSource

void PlaylistComponent::prepareToPlay(
    int samplesPerBlockExpected,
    double sampleRate)
{
    juce::ignoreUnused(samplesPerBlockExpected);
    juce::ignoreUnused(sampleRate);
}

void PlaylistComponent::getNextAudioBlock(
    const juce::AudioSourceChannelInfo& bufferToFill)
{
    juce::ignoreUnused(bufferToFill);
}

void PlaylistComponent::releaseResources()
{
    transportSource.releaseResources();
}

//==============================================================================
// Drag & Drop

bool PlaylistComponent::isInterestedInFileDrag(
    const juce::StringArray& files)
{
    juce::ignoreUnused(files);

    return true;
}

void PlaylistComponent::filesDropped(
    const juce::StringArray& files,
    int x,
    int y)
{
    juce::ignoreUnused(x);
    juce::ignoreUnused(y);

    for (const auto& file : files)
    {
        addTrack(juce::File(file));
    }

    tableComponent.updateContent();
    repaint();
}

//==============================================================================
// Playlist Logic

void PlaylistComponent::loadTrackToDeck(
    const std::string& filePath,
    int deckIndex)
{
    if (deckIndex == 0)
    {
        leftStore.push_back(filePath);
    }
    else if (deckIndex == 1)
    {
        midStore.push_back(filePath);
    }
    else if (deckIndex == 2)
    {
        rightStore.push_back(filePath);
    }
}

void PlaylistComponent::addTrack(const juce::File& file)
{
    if (!file.existsAsFile())
        return;

    TrackInfo track;

    track.filePath =
        file.getFullPathName().toStdString();

    track.title =
        file.getFileNameWithoutExtension().toStdString();

    if (auto* reader =
        formatManager.createReaderFor(file))
    {
        track.duration =
            reader->lengthInSamples
            / reader->sampleRate;

        delete reader;
    }

    tracks.push_back(track);
}

void PlaylistComponent::removeTrack(int index)
{
    if (!isValidRow(index))
        return;

    tracks.erase(tracks.begin() + index);

    tableComponent.updateContent();
    repaint();
}

bool PlaylistComponent::isValidRow(int row) const
{
    return row >= 0
        && row < static_cast<int>(tracks.size());
}

//==============================================================================
// Audio File Helpers

void PlaylistComponent::loadAudioFile(
    const juce::URL& audioURL)
{
    juce::ignoreUnused(audioURL);
}

double PlaylistComponent::extractFileLengthInSeconds() const
{
    return transportSource.getLengthInSeconds();
}

//==============================================================================
// File Helpers

std::string PlaylistComponent::getFilePath(
    const juce::String& fromFilename)
{
    return fromFilename.toStdString();
}

std::string PlaylistComponent::getFileNameWithoutExtension(
    const juce::String& fromFilename)
{
    return juce::File(fromFilename)
        .getFileNameWithoutExtension()
        .toStdString();
}

//==============================================================================
// Time Formatting

juce::String PlaylistComponent::formatTime(
    double seconds) const
{
    int mins = static_cast<int>(seconds) / 60;

    int secs = static_cast<int>(seconds) % 60;

    return juce::String::formatted(
        "%02d:%02d",
        mins,
        secs
    );
}
