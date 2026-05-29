/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 23 Feb 2024 11:01:05pm
    Author:  Bright  Osahenhen Iyahen

  ==============================================================================
*/

//#include <JuceHeader.h>
#include "PlaylistComponent.h"



//==============================================================================
PlaylistComponent::PlaylistComponent(juce::AudioFormatManager& _formatManager, DJAudioPlayer* _player1, DJAudioPlayer* _player2, DJAudioPlayer* _player3): formatManager(_formatManager), player1{_player1}, player2{_player2}, player3{_player3}
{
    
    /**Makes the PlaylistComponent visible**/
    addAndMakeVisible(tableComponent);
    
    /**Label the PlaylistComponent accordingly**/
    tableComponent.getHeader().addColumn("Track Title",1, 250);
    tableComponent.getHeader().addColumn("Length", 2, 100);
    tableComponent.getHeader().addColumn("", 3, 100);
    tableComponent.getHeader().addColumn("", 4, 100);
    tableComponent.getHeader().addColumn("", 5, 100);
    tableComponent.getHeader().addColumn("", 6, 100);
    
    /**Creats a table components for the PlaylistComponent**/
    tableComponent.setModel(this);
    
 
}

PlaylistComponent::~PlaylistComponent()
{
    /**Destroys the table components  when the app exits**/
    tableComponent.setModel (nullptr);
    
}

void PlaylistComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate){}

void PlaylistComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill){}

void PlaylistComponent::releaseResources()
{ 
    transportSource.releaseResources();
}


void PlaylistComponent::paint (juce::Graphics& g)
{
   

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    
   
    
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{

    
    /**Positios and resizes the PlaylistComponent**/
    double rowH = getHeight() / 8;
   tableComponent.setBounds(0, rowH, getWidth(), rowH*7);
  
}

int PlaylistComponent::getNumRows()
{
   /**Return the full length of the sound file**/
    return static_cast<int>( fullFilesStore.size()) ;
    
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    /**Paints the background of the playlistComponent when a playlsit row ls selected, alternets with coral colour else  colours white**/
    
    if (rowIsSelected) {
        g.fillAll(juce::Colours::aqua);
    }
    else if (rowNumber % 2)
         g.fillAll (juce::Colours::coral);

    
    
    else{
        g.fillAll(juce::Colours::white);
    }
     
}

void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
   
    
   
    
    /**Draw the track full fill name to first column**/
        if (columnId == 1)
        {
            g.drawText(fullFilesStore[rowNumber],
                       1, rowNumber,
                       width - 4, height,
                       juce::Justification::centredLeft,
                       true);
         
        }
        /**Draw the duration of the track to the second column**/
        if (columnId == 2)
        {
            g.drawText(std::to_string(fileLengthStore[rowNumber]) + "s",
                       1, rowNumber,
                       width - 4, height,
                       juce::Justification::centredLeft,
                       true);
            
           
                       
        }
        
   

}

 juce::Component * PlaylistComponent:: refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component *existingComponentToUpdate)
    {
    
    /**Creates a text button programmatically  in the third column when a sound file is dropped on the PlaylistComponent**/
        if (columnId == 3)
        {
            if (existingComponentToUpdate == nullptr)
            {
               
                juce::TextButton* textButton = new juce::TextButton{ "Add to LDeck" };
                
                juce::String id{ std::to_string(rowNumber) };
                
                textButton->setComponentID(id);
                textButton->addListener(this);
                
                existingComponentToUpdate = textButton;
                textButton->setColour(juce::TextButton::buttonColourId, juce::Colours::coral);
                
                
             
               
                
            }
           
         
            
            
        }
        
        
        
    /**Creates a text button programmatically  in the fourth column when a sound file is dropped on the PlaylistComponent**/
        if (columnId == 4)
        {
            if (existingComponentToUpdate == nullptr)
            {
                
                juce::TextButton* textButton = new juce::TextButton{ "Add to  MDeck" };
               
                juce::String id{ std::to_string(rowNumber) };
                
                
                textButton->setComponentID(id);
                textButton->addListener(this);
              
                existingComponentToUpdate = textButton;
                textButton->setColour(juce::TextButton::buttonColourId, juce::Colours::blue);
                
                
              }
        }
        
    
    /**Creates a text button programmatically  in the fifth column when a sound file is dropped on the PlaylistComponent**/
        
        if (columnId == 5)
        {
            if (existingComponentToUpdate == nullptr)
            {
                
                juce::TextButton* textButton = new juce::TextButton{ "Add to Rdeck" };
              
                juce::String id{ std::to_string(rowNumber) };
                
                textButton->setComponentID(id);
                textButton->addListener(this);
                existingComponentToUpdate = textButton;
                textButton->setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
              
                
                
                jassert (existingComponentToUpdate != nullptr);
             
                
            }
            
           
            
            
        }
    
    /**Creates a delete  text button programmatically  in the sixth column when a sound file is dropped on the PlaylistComponent**/
    
    if (columnId==6)
        {
            if (existingComponentToUpdate == nullptr)
            {
                juce::TextButton* textButton = new juce::TextButton{ "X" };
                juce::String id{ std::to_string(rowNumber + 200) };
                textButton->setComponentID(id);
               
                textButton->addListener(this);
                existingComponentToUpdate = textButton;
                textButton->setColour(juce::TextButton::buttonColourId, juce::Colours::red);
               
                
                
            }
            
            
        }
        
        
   
        
        return existingComponentToUpdate;
    
    
    
    }
        
    
    

    
   /** Add track file to each of the respective Left, Middle and Right  Decks**/
