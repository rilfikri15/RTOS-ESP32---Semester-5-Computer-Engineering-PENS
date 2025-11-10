#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <ESP32Servo.h>

// Konfigurasi pin
#define SDA_PIN 13
#define SCL_PIN 14
#define SERVO_PIN 20

// OLED setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Servo setup
Servo myServo;
int servoAngle = 0;
bool increasing = true;

// Task handles
TaskHandle_t oledTaskHandle = NULL;
TaskHandle_t servoTaskHandle = NULL;

// OLED 
void OledTask(void *parameter) {
  (void)parameter;

  for (;;) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(5, 25);
    display.println("FreeRTOS");
    display.setTextSize(1);
    display.setCursor(30, 50);
    display.println("Running...");
    display.display();

    vTaskDelay(pdMS_TO_TICKS(500)); // update tiap 0.5 detik
  }
}

// SERVO
void ServoTask(void *parameter) {
  (void)parameter;

  for (;;) {
    myServo.write(servoAngle);

    if (increasing)
      servoAngle += 5;
    else
      servoAngle -= 5;

    if (servoAngle >= 180) increasing = false;
    if (servoAngle <= 0) increasing = true;

    vTaskDelay(pdMS_TO_TICKS(150)); // kecepatan servo
  }
}

// SETUP 
void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Program 1: OLED + Servo (FreeRTOS)");

  // Inisialisasi OLED
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("Gagal inisialisasi OLED!");
    for (;;);
  }
  display.clearDisplay();
  display.display();

  // Inisialisasi Servo
  myServo.attach(SERVO_PIN);

  // Buat task
  xTaskCreatePinnedToCore(OledTask, "OledTask", 4096, NULL, 1, &oledTaskHandle, 0);
  xTaskCreatePinnedToCore(ServoTask, "ServoTask", 2048, NULL, 1, &servoTaskHandle, 1);
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(1000));
}