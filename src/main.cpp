#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN 6
#define NUM_LEDS 16
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

// --- PALETTE 1: The Drum Hits (Sharp, electric ice blues & cyans) ---
CRGBPalette16 verseDrumPalette = CRGBPalette16(
    CRGB::Aqua, CRGB::Cyan, CRGB::LightSkyBlue, CRGB::White,
    CRGB::DodgerBlue, CRGB::Aqua, CRGB::Cyan, CRGB::LightSkyBlue,
    CRGB::Aqua, CRGB::Cyan, CRGB::LightSkyBlue, CRGB::White,
    CRGB::DodgerBlue, CRGB::Aqua, CRGB::Cyan, CRGB::LightSkyBlue
);

// --- PALETTE 2: The Slow Idle Backdrop (Deep, moody ocean blues & indigos) ---
CRGBPalette16 verseIdlePalette = CRGBPalette16(
    CRGB::MidnightBlue, CRGB::DarkBlue, CRGB::Navy, CRGB::Indigo,
    CRGB::DarkSlateBlue, CRGB::MidnightBlue, CRGB::Navy, CRGB::DarkBlue,
    CRGB::MidnightBlue, CRGB::DarkBlue, CRGB::Navy, CRGB::Indigo,
    CRGB::DarkSlateBlue, CRGB::MidnightBlue, CRGB::Navy, CRGB::DarkBlue
);

// --- PALETTE 3: The Chorus Drop (Strictly warm sunsets & golds) ---
CRGBPalette16 chorusPalette = CRGBPalette16(
    CRGB::DarkRed, CRGB::FireBrick, CRGB::OrangeRed, CRGB::Gold,
    CRGB::Goldenrod, CRGB::DarkOrange, CRGB::OrangeRed, CRGB::FireBrick,
    CRGB::DarkRed, CRGB::FireBrick, CRGB::OrangeRed, CRGB::Gold,
    CRGB::Goldenrod, CRGB::DarkOrange, CRGB::OrangeRed, CRGB::FireBrick
);

// Timecode Cues (in milliseconds)
const unsigned long chorus1Start = 36500;  
const unsigned long verse2Start  = 64000;  
const unsigned long chorus2Start = 90100;  
const unsigned long finaleStart  = 135400; 

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
}

void loop() {
  unsigned long elapsedTime = millis();
  
  uint8_t colorSpread;
  CRGBPalette16 currentPalette; 
  uint8_t brightnessPulse;
  
  // 1. The continuous color spin speed (Runs all the time)
  uint8_t sweep = beat8(40); 
  
  // 2. THE OLD MAGIC: The slow, breathing pulse for the idle sections
  // This breathes between a dim 40 and a medium 120 brightness
  uint8_t idleBreath = beatsin8(15, 40, 120); 

  // ==========================================
  // SCENE LOGIC: Are we in a Verse or a Chorus?
  // ==========================================
  bool isChorus = false;

  if (elapsedTime >= chorus1Start && elapsedTime < verse2Start) {
    isChorus = true;
  } else if (elapsedTime >= chorus2Start && elapsedTime < finaleStart) {
    isChorus = true;
  } else if (elapsedTime >= finaleStart) {
    isChorus = true;
  }

  // ==========================================
  // BEAT LOGIC: How do the lights flash & color swap?
  // ==========================================
  
  if (isChorus) {
    // CHORUS: The Driving 160 BPM Thump (Gold)
    currentPalette = chorusPalette; 
    colorSpread = 15;  
    
    // Smooth sawtooth wave for the chorus beat
    uint8_t beatPhase = beat8(160);
    brightnessPulse = map(255 - beatPhase, 0, 255, 80, 255);
    
  } else {
    // VERSE: The Custom "Double-Tap" Drum Fill + Breathing Backdrop
    colorSpread = 2;   
    
    // We create a repeating 3000ms (3 second) measure
    unsigned long measureTime = elapsedTime % 3000; 
    
    // HIT 1: Max brightness + Ice Blue palette
    if (measureTime < 150) {
      brightnessPulse = 255;
      currentPalette = verseDrumPalette;
    } 
    // HIT 2: Max brightness + Ice Blue palette
    else if (measureTime > 400 && measureTime < 550) {
      brightnessPulse = 255;
      currentPalette = verseDrumPalette;
    } 
    // IDLE REST: Slow breathing glow + Moody Blue palette
    else {
      brightnessPulse = idleBreath; // Uses the smooth sine wave!
      currentPalette = verseIdlePalette; 
    }
  }

  // ==========================================
  // PAINT THE LEDs
  // ==========================================
  for(int i = 0; i < NUM_LEDS; i++) {
    uint8_t colorIndex = sweep + (i * colorSpread);
    leds[i] = ColorFromPalette(currentPalette, colorIndex, brightnessPulse, LINEARBLEND);
  }

  FastLED.show();
  FastLED.delay(1000 / 120); // Forces a smooth 120 FPS; 
}