void PlaylistComponent::loadTrackToDeck(std::string useFilePath, int useDeck)
    {
        
       
            
            if (useDeck == 0  )
            {
                leftStore.push_back(useFilePath);
           
               
              
            }
            if (useDeck == 1 )
            {
                midStore.push_back(useFilePath);
                
           
                
            }
            if (useDeck == 2 )
            {
                rightStore.push_back(useFilePath);
                
           
                
            }
                 
                 
   
        }
    


void PlaylistComponent::buttonClicked(juce::Button* button)
{
    
    /**Creates a button id**/
    int btnId = std::stoi(button->getComponentID().toStdString());
    
    
    /**Loads a track to the  Left Deck**/
    if ((refreshComponentForCell(0, 3, false, button)))
    {
        
        
        loadTrackToDeck(filesStore[btnId], 0);
        
    }
    
    /**Loads a track to the  Middle Deck**/
    
    if((refreshComponentForCell(0, 4, false, button)))
    {
        loadTrackToDeck(filesStore[btnId], 1);
        
    }
    
    
    
    
    /**Loads a track to the  Right Deck**/
    if((refreshComponentForCell(0, 5, false, button)))
    {
        loadTrackToDeck(filesStore[btnId], 2);
        
    }
    
    
    
    /**Deletes track from the PlaylistComponent**/
    if(btnId>=200)
    
    {
        fullFilesStore.erase(fullFilesStore.begin());
        tableComponent.updateContent();
    }
    
    
    
}

bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray& files)
{
    return true; // allows files to be dragged and dropped
}



std::string PlaylistComponent::getFilePath(const juce::String &fromFilename) 
{
    /**Returns the file path of the track file**/
    return  juce::String(fromFilename).toStdString();
}

std::size_t PlaylistComponent::getSlashInFile(const juce::String &fromFilename) 
{
    /**Returns  slash  from  the track file**/
    return getFilePath(fromFilename).find_last_of("\\");
}

std::size_t PlaylistComponent::getDotInFile(const juce::String &fromFilename) 
{
    /**Returns dot from  the track file**/
    return getFilePath(fromFilename).find_last_of(".");
}

std::string PlaylistComponent::getDotExtension(const juce::String &fromFilename) 
{
    /**Returns any other  dot  extension from the track file**/
    return getFilePath(fromFilename).substr(getDotInFile(fromFilename) + 1, getFilePath(fromFilename).length() - getDotInFile(fromFilename));
}

std::string PlaylistComponent::getFileFullPath(const juce::String &fromFilename) 
{
    /**Returns the file ful path of the track file**/
    return getFilePath(fromFilename).substr(getSlashInFile(fromFilename) + 1, getFilePath(fromFilename).length() - getSlashInFile(fromFilename) - getDotExtension(fromFilename).size() - 2);
}

void PlaylistComponent::filesDropped(const juce::StringArray& files, int x, int y)
{
    //perform if files have been dropped (mouse released with files)
    for (juce::String fromFilename : files)
    {
       
        /** Store track files into vectors*/
        filesStore.push_back(getFilePath(fromFilename));
        fullFilesStore.push_back(getFileFullPath(fromFilename));
       
        /**Loads file track into the  Decks when the loadTrack button is pressed**/
        loadAudioFile(juce::URL{juce::File{getFilePath(fromFilename)} });
       
    }
   

    /**Updates the playlistComponent to include added track files**/
    tableComponent.updateContent();
   
   
    
}





double PlaylistComponent::extractFileLengthInSeconds() 
{
    /**Returns the file length in seconds**/
    return transportSource.getLengthInSeconds();
}


void PlaylistComponent::loadAudioFile(juce::URL fromAudioURL)
{
   
    
    /**Reads and stores the audio track file**/
    auto* audioReader = formatManager.createReaderFor(fromAudioURL.createInputStream(juce::URL::InputStreamOptions(juce::URL::ParameterHandling::inAddress).withConnectionTimeoutMs(1000).withNumRedirectsToFollow(0)));

    /** A successfully read the audio file then stores it  to the  fileLengthStore vector*/
    if (audioReader != nullptr) // good file!
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(audioReader,
            true));
        transportSource.setSource(newSource.get(), 0, nullptr, audioReader->sampleRate);
        readerSource.reset(newSource.release());
         // get length of audio
        fileLengthStore.push_back(extractFileLengthInSeconds()); // add audio length to vector
    }

   
    
    
   
}
