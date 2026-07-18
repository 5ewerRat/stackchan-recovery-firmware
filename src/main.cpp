#include <Arduino.h>
#include <M5Unified.h>
#include "config.h"
#include "EmergencyRatFace.h"

// Global variables
EmergencyRatFace emergencyRatFace;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("=== EMERGENCY STACKCHAN RECOVERY ===");
  Serial.println("Starting device recovery process...");
  
  // Initialize M5Stack
  M5.begin();
  M5.Power.begin();
  
  // Initialize LED and speaker pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(SPEAKER_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(SPEAKER_PIN, LOW);
  
  // Initialize recovery system
  Serial.println("Initializing emergency recovery system...");
  emergencyRatFace.begin();
  emergencyRatFace.setState("BOOTING");
  
  // Test basic functionality
  Serial.println("Testing basic functionality...");
  
  // LED test
  digitalWrite(LED_PIN, HIGH);
  delay(200);
  digitalWrite(LED_PIN, LOW);
  
  // Speaker test
  tone(SPEAKER_PIN, 1000, 100);
  
  Serial.println("Emergency recovery system initialized!");
  Serial.println("Starting recovery sequence...");
  
  // Move to testing phase
  emergencyRatFace.setState("TESTING");
  delay(2000);
  
  // Check display
  if (M5.Display.width() == 0 || M5.Display.height() == 0) {
    Serial.println("❌ Display initialization failed!");
    emergencyRatFace.setState("ERROR");
    while (1) {
      delay(1000);
    }
  }
  
  Serial.println("✅ Display functional");
  
  // Move to idle phase
  emergencyRatFace.setState("IDLE");
  delay(1000);
  
  // Final check
  emergencyRatFace.setState("RECOVERED");
  Serial.println("=== RECOVERY COMPLETE ===");
  Serial.println("StackChan emergency recovery successful!");
  Serial.println("System ready for use");
}

void loop() {
  static unsigned long stateChangeTime = 0;
  static int currentStateIndex = 0;
  static const char* states[] = {"IDLE", "TESTING", "RECOVERED"};
  
  // Update M5 system
  M5.update();
  
  // Cycle through states for testing
  if (millis() - stateChangeTime > 5000) {
    currentStateIndex = (currentStateIndex + 1) % 3;
    ratFace.setState(states[currentStateIndex]);
    Serial.printf("Recovery test state: %s\n", states[currentStateIndex]);
    stateChangeTime = millis();
  }
  
  // Update rat face animations
  emergencyRatFace.update();
  emergencyRatFace.draw();
  
  // Small delay for animation smoothness
  delay(FRAME_INTERVAL_MS);
}