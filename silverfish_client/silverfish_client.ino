#include <AWS_IOT.h>
//#include <aws_iot_config.h>
//#include <aws_iot_error.h>
//#include <aws_iot_json_utils.h>
//#include <aws_iot_log.h>
//#include <aws_iot_mqtt_client.h>
//#include <aws_iot_mqtt_client_common_internal.h>
//#include <aws_iot_mqtt_client_interface.h>
//#include <aws_iot_shadow_actions.h>
//#include <aws_iot_shadow_interface.h>
//#include <aws_iot_shadow_json.h>
//#include <aws_iot_shadow_json_data.h>
//#include <aws_iot_shadow_key.h>
//#include <aws_iot_shadow_records.h>
//#include <aws_iot_version.h>

// esp32 SDK includes JSMN so no need to use AWS_IOT's version
// #include <jsonParser.h>

//#include <network_interface.h>
//#include <network_platform.h>
//#include <threads_interface.h>
//#include <threads_platform.h>
//#include <timer_interface.h>
//#include <timer_platform.h>

#include <ESP32_Servo.h>

#include <WiFi.h>

// contains WIFI_SSID, WIFI_PASSWORD, and HOST_ADDR
#include "secrets.h"

#include "Chassis.hpp"

AWS_IOT silverfish;

char CLIENT_ID[] = "silverfish";
char TOPIC_NAM[] = "$aws/things/silverfish/shadow/update";

int status = WL_IDLE_STATUS;

Chassis chassis(22, 23);

void handle_forward(char*, int, char* payload) {
  // TODO
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
  silverfish.subscribe((char*)"robot/drive/forward", test_sub);
  silverfish.subscribe((char*)"robot/drive/reverse", test_sub);
  silverfish.subscribe((char*)"robot/turn/cw", test_sub);
  silverfish.subscribe((char*)"robot/turn/ccw", test_sub);
}

void loop() {
  // noop
}
