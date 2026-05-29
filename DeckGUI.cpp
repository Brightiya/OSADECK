/*
  ==============================================================================

    DeckGUI.cpp
    Created: 23 Feb 2024 11:03:24pm
    Author:  Bright  Osahenhen Iyahen

  ==============================================================================
*/

//#include <JuceHeader.h>
#include "DeckGUI.h"



//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse, PlaylistComponent* _playlistComponent,  int channelInUse) : player{_player}, waveFormDisplay(formatManagerToUse, cacheToUse),  playlistComponent(_playlistComponent), inChannel(channelInUse)
{
    
  
    
    /**Makes all the buttons and sliders appear on the GUI**/
    addAndMakeVisible(&playButton);
    addAndMakeVisible(&stopButton);
    addAndMakeVisible (&currentTimePositionLabel);
    addAndMakeVisible(&loadButton);
    addAndMakeVisible(&volSlider);
    addAndMakeVisible(&volumeLabel);
    addAndMakeVisible(&speedSlider);
    addAndMakeVisible(&speedLabel);
    addAndMakeVisible(&posSlider);
    addAndMakeVisible(&forwardBackwardLabel);
    addAndMakeVisible(&roomSlider);
    addAndMakeVisible(&roomLabel);
    addAndMakeVisible(&wetLevelSlider);
    addAndMakeVisible(&wetLevelLabel);
    addAndMakeVisible(&dryLevelSlider);
    addAndMakeVisible(&dryLevelLabel);
    addAndMakeVisible(&waveFormDisplay);
    addAndMakeVisible(&trackButton);
    addAndMakeVisible(playlistEditor);
    
    /**Set the texts against the various sliders**/
    volumeLabel.setText("Volume", juce::dontSendNotification);
    currentTimePositionLabel.setText ("Time counts here", juce::dontSendNotification);
    speedLabel.setText("Speed", juce::dontSendNotification);
    forwardBackwardLabel.setText("Forward", juce::dontSendNotification);
    roomLabel.setText("Room", juce::dontSendNotification);
    wetLevelLabel.setText("Wet", juce::dontSendNotification);
    dryLevelLabel.setText("Dry", juce::dontSendNotification);
    
    
    /**Sets the text on the playlist editor**/
    playlistEditor.setTextToShowWhenEmpty("Search Tracks after load (press enter to submit)",juce::Colours::yellow);
    
    
    
    /**The playlistEditor function uses  lambda function to search for a track on the playlist and return the track title.**/
    playlistEditor.onReturnKey = [this] { findTrackInPlaylist(playlistEditor.getText());
    };
   
    
    /**Paints a rotary shape of the sliders**/
    volSlider.setSliderStyle(juce::Slider::Rotary);
    speedSlider.setSliderStyle(juce::Slider::Rotary);
    posSlider.setSliderStyle(juce::Slider::Rotary);
    roomSlider.setSliderStyle(juce::Slider::Rotary);
    wetLevelSlider.setSliderStyle(juce::Slider::Rotary);
    dryLevelSlider.setSliderStyle(juce::Slider::Rotary);
   
    
    /** attachToComponent function attaches each label to its respective slider**/
    volumeLabel.attachToComponent(&volSlider, true);
    speedLabel.attachToComponent(&speedSlider, true);
    forwardBackwardLabel.attachToComponent(&posSlider, true);
    roomLabel.attachToComponent(&roomSlider, true);
    wetLevelLabel.attachToComponent(&wetLevelSlider, true);
    dryLevelLabel.attachToComponent(&dryLevelSlider, true);
    
    
    
   /**Event listeners are applied on each buttons and sliders**/
    trackButton.addListener(this);
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    roomSlider.addListener(this);
    wetLevelSlider.addListener(this);
    dryLevelSlider.addListener(this);
    playlistEditor.addListener(this);
    
    /**Various sliders ranges are set respectively**/
    volSlider.setRange(0.0, 1.0);
    posSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 5.0);
    roomSlider.setRange(0.0, 1.0);
    dryLevelSlider.setRange(0.0, 1.0);
    wetLevelSlider.setRange(0.0, 1.0);
    
   
    /**Starts the Timer function**/
    startTimer(500);
    
    
}

