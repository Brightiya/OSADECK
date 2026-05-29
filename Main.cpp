/*
  ==============================================================================

    Main.cpp
    Professional JUCE Application Entry Point

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainComponent.h"

//==============================================================================
class OSADeckApplication : public juce::JUCEApplication
{
public:
    //==============================================================================
    OSADeckApplication() = default;

    //==============================================================================
    const juce::String getApplicationName() override
    {
        return "OSADECK";
    }

    const juce::String getApplicationVersion() override
    {
        return "2.0.0";
    }

    bool moreThanOneInstanceAllowed() override
    {
        return false;
    }

    //==============================================================================
    void initialise(const juce::String& commandLine) override
    {
        juce::ignoreUnused(commandLine);

        initialiseLookAndFeel();

        mainWindow = std::make_unique<MainWindow>(getApplicationName());
    }

    void shutdown() override
    {
        mainWindow.reset();
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted(const juce::String& commandLine) override
    {
        juce::ignoreUnused(commandLine);
    }

private:
    //==============================================================================
    void initialiseLookAndFeel()
    {
        auto& lookAndFeel = juce::LookAndFeel::getDefaultLookAndFeel();

        lookAndFeel.setColour(
            juce::ResizableWindow::backgroundColourId,
            juce::Colour(18, 18, 18));

        lookAndFeel.setColour(
            juce::TextButton::buttonColourId,
            juce::Colour(40, 40, 40));

        lookAndFeel.setColour(
            juce::TextButton::textColourOffId,
            juce::Colours::white);

        lookAndFeel.setColour(
            juce::Slider::thumbColourId,
            juce::Colours::deepskyblue);

        lookAndFeel.setColour(
            juce::Slider::rotarySliderFillColourId,
            juce::Colours::deepskyblue);

        lookAndFeel.setColour(
            juce::Label::textColourId,
            juce::Colours::white);
    }

    //==============================================================================
    class MainWindow : public juce::DocumentWindow
    {
    public:
        explicit MainWindow(juce::String name)
            : juce::DocumentWindow(
                  name,
                  juce::Colour(15, 15, 15),
                  juce::DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);

            setContentOwned(new MainComponent(), true);

           #if JUCE_IOS || JUCE_ANDROID
                setFullScreen(true);
           #else
                setResizable(true, true);

                setResizeLimits(1000, 700, 1920, 1080);

                centreWithSize(1400, 850);
           #endif

            setVisible(true);
        }

        //==============================================================================
        void closeButtonPressed() override
        {
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };

    //==============================================================================
    std::unique_ptr<MainWindow> mainWindow;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OSADeckApplication)
};

//==============================================================================
// Launch Application
START_JUCE_APPLICATION(OSADeckApplication)