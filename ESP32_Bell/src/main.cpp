/*
 Copyright 2024 Tauno Erik

 By Tauno Erik
 Modified 31.08.2024
*/
#include <Arduino.h>
#include <ESP32Servo.h>  // https://github.com/madhephaestus/ESP32Servo

#include "secret.h"  // Wifi ssid & pass
// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

BlynkTimer blynk_timer;

// Servo
const int SERVO_PIN = 18;
const int SERVO_DELAY = 2;
const int SERVO_MIN = 500;
const int SERVO_MAX = 2400;
Servo myservo;

// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33
// Possible PWM GPIO pins on the ESP32-S2:
//   0(used by on-board button),1-17,18(used by on-board LED),19-21,26,33-42
// Possible PWM GPIO pins on the ESP32-S3:
//    0(used by on-board button),1-21,35-45,47,48(used by on-board LED)
// Possible PWM GPIO pins on the ESP32-C3:
//    0(used by on-board button),1-7,8(used by on-board LED),9-10,18-21

/*
 Move servo to +180 position
*/
void servo_move_plus(int delay_val) {
  for (int pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);
    delay(delay_val);
  }
}

/*
 Move servo to 0 position.
*/
void servo_move_minus(int delay_val) {
  for (int pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(delay_val);
  }
}

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0) {
  Serial.print("V0 button: ");
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  Serial.println(value);

  // Update state
  Blynk.virtualWrite(V1, value);

  // If is a button signal
  if (value == 1) {
    if (!myservo.attached()) {
      myservo.setPeriodHertz(50);  // standard 50 hz servo
      myservo.attach(SERVO_PIN, SERVO_MIN, SERVO_MAX);
    }

    servo_move_plus(SERVO_DELAY);

    Blynk.virtualWrite(V0, 0);
    servo_move_minus(SERVO_DELAY);

    Blynk.virtualWrite(V1, 0);  // Write V1 pin to 0
    myservo.detach();  // Turn the servo off
  }
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED() {
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent() {
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}


////////////////////////////////////////////////////////////
// https://community.blynk.cc/t/how-i-can-use-multiple-wifi-network/31667/15
char* SSIDs[] = {SSID_1, SSID_2};  // list a wifi networks
char* PASSs[] = {PASS_1, PASS_2};  // list a passwords

void MultyWiFiBlynkBegin() {
  int ssid_count = 0;
  int ssid_mas_size = sizeof(SSIDs) / sizeof(SSIDs[0]);

  do {
    Serial.println("\nConnect to wi-fi " + String(SSIDs[ssid_count]));
    WiFi.begin(SSIDs[ssid_count], PASSs[ssid_count]);
    int WiFi_timeout_count = 0;

    while (WiFi.status() != WL_CONNECTED && WiFi_timeout_count < 50) {
      delay(200);
      Serial.print(".");
      ++WiFi_timeout_count;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Connected to WiFi!");
      Blynk.config(BLYNK_AUTH_TOKEN);
      Blynk.connect(5000);  // waiting 5 sec
    }
    ++ssid_count;
  } while (!Blynk.connected() && ssid_count < ssid_mas_size);

  if (!Blynk.connected() && ssid_count == ssid_mas_size) {
    Serial.println("Could not connect to blynk");
  }
}


////////////////////////////////////////////////////////////


void setup() {
  Serial.begin(115200);

  delay(500);
  Serial.println("\nSetup!");
  MultyWiFiBlynkBegin();

  // Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  // Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  // Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Blynk timer
  // Setup a function to be called every second
  blynk_timer.setInterval(1000L, myTimerEvent);

  // Servo
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  // myservo.setPeriodHertz(50);    // standard 50 hz servo
  // myservo.attach(SERVO_PIN, SERVO_MIN, SERVO_MAX);  // 1000, 2000 // 500, 2500
  
  // using default min/max of 1000us and 2000us
  // different servos may require different min/max settings
  // for an accurate 0 to 180 sweep
}

void loop() {
  Blynk.run();
  blynk_timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}
