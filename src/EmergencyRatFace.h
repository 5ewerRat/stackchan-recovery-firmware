#include <Arduino.h>
#include <M5Unified.h>
#include "config.h"

// Emergency RatFace system for recovery
class EmergencyRatFace {
public:
  EmergencyRatFace();
  void begin();
  void update();
  void setState(const String& state);
  void draw();
  
private:
  // Animation states
  enum class RecoveryState {
    BOOTING,
    IDLE,
    TESTING,
    ERROR,
    RECOVERED
  };
  
  RecoveryState currentState;
  String stateString;
  
  // Animation timing
  unsigned long lastFrameTime;
  unsigned long lastBlinkTime;
  unsigned long lastTwitchTime;
  bool isBlinking;
  bool isTwitching;
  int blinkProgress;
  int twitchProgress;
  int whiskerPhase;
  
  // Canvas for drawing
  M5Canvas canvas;
  
  // Animation methods
  void updateBlink();
  void updateTwitch();
  void updateWhiskers();
  
  // Drawing methods
  void drawFace();
  void drawEyes();
  void drawNose();
  void drawEars();
  void drawWhiskers();
  void drawMouth();
  void drawTeeth();
  void drawStatusText();
  
  // State-specific drawings
  void drawBootingState();
  void drawIdleState();
  void drawTestingState();
  void drawErrorState();
  void drawRecoveredState();
};

// Global instance (defined in EmergencyRatFace.cpp)
extern EmergencyRatFace ratFace;