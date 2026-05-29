#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (1290, 870);
   
   

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    
    
   
   
    /**Make visible the components**/
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(deckGUI3);
    addAndMakeVisible(playlistComponent);
    addAndMakeVisible(labelPlaylist);
    addAndMakeVisible(labelGUI);
    
    
   /**Attaches text on the side of the playlistComponent**/
    labelPlaylist.setFont (18.0f);
    labelPlaylist.setText("Drag and drop files here>>>>>>>>>>", juce::dontSendNotification);
    
    labelPlaylist.setColour(juce::Label::textColourId, juce::Colours::white);
    labelPlaylist.setJustificationType(juce::Justification::centred);
    
    /**Attaches text on the Deck GUI**/
    labelGUI.setFont (18.0f);
    labelGUI.setText("Drag and drop files...", juce::dontSendNotification);
    
    labelGUI.setColour(juce::Label::textColourId, juce::Colours::white);
    labelGUI.setJustificationType(juce::Justification::centred);
    
 
    
    
    formatManager.registerBasicFormats();
    
    
    
    
}

MainComponent::~MainComponent()

{
    deviceManager.removeAllChangeListeners();
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
   
   
  /**Prepares each Deck to play sound tracks**/
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
    mixerSource.addInputSource(&player3, false);
  
}




void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    /**Get the next audio sample and distribute them to each Deck**/
  mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
     

  /**This will be called when the audio device stops, or when it is being
   restarted due to a setting change*/
    player1.releaseResources();
    player2.releaseResources();
    player3.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    g.setFont (20.0f);
    //g.setColour({255, 0,0});
    
    
   
   
                      
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    
    
    double rowH = getHeight() / 12;
    double colW = getWidth()/7;
    
    labelGUI.setBounds(0,rowH*2.8, colW, rowH*3);
    deckGUI1.setBounds(0, 0, getWidth()/3, getHeight()/2);
    deckGUI2.setBounds(getWidth()/3, 0, getWidth()/3, getHeight()/2);
    deckGUI3.setBounds((getWidth()/3)*2, 0, getWidth()/3, getHeight()/2);
    labelPlaylist.setBounds(0,rowH*6, colW, rowH*3);
    playlistComponent.setBounds(getWidth()/6, getHeight()/2, getWidth(), getHeight()/2);
    
    
   
}
