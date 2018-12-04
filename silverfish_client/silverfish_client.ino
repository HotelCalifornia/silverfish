// "system" includes
#include <Arduino.h>
#include <ArduinoJson.h>
#include <AWS_IOT.h>
#include <jsmn.h>
#include <WiFi.h>
#include <StreamString.h>
#include <HTTPClient.h>
#include "esp_wpa2.h"

// system includes
#include <tuple>
// contains WIFI_SSID, WIFI_PASSWORD, and HOST_ADDR
#include "secrets.h"

// chassis
#include "Chassis.hpp"

AWS_IOT silverfish;

char CLIENT_ID[] = "silverfish";

int status = WL_IDLE_STATUS;

Chassis chassis(23, 22);

void handle_drive(char*, int, char* payload) {
  StaticJsonBuffer<JSON_OBJECT_SIZE(3)> jbuf;
  JsonObject& jpayload = jbuf.parseObject(payload);
  if (!jpayload.success()) {
    Serial.println("failed to parse payload");
    return;
  }
  int lp = jpayload["left_power"];
  int rp = jpayload["right_power"];
  int duration = jpayload["duration"];

  chassis.drive(lp, rp, duration);
  chassis.stop();
}

void test_sub(char* topic, int len, char* payload) {
  Serial.printf("received topic %s with payload %s\n", topic, payload);
}

void setup() {
  chassis.attachAll();

  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.persistent(false);
  Serial.begin(115200);
  int counter = 0;
  Serial.printf("\nAttempting to connect to network %s", "Pentho5e");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    counter++;
    if (counter >= 60) ESP.restart();
  }
  Serial.println("Done");
  
  Serial.print("Attempting to connect to AWS... ");
  if (silverfish.connect(HOST_ADDR, CLIENT_ID)) {
    Serial.println("Could not connect to AWS. Check host address.");
    // trap
    for (;;) ;
  }
  Serial.println("Done");
  delay(5000);
  silverfish.publish((char*)"robot/status", (char*)"{\"ready\":true}");
  silverfish.subscribe((char*)"robot/drive", handle_drive);
}

void loop() {
  // noop
}