DeckGUI::~DeckGUI()
{
    /**Stops the Timer when the app exits**/
    stopTimer();
   
}

void DeckGUI::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

      
    */

    g.fillAll(juce::Colours::darkorchid);
    

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
  

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("OSADECK", getLocalBounds(),
                juce::Justification::centredLeft, true);   // draw some placeholder text
   
}

void DeckGUI::resized()
{
   /**setBounds function sets the various buttons and sliders appropriately on the GUI interface**/
    double rowH = getHeight()/11;
    waveFormDisplay.setBounds(0, 0, getWidth(), rowH*2);
    volSlider.setBounds(50, rowH * 2, getWidth()/2, rowH*2);
    speedSlider.setBounds(50, rowH * 4, getWidth()/2, rowH *2);
    posSlider.setBounds(50, rowH * 6, getWidth()/2, rowH*2);
    roomSlider.setBounds(rowH*7.2,rowH*2,getWidth()/2,rowH*2);
    wetLevelSlider.setBounds(rowH*7.2,rowH*4,getWidth()/2,rowH*2);
    dryLevelSlider.setBounds(rowH*7.2,rowH*6,getWidth()/2,rowH*2);
    playButton.setBounds(120, rowH*8, getWidth()/4, rowH);
    stopButton.setBounds(rowH*7,rowH*8,getWidth()/4,rowH);
    loadButton.setBounds(120, rowH * 9.5, getWidth()/4, rowH);
    trackButton.setBounds(rowH * 7, rowH * 9.5, getWidth()/4, rowH);
    currentTimePositionLabel.setBounds (rowH*7,rowH*2,getWidth(),rowH);
    playlistEditor.setBounds (10, 370, getWidth(), 25);
    
   
    
    

}


