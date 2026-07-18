#include "EmergencyRatFace.h"

namespace {
  // Helper functions for smooth animations
  int triWave(uint16_t phase, int amplitude) {
    uint8_t p = phase & 0xFF;
    int v = p < 128 ? p : 255 - p;
    return ((v * 2 * amplitude) / 255) - amplitude;
  }
  
  float easeInOutCubic(float t) {
    return t < 0.5 ? 4 * t * t * t : 1 - pow(-2 * t + 2, 3) / 2;
  }
}

EmergencyRatFace::EmergencyRatFace() 
  : currentState(RecoveryState::BOOTING)
  , stateString("BOOTING")
  , lastFrameTime(0)
  , lastBlinkTime(0)
  , lastTwitchTime(0)
  , isBlinking(false)
  , isTwitching(false)
  , blinkProgress(0)
  , twitchProgress(0)
  , whiskerPhase(0) {
}

// Global instance definition
EmergencyRatFace ratFace;

void EmergencyRatFace::begin() {
  Serial.println("Emergency RatFace: Starting recovery system...");
  
  // Initialize canvas with conservative settings
  canvas.setColorDepth(8);
  if (!canvas.createSprite(DISPLAY_WIDTH, DISPLAY_HEIGHT)) {
    Serial.println("Emergency RatFace: Canvas allocation failed!");
    currentState = RecoveryState::ERROR;
    return;
  }
  
  currentState = RecoveryState::BOOTING;
  stateString = "BOOTING";
  
  Serial.println("Emergency RatFace: System initialized");
}

void EmergencyRatFace::setState(const String& state) {
  if (state == "BOOTING") currentState = RecoveryState::BOOTING;
  else if (state == "IDLE") currentState = RecoveryState::IDLE;
  else if (state == "TESTING") currentState = RecoveryState::TESTING;
  else if (state == "ERROR") currentState = RecoveryState::ERROR;
  else if (state == "RECOVERED") currentState = RecoveryState::RECOVERED;
  else currentState = RecoveryState::ERROR;
  
  stateString = state;
  Serial.printf("Emergency RatFace: State changed to %s\n", stateString.c_str());
}

void EmergencyRatFace::update() {
  unsigned long currentTime = millis();
  
  // Update animations
  updateBlink();
  updateTwitch();
  updateWhiskers();
}

void EmergencyRatFace::updateBlink() {
  unsigned long currentTime = millis();
  
  if (!isBlinking && currentTime - lastBlinkTime > BLINK_INTERVAL_IDLE_MS) {
    isBlinking = true;
    blinkProgress = 0;
    lastBlinkTime = currentTime;
  }
  
  if (isBlinking) {
    blinkProgress += 10;
    if (blinkProgress >= 100) {
      isBlinking = false;
    }
  }
}

void EmergencyRatFace::updateTwitch() {
  unsigned long currentTime = millis();
  
  if (!isTwitching && currentTime - lastTwitchTime > TWITCH_INTERVAL_IDLE_MS) {
    isTwitching = true;
    twitchProgress = 0;
    lastTwitchTime = currentTime;
  }
  
  if (isTwitching) {
    twitchProgress += 15;
    if (twitchProgress >= 100) {
      isTwitching = false;
    }
  }
}

void EmergencyRatFace::updateWhiskers() {
  whiskerPhase = (whiskerPhase + 1) % 360;
}

void EmergencyRatFace::draw() {
  // Clear canvas
  canvas.fillSprite(BACKGROUND_COLOR);
  
  // Draw based on current state
  switch (currentState) {
    case RecoveryState::BOOTING:
      drawBootingState();
      break;
    case RecoveryState::IDLE:
      drawIdleState();
      break;
    case RecoveryState::TESTING:
      drawTestingState();
      break;
    case RecoveryState::ERROR:
      drawErrorState();
      break;
    case RecoveryState::RECOVERED:
      drawRecoveredState();
      break;
  }
  
  // Push to display
  canvas.pushSprite(0, 0);
}

void EmergencyRatFace::drawFace() {
  // Draw main face
  canvas.fillCircle(DISPLAY_CENTER_X, DISPLAY_CENTER_Y, FACE_RADIUS, FACE_COLOR);
  
  // Draw face outline
  canvas.drawCircle(DISPLAY_CENTER_X, DISPLAY_CENTER_Y, FACE_RADIUS, FACE_COLOR);
}

void EmergencyRatFace::drawEyes() {
  int leftEyeX = DISPLAY_CENTER_X - 22;
  int rightEyeX = DISPLAY_CENTER_X + 22;
  int eyeY = DISPLAY_CENTER_Y - 20;
  
  if (isBlinking) {
    // Blinking eyes
    int eyeHeight = map(blinkProgress, 0, 100, EYE_RADIUS, 2);
    if (eyeHeight > 0) {
      canvas.fillRect(leftEyeX - EYE_RADIUS/2, eyeY - eyeHeight/2, EYE_RADIUS, eyeHeight, EYE_COLOR);
      canvas.fillRect(rightEyeX - EYE_RADIUS/2, eyeY - eyeHeight/2, EYE_RADIUS, eyeHeight, EYE_COLOR);
    }
  } else {
    // Normal eyes with pupils
    canvas.fillCircle(leftEyeX, eyeY, EYE_RADIUS, EYE_COLOR);
    canvas.fillCircle(rightEyeX, eyeY, EYE_RADIUS, EYE_COLOR);
    canvas.fillCircle(leftEyeX, eyeY, PUPIL_RADIUS, PUPIL_COLOR);
    canvas.fillCircle(rightEyeX, eyeY, PUPIL_RADIUS, PUPIL_COLOR);
  }
}

