

# 🌆 "End of Beginning" Audio-Reactive Visualizer

An embedded C++ lighting project built with **FastLED** and the **Wokwi Simulator**. This visualizer is hard-coded to perfectly synchronize with the track *"End of Beginning"* by Djo, using precise timecodes, custom drum sequencers, and procedural color palettes to map the flow of the song.

## ✨ Features

* **Timecode Synchronization:** Uses the Arduino's internal `millis()` clock to cue specific lighting scenes (Intro, Verses, Choruses, Finale) mapped perfectly to the song's structure.
* **Custom Drum Sequencer:** Replaces standard continuous beats with a custom modulo-based `(elapsedTime % 3000)` sequencer to trigger a specific "double-tap" drum fill mimicking the track.
* **Procedural Background Breathing:** Utilizes `beatsin8()` sine waves to create a slow, moody breathing background that gets aggressively interrupted by maximum-brightness drum hits.
* **Dynamic Palette Swapping:** Hot-swaps between three custom CRGB palettes depending on the timecode:
  * 🌊 **Midnight Blue:** Slow, breathing idle state for the verses.
  * ⚡ **Electric Cyan:** Sharp, high-brightness drum hits.
  * 🌅 **Golden Sunset:** Explosive, driving sawtooth waves for the massive chorus drops.

## 🛠️ Hardware Emulated (Wokwi)

* **Microcontroller:** Arduino Nano (ATmega328)
* **LEDs:** 16x WS2812B NeoPixel Ring
* **Data Pin:** D6

## 🚀 How to Run the Simulation Locally

This project is configured to run locally inside Visual Studio Code using the PlatformIO build system and the Wokwi Simulator extension.

**Prerequisites:**
* VS Code with the **PlatformIO IDE** extension installed.
* Python 3 installed (required for PlatformIO).
* The **Wokwi Simulator** extension for VS Code.

**Setup Steps:**
1. Clone this repository to your local machine.
2. Open the folder in VS Code. PlatformIO will automatically detect the `platformio.ini` file and download the required `FastLED` library.
3. Click the **Checkmark (✓)** icon in the bottom blue status bar to compile the C++ firmware.
4. Once compiled successfully, open the Command Palette (`Ctrl+Shift+P` or `Cmd+Shift+P`).
5. Run the command: `Wokwi: Start Simulator`.
6. Cue up *"End of Beginning"* by Djo, hit play on the track and the Wokwi simulator at the same time, and enjoy the show!

## 🧠 How the Code Works

Instead of relying on a continuous loop, the visualizer uses an `if / else if` structure to constantly check the elapsed milliseconds against predefined "Cues". 

```cpp
// Example of the Timecode architecture
const unsigned long chorus1Start = 36500;  // 0:36.5
const unsigned long verse2Start  = 64000;  // 1:04.0

if (elapsedTime >= chorus1Start && elapsedTime < verse2Start) {
    // Snap to the Golden Sunset palette and ramp up to 160 BPM
}
```
During the verses, a custom sequence handles the rhythm. It calculates the time inside a 3-second measure and fires off two distinct, sharp flashes for the drum hits before falling back into a smooth, sine-wave-driven "breath."

## 🔮 Future Improvements
While currently hard-coded via timecode for a specific song, the next iteration of this hardware build will replace the internal clock with an **MSGEQ7 Audio Graphic Equalizer IC**. This will allow the Arduino to physically "listen" to 7 different frequency bands in real-time, making the visualizer dynamically react to any song played into it.

***
