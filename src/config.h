#pragma once

// Emergency StackChan Recovery Configuration
#define DEBUG Serial

// Display settings
#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240
#define DISPLAY_CENTER_X DISPLAY_WIDTH / 2
#define DISPLAY_CENTER_Y DISPLAY_HEIGHT / 2

// Animation timing
#define FRAME_INTERVAL_MS 35          // ~28 FPS
#define BLINK_INTERVAL_IDLE_MS 3000    // 3 seconds
#define TWITCH_INTERVAL_IDLE_MS 2000   // 2 seconds
#define BLINK_DURATION_MS 150          // 150ms
#define TWITCH_DURATION_MS 100         // 100ms

// Rat face dimensions (authentic proportions)
#define FACE_RADIUS 55
#define EYE_RADIUS 12
#define PUPIL_RADIUS 4
#define NOSE_SIZE 10
#define EAR_SIZE 18
#define EAR_LENGTH 25
#define WHISKER_LENGTH 45
#define TOOTH_SIZE 4
#define SNOUT_LENGTH 15

// Colors (rat theme)
#define FACE_COLOR TFT_BROWN
#define EYE_COLOR TFT_WHITE
#define PUPIL_COLOR TFT_BLACK
#define NOSE_COLOR TFT_PINK
#define EAR_COLOR TFT_BROWN
#define WHISKER_COLOR TFT_DARKGREY
#define BACKGROUND_COLOR TFT_BLACK

// Pin definitions
#define LED_PIN 2
#define SPEAKER_PIN 5