void DeckGUI::buttonClicked(juce::Button * button)
{
  /**Checks that  button is the address of the  playButton  and it  is pressed and if so sets the colour green and calls the DJAudio player start function and if not, sets the colour grey**/
    if (button == &playButton) {
       
        playButton.setColour (juce::TextButton::buttonColourId, juce::Colours::green);
        player->start();
    }
    else{
        playButton.setColour (juce::TextButton::buttonColourId, juce::Colours::grey);
    }
 
    /**Checks that the  button is the address of the loadButton  and it is  pressed and if so FileBrowserComponent function uses canSelectFiles to make the track file selectable and a fChooser is called asynchronously to get the actual track file. Then DJAudioPlayer loadURL function is called to load the file as well as the waveFormDisplay loadURL function is also called to display the wavefrom of the track file. The loadButton is finally set to green otherwise  its set to grey  **/

    if (button == &loadButton) {
        auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;
            fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser & chooser){
                
                auto chosenFile = chooser.getResult();
                loadButton.setColour (juce::TextButton::buttonColourId, juce::Colours::green);
                
                player->loadURL(juce::URL{chosenFile});
                waveFormDisplay.loadURL(juce::URL{chosenFile});
              

                
                
            });
        
        
        
    }
    
    else{
        loadButton.setColour (juce::TextButton::buttonColourId, juce::Colours::grey);
        
    }
     
    
    /**Checks that  button is the address of the  stopButton and it is pressed and if so sets the colour red and calls the DJAudio player start function and if not, sets the colour grey**/
  
    if (button == &stopButton) {
       
        
     
     
        player->stop();
        stopButton.setColour (juce::TextButton::buttonColourId, juce::Colours::red);
        
    }else{
        stopButton.setColour (juce::TextButton::buttonColourId, juce::Colours::grey);
    }
   
    
 
  
    /**Checks that the  button is the address of the trackButton  and it is  pressed  **/
   
    if (button == &trackButton)
       {
         
           if (inChannel == 0 && playlistComponent->leftStore.size() > 0)
           {
               /**Gets  the track file in the leftStore vector of the playlistComponent**/
               juce::URL trackURL = juce::URL{ juce::File{playlistComponent->leftStore[0]}
               };
              
             /**DJAudioPlayer´s loadURL function loads the track file into the left Deck**/
               player->loadURL(trackURL);
               
              /**waveFormDisplay´s  loadURL function displays the track waveform on the left Deck**/
               waveFormDisplay.loadURL(trackURL);
               
               
               /**Gets  the track file in the leftStore vector of the playlistComponent into the TrackList file function in which the track title is extracted and stored in the titledTrack vector **/
               TrackList trackList{ juce::File{playlistComponent->leftStore[0]} };
                             juce::URL audioURL{ trackURL };
                          
                         titledTracks.push_back(trackList);
        
               
               playlistComponent->leftStore.erase(playlistComponent->leftStore.begin());
           }
           
           /**Same as above but only the channel and playlist store are changed**/
          
           if (inChannel == 1 && playlistComponent->midStore.size() > 0)
           {
              
               juce::URL trackURL = juce::URL{ juce::File{playlistComponent->midStore[0]} };
             
               player->loadURL(trackURL);
              
               waveFormDisplay.loadURL(trackURL);
              
               
               TrackList trackList{ juce::File{playlistComponent->midStore[0]} };
                             juce::URL audioURL{ trackURL };
                        
               titledTracks.push_back(trackList);
              
              
               playlistComponent->midStore.erase(playlistComponent->midStore.begin()); 
           }
           
           /**Same as above but only the channel and playlist store are changed**/
           if (inChannel == 2 && playlistComponent->rightStore.size() > 0)
           {
             
               juce::URL trackURL = juce::URL{ juce::File{playlistComponent->rightStore[0]} };
               
               player->loadURL(trackURL);
               
              
               waveFormDisplay.loadURL(trackURL);
              
               
               TrackList trackList{ juce::File{playlistComponent->rightStore[0]} };
                             juce::URL audioURL{ trackURL };
                          
                         titledTracks.push_back(trackList);
           
               
               playlistComponent->rightStore.erase(playlistComponent->rightStore.begin());
           }

           trackButton.setColour (juce::TextButton::buttonColourId, juce::Colours::green);
       }
    
    else{
           trackButton.setColour (juce::TextButton::buttonColourId, juce::Colours::grey);
       }
        
}


void DeckGUI::sliderValueChanged(juce::Slider *slider)
{
    /**Checks that slider is the address of the volume slider and if so the DJAudioPlayer's  increaseDecreaseSoundVolume function is called to get the slider's value**/
    
    if (slider == &volSlider) {
      
     
        player->increaseDecreaseSoundVolume(slider->getValue());
    }
    
    /**Checks that slider is the address of the speed slider and if so the DJAudioPlayer's  fastSlowSound function is called to get the slider's value**/
    if (slider == &speedSlider) {
        player->fastSlowSound(slider->getValue());
    }
     
    /**Checks that slider is the address of the position slider and if so the DJAudioPlayer's  forwardBackwardSoundRelative function is called to get the slider's value**/
    
    if (slider == &posSlider) {
        player->forwardBackwardSoundRelative(slider->getValue());
    }
    
    /**Checks that slider is the address of the room slider and if so the DJAudioPlayer's  roomSong function is called to get the slider's value**/
    if (slider == &roomSlider) {
        player->roomSong(slider->getValue());
    }
    
    /**Checks that slider is the address of the wetLevel slider and if so the DJAudioPlayer's  wetSong function is called to get the slider's value**/
    if (slider == &wetLevelSlider) {
        player->wetSong(slider->getValue());
    }
    /**Checks that slider is the address of the dryLevel slider and if so the DJAudioPlayer's  drySong function is called to get the slider's value**/
    
    if (slider == &dryLevelSlider) {
        player->drySong(slider->getValue());
    }
    
   
    
    
    
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray &files){
   
    
    return true;
}
 