void EmergencyRatFace::drawNose() {
  int noseX = DISPLAY_CENTER_X;
  int noseY = DISPLAY_CENTER_Y + 10;
  
  // Triangular nose
  canvas.fillTriangle(noseX - NOSE_SIZE/2, noseY + NOSE_SIZE/3,
                      noseX + NOSE_SIZE/2, noseY + NOSE_SIZE/3,
                      noseX, noseY - NOSE_SIZE/2, NOSE_COLOR);
  
  // Nostrils
  canvas.fillCircle(noseX - 3, noseY + 3, 2, TFT_DARKGREY);
  canvas.fillCircle(noseX + 3, noseY + 3, 2, TFT_DARKGREY);
}

void EmergencyRatFace::drawEars() {
  int earTwitch = isTwitching ? triWave(whiskerPhase, 2) : 0;
  
  // Left ear
  canvas.fillTriangle(DISPLAY_CENTER_X - 30, DISPLAY_CENTER_Y - 30 - earTwitch,
                      DISPLAY_CENTER_X - 48, DISPLAY_CENTER_Y - 55,
                      DISPLAY_CENTER_X - 24, DISPLAY_CENTER_Y - 36, EAR_COLOR);
  
  // Right ear
  canvas.fillTriangle(DISPLAY_CENTER_X + 30, DISPLAY_CENTER_Y - 30 + earTwitch,
                      DISPLAY_CENTER_X + 48, DISPLAY_CENTER_Y - 55,
                      DISPLAY_CENTER_X + 24, DISPLAY_CENTER_Y - 36, EAR_COLOR);
}

void EmergencyRatFace::drawWhiskers() {
  int whiskerOffset = triWave(whiskerPhase, 5);
  
  // Left whiskers
  for (int i = 0; i < 3; i++) {
    int y = DISPLAY_CENTER_Y - 5 + i * 5;
    canvas.drawLine(DISPLAY_CENTER_X - 25, y, DISPLAY_CENTER_X - 80 - whiskerOffset, y, WHISKER_COLOR);
  }
  
  // Right whiskers
  for (int i = 0; i < 3; i++) {
    int y = DISPLAY_CENTER_Y - 5 + i * 5;
    canvas.drawLine(DISPLAY_CENTER_X + 25, y, DISPLAY_CENTER_X + 80 + whiskerOffset, y, WHISKER_COLOR);
  }
}

void EmergencyRatFace::drawMouth() {
  int mouthY = DISPLAY_CENTER_Y + 25;
  
  // Simple mouth
  canvas.drawLine(DISPLAY_CENTER_X - 10, mouthY, DISPLAY_CENTER_X + 10, mouthY, EYE_COLOR);
}

void EmergencyRatFace::drawTeeth() {
  int mouthY = DISPLAY_CENTER_Y + 25;
  
  // Small teeth
  canvas.fillRect(DISPLAY_CENTER_X - 4, mouthY - 2, 4, 4, TFT_WHITE);
  canvas.fillRect(DISPLAY_CENTER_X, mouthY - 2, 4, 4, TFT_WHITE);
}

void EmergencyRatFace::drawStatusText() {
  canvas.setTextColor(TFT_WHITE);
  canvas.setTextSize(2);
  canvas.setCursor(10, 10);
  canvas.printf("RECOVERY MODE: %s", stateString.c_str());
}

void EmergencyRatFace::drawBootingState() {
  drawFace();
  drawEars();
  drawEyes();
  drawNose();
  drawWhiskers();
  drawMouth();
  drawStatusText();
  
  canvas.setTextSize(1);
  canvas.setCursor(10, 50);
  canvas.println("Emergency recovery system");
  canvas.println("Starting diagnostics...");
}

void EmergencyRatFace::drawIdleState() {
  drawFace();
  drawEars();
  drawEyes();
  drawNose();
  drawWhiskers();
  drawMouth();
  drawStatusText();
}

void EmergencyRatFace::drawTestingState() {
  drawFace();
  drawEars();
  drawEyes();
  drawNose();
  drawWhiskers();
  drawMouth();
  drawStatusText();
  
  canvas.setTextSize(1);
  canvas.setCursor(10, 50);
  canvas.println("Testing display...");
  canvas.println("Testing animations...");
  canvas.println("Testing connectivity...");
}

void EmergencyRatFace::drawErrorState() {
  drawFace();
  drawEars();
  drawEyes();
  drawNose();
  drawWhiskers();
  drawMouth();
  drawStatusText();
  
  canvas.setTextSize(1);
  canvas.setCursor(10, 50);
  canvas.setTextColor(TFT_RED);
  canvas.println("ERROR!");
  canvas.println("Recovery failed");
  canvas.println("Check hardware");
}

void EmergencyRatFace::drawRecoveredState() {
  drawFace();
  drawEars();
  drawEyes();
  drawNose();
  drawWhiskers();
  drawMouth();
  drawTeeth();
  drawStatusText();
  
  canvas.setTextSize(1);
  canvas.setCursor(10, 50);
  canvas.setTextColor(TFT_GREEN);
  canvas.println("RECOVERY SUCCESS!");
  canvas.println("System operational");
  canvas.println("Ready for use");
}