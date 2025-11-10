#include <Arduino.h>

#define BUTTON_PIN 36
#define LED_PIN    39

TaskHandle_t buttonTaskHandle = NULL;
TaskHandle_t ledBlinkTaskHandle = NULL;

// Button
void ButtonTask(void *parameter) {
  (void)parameter;

  for (;;) {
    int buttonState = digitalRead(BUTTON_PIN);

    if (buttonState == LOW) {
      Serial.println("Tombol DITEKAN");
    } else {
      Serial.println("---");
    }

    vTaskDelay(pdMS_TO_TICKS(300));
  }
}

// LED Blink
void LedBlinkTask(void *parameter) {
  (void)parameter;
  pinMode(LED_PIN, OUTPUT);

  for (;;) {
    digitalWrite(LED_PIN, HIGH);
    vTaskDelay(pdMS_TO_TICKS(500));
    digitalWrite(LED_PIN, LOW);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

// SETUP 
void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("ESP32-S3 FreeRTOS: Button + LED Blink 500 ms");

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  // Task 1:
  xTaskCreatePinnedToCore(
    ButtonTask,
    "ButtonTask",
    2048,
    NULL,
    1,
    &buttonTaskHandle,
    1
  );

  // Task 2:
  xTaskCreatePinnedToCore(
    LedBlinkTask,
    "LedBlinkTask",
    2048,
    NULL,
    1,
    &ledBlinkTaskHandle,
    0
  );
}

void loop() {
  // Kosong – semua berjalan lewat FreeRTOS task
  vTaskDelay(pdMS_TO_TICKS(1000));
}