void DeckGUI::filesDropped(const juce::StringArray &files, int x, int y)
{
    
    for (juce::String filename : files) {
       
        /**File track is loaded by the DJAudioPlayer when dropped and would be wave-displayed when played**/
        juce::URL fileURL = juce::URL{juce::File{filename}};
        
        player->loadURL(fileURL);
        waveFormDisplay.loadURL(fileURL);
    }
    
    
    
   
    
    
}

void DeckGUI::timerCallback()
{
    
    
    
   
    /** getPositionRelativeCallback() is called repeatedly by the Timer's callback function*/
    
   
    waveFormDisplay.setPositionRelative(player->getPositionRelativeCallback());
    
   
   
    
   
    /**Checks that the getPositionRelativeCallback() function is called **/
    if (player->getPositionRelativeCallback())
    {
        /**Gets the relative position of the playhead**/
        juce::RelativeTime relativePos (player->getPositionRelativeCallback());
        
        /**Calculate time in minutes, seconds and milliseconds**/
        auto inMin = ((int) relativePos.inMinutes()) % 60; // 1 min.
        auto inSec = ((int) relativePos.inSeconds()) % 60; //1 sec.
        auto inMilis  = ((int) relativePos.inMilliseconds()) % 1000;
        /**Gets the time position of the moving playhead**/
        auto timePosition = juce::String::formatted ("%02d:%02d:%03d", inMin, inSec, inMilis );
        /** Sets the text to the left of the time*/
        currentTimePositionLabel.setText (timePosition, juce::dontSendNotification);
    }
    else
    {
      /**Sets the text of the time if the tme is yet to start to count**/
        currentTimePositionLabel.setText (" Time counts here", juce::dontSendNotification);
    }

   
}


int DeckGUI::getNumRows()
{
  
     
    titledTracks.size();
    return 0;
}


void DeckGUI::paintRowBackground(juce::Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected){}






std::string DeckGUI::getFileFullPath(const std::string &fromFilepath) {
    std::string fullP = fromFilepath.substr(playlistComponent->getSlashInFile(fromFilepath) + 1, fromFilepath.length() - playlistComponent->getSlashInFile(fromFilepath) - playlistComponent->getDotExtension(fromFilepath).size() - 2);
    
    return fullP;
}

void DeckGUI::paintCell(juce::Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected){}



void DeckGUI::findTrackInPlaylist(juce::String text)
{
    
    
    if (text != "")
    {
        /**Selects the track when the track title text is found on the playlistComponent  else deselect**/
        int numRow = getTrackInPlaylist(text);
        playlistComponent->tableComponent.selectRow(numRow);
        
    }
    else
    {
       
        
        playlistComponent->tableComponent.deselectAllRows();
    }
}

int DeckGUI::getTrackInPlaylist(juce::String text)
{
   /**Finds the track title index when the search text matches it**/
    auto findIndex = find_if(titledTracks.begin(), titledTracks.end(),
        [&text](const TrackList& list) {return list.trackTitle.contains(text); });
    
    int rowIndex = -1;

    if (findIndex != titledTracks.end())
    {
        /**Gets the row where the track is found else display 'No such name found!**/
        rowIndex = static_cast<int>(std::distance(titledTracks.begin(), findIndex)) ;
    }
    else{
        juce::AlertWindow::showMessageBoxAsync (juce::AlertWindow::AlertIconType::InfoIcon,
                   "Search loaded tracks on this Deck:",
                   "No such name found! Please try again",
                   "OK",
                   nullptr
               );
    }
    
    return rowIndex;
    
}
