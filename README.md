# OSADECK 🎧

OSADECK is a desktop DJ and audio playback application built with **C++** and the **JUCE Framework**.

The project simulates a simplified DJ mixing environment with three audio decks, waveform visualization, playlist interaction, playback manipulation, and real-time audio processing.

Developed as a multimedia software engineering project, OSADECK demonstrates practical concepts in audio programming, GUI application development, object-oriented design, and event-driven systems using modern C++.

---

# Features

* 🎵 Three deck audio playback
* 🌊 Real-time waveform visualization
* 📂 Playlist and track management
* ⏯ Play and stop controls
* 🎚 Volume control
* ⏩ Playback speed control
* 📍 Track position seeking
* 🎧 Real-time audio processing
* 🔊 Reverb effects support
* 🖥 Interactive JUCE-based graphical interface

---

# Built With

* **C++17**
* **JUCE Framework**
* **Xcode**
* **Projucer**
* Object-Oriented Programming (OOP)
* Real-Time Audio Processing

---

# Project Structure

```bash
OSADECK/
│--README.md
├──Source/
|  |── Main.cpp
|  |── MainComponent.cpp
|  |── MainComponent.h
|  |── DJAudioPlayer.cpp
|  |── DJAudioPlayer.h
|  |── DeckGUI.cpp
|  |── DeckGUI.h
|  |── WaveFormDisplay.cpp
|  |── WaveFormDisplay.h
|  |── PlaylistComponent.cpp
|  |── PlaylistComponent.h
|  |── TrackList.cpp
|  |── TrackList.h
├── Builds/
│   └── MacOSX/

```

---

# Core Components

| Component           | Description                                                                           |
| ------------------- | ------------------------------------------------------------------------------------- |
| `DJAudioPlayer`     | Handles audio loading, playback, resampling, gain control, seeking, and audio effects |
| `DeckGUI`           | Provides the graphical interface and deck controls                                    |
| `WaveFormDisplay`   | Displays waveform thumbnails and playback position                                    |
| `PlaylistComponent` | Manages playlist UI interactions                                                      |
| `TrackList`         | Stores and organizes track metadata                                                   |
| `MainComponent`     | Coordinates the main application layout and audio engine                              |

---

# How It Works

OSADECK uses JUCE audio components to:

* load audio files
* stream audio in real time
* visualize waveforms
* manipulate playback speed
* control gain and transport position
* process audio effects such as reverb

The application architecture separates:

* audio processing logic
* waveform rendering
* user interface controls
* playlist management

into modular C++ classes.

---

# Prerequisites

Before running the project, install:

* JUCE Framework
* Xcode (macOS)
* Git
* C++17 compatible compiler

---

# Installation

## Clone the Repository

```bash
git clone https://github.com/Brightiya/OSADECK.git
cd OSADECK
```

---

# JUCE Setup

This project uses the JUCE Projucer workflow rather than CMake.

## Install JUCE

Download JUCE from:

https://juce.com/get-juce/download

---

# Opening the Project

## Open the `.jucer` File

```bash
open OSADECK.jucer
```

OR open it directly using Projucer.

---

# Configure Modules

Ensure the following JUCE modules are enabled:

* juce_core
* juce_events
* juce_graphics
* juce_gui_basics
* juce_gui_extra
* juce_audio_basics
* juce_audio_formats
* juce_audio_utils
* juce_audio_devices

---

# Export to Xcode

Inside Projucer:

```text
File → Save Project and Open in IDE
```

This generates the Xcode project automatically.

---

# Running the Application

In Xcode:

```text
Press ▶ Run
```

The application will launch as a desktop audio player.

---

# Using VS Code

You may also edit the source code using VS Code.

## Open the Repository

```bash
code .
```

However, building and exporting are handled through:

* Projucer
* Xcode

rather than directly through VS Code.

---

# Screenshots

Add application screenshots here.

Suggested screenshots:

* Main interface
* Waveform visualization
* Playlist section
* Deck controls

Example:

```markdown
![Main Interface](screenshots/main-ui.png)
```

---

# Current Functionality

The current implementation supports:

* audio file loading
* waveform rendering
* playback control
* speed adjustment
* gain control
* playback position tracking
* reverb parameter manipulation

---

# Known Limitations

Current limitations include:

* no crossfader
* limited audio effects
* no BPM analysis
* no persistent playlist storage
* no drag-and-drop support
* no recording/export functionality

---

# Future Improvements

Planned future enhancements:

* BPM detection
* Crossfader implementation
* EQ and audio filters
* Drag-and-drop playlist support
* Recording/exporting mixes
* Improved UI responsiveness
* Keyboard shortcuts
* Streaming integration

---

# Learning Outcomes

This project demonstrates:

* audio software development
* JUCE framework integration
* event-driven GUI programming
* real-time multimedia processing
* modular software architecture
* object-oriented programming in C++

---

# Contributing

Contributions are welcome.

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push your branch
5. Open a Pull Request

---

# License

This project is open source and available under the MIT License.

---

# Author

## Bright O Iyahen

GitHub:
https://github.com/Brightiya
