// system includes
#include <string>
#include <vector>

// "system" includes
#include <AWS_IOT.h>
#include <jsmn.h>
#include <WiFi.h>

// contains WIFI_SSID, WIFI_PASSWORD, and HOST_ADDR
#include "secrets.h"
// chassis
#include "Chassis.hpp"

AWS_IOT silverfish;

char CLIENT_ID[] = "silverfish";

int status = WL_IDLE_STATUS;

Chassis chassis(23, 22);

void handle_forward(char*, int, char* payload) {
  // TODO: get power, duration from payload
  chassis.forward(80, 1000);
  chassis.stop();
}

void handle_reverse(char*, int, char* payload) {
  // TODO
}

void handle_cw(char*, int, char* payload) {
  // TODO
}

void handle_ccw(char*, int, char* payload) {
  // TODO
}

void test_sub(char* topic, int len, char* payload) {
  Serial.printf("received topic %s with payload %s\n", topic, payload);
}

void setup() {
  chassis.attachAll();

  WiFi.disconnect(true);
  Serial.begin(115200);
  while (status != WL_CONNECTED) {
    Serial.printf("\nAttempting to connect to network %s... ", WIFI_SSID);
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(5000);
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
  silverfish.subscribe((char*)"robot/drive/forward", handle_forward);
//  silverfish.subscribe((char*)"robot/drive/reverse", test_sub);
//  silverfish.subscribe((char*)"robot/turn/cw", test_sub);
//  silverfish.subscribe((char*)"robot/turn/ccw", test_sub);
}

void loop() {
  // noop
}
