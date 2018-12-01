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

std::tuple<int, int> parse_payload(char* payload) {
  Serial.println("parsing payload");
  StaticJsonBuffer<JSON_OBJECT_SIZE(2)> jbuf;
  JsonObject& jpayload = jbuf.parseObject(payload);
  if (!jpayload.success()) {
    Serial.println("failed to parse payload");
    return std::make_tuple(0, 0);
  }
  int power = jpayload["power"];
  int duration = jpayload["duration"];
  return std::make_tuple(power, duration);
}

void handle_drive(char*, int, char* payload) {
  StaticJsonBuffer<JSON_OBJECT_SIZE(2)> jbuf;
  JsonObject& jpayload = jbuf.parseObject(payload);
  if (!jpayload.success()) {
    Serial.println("failed to parse payload");
    return;
  }
  int power = jpayload["power"];
  int duration = jpayload["duration"];

  chassis.drive(power, duration);
  chassis.stop();
}

void handle_cw(char*, int, char* payload) {
  StaticJsonBuffer<JSON_OBJECT_SIZE(2)> jbuf;
  JsonObject& jpayload = jbuf.parseObject(payload);
  if (!jpayload.success()) {
    Serial.println("failed to parse payload");
    return;
  }
  int power = jpayload["power"];
  int duration = jpayload["duration"];
  
  chassis.turnCW(power, duration);
  chassis.stop();
}

void handle_ccw(char*, int, char* payload) {
  auto pl = parse_payload(payload);
  chassis.turnCCW(std::get<0>(pl), std::get<1>(pl));
  chassis.stop();
}

void test_sub(char* topic, int len, char* payload) {
  Serial.printf("received topic %s with payload %s\n", topic, payload);
}

//extern "C" {
//#include <user_interface.h>
//}

void makeRandomMac(uint8_t mac[6]) {
  for (size_t i = 0; i < 6; ++i) {
    mac[i] = random(256);
  }
  mac[0] = mac[0] & ~0x01;
}

bool changeMac(const uint8_t mac[6]) {
  return esp_base_mac_addr_set(const_cast<uint8_t*>(mac));
}
String extract_html_attr(const String& tag, String attr) {
  attr += "=\"";
  int first = tag.indexOf(attr);
  if (first < 0) return "";
  first += attr.length();
  int last = tag.indexOf("\"", first);
  if (last < 0) return "";
  return tag.substring(first, last);
}
String escape_form_field(const String& input) {
  StreamString escaped;
  for (size_t i = 0; i < input.length(); i++) {
    char ch = input[i];
    if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
      escaped.write(ch);
    } else escaped.printf("%%%02x", ch);
  }
  return escaped;
}
bool captive_login() {
  static const char* LOCATION = "Location";
  String uri;
  {
    HTTPClient http;
    http.begin("http://captive.apple.com/");
    http.collectHeaders(&LOCATION, 1);
    int code = http.GET();
    if (code == 200) return true;
    if (code != 302 || !http.hasHeader(LOCATION)) {
      Serial.printf("captive error: %d\n", code);
      return false;
    }
    uri = http.header(LOCATION);
    http.end();
    Serial.print("portal=");
    Serial.println(uri);
    delay(2000);
  }
  String host;
  {
    int pos = uri.indexOf("://");
    if (pos < 0) return false;
    pos = uri.indexOf("/", pos + 3);
    if (pos < 0) return false;
    host = uri.substring(0, pos);
  }
  Serial.print("host=");
  Serial.println(host);
  String form;
  {
    HTTPClient http;
    http.begin(uri);
    int code = http.GET();
    if (code != 200) {
      Serial.printf("portal error: %d\n", code);
      return false;
    }
    delay(100);
    Stream& payload = http.getStream();
    while (payload.available()) {
      String line = payload.readStringUntil('\n');
      if (line.indexOf("<form ") >= 0) {
        String tmp = extract_html_attr(line, "action");
        if (tmp.indexOf("//") == 0) { // handle weird purdue case with protocol-relative url
          int f = tmp.indexOf("/", 2);
          tmp = tmp.substring(f);
        }
        uri = host + tmp;
        Serial.print("submit=");
        Serial.println(uri);
        continue;
      }
      if (line.indexOf("<input ") < 0) {
        continue;
      }
      String name = extract_html_attr(line, "name");
      String value = extract_html_attr(line, "value");
      String escaped = escape_form_field(value);
      if (form.length() > 0) {
        form += "&";
      }
      form += name + "=" + escaped;
    }
    http.end();
    delay(3000);
  }
  {
    Serial.print("form: ");
    Serial.println(form);
    HTTPClient http;
    http.begin(uri);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int code = http.POST(form);
    if (code != 200) {
      Serial.printf("submit error: %d\n", code);
      return false;
    }
    http.end();
    delay(500);
  }
  {
    HTTPClient http;
    http.begin("http://captive.apple.com/");
    int code = http.GET();
    if (code != 200) {
      Serial.printf("verify error: %d\n", code);
      return false;
    }
    http.end();
    return true;
  }
}

void setup() {
  chassis.attachAll();

  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.persistent(false);
//  uint8_t mac[6];
//  makeRandomMac(mac);
//  changeMac(mac);
//  String hostname = "iPad-";
//  hostname += random(10);
//  hostname += random(10);
//  hostname += random(10);
//  hostname += random(10);
//  WiFi.setHostname(hostname.c_str());
//  esp_wifi_sta_wpa2_ent_set_identity((uint8_t*)EAP_IDENTITY, strlen(EAP_IDENTITY));
//  esp_wifi_sta_wpa2_ent_set_username((uint8_t*)EAP_IDENTITY, strlen(EAP_IDENTITY));
//  esp_wifi_sta_wpa2_ent_set_password((uint8_t*)EAP_PASSWORD, strlen(EAP_PASSWORD));
//  esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT();
//  esp_wifi_sta_wpa2_ent_enable(&config);
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
  
  Serial.println("Attempting to captive login...");
  if (!captive_login()) ESP.restart();
  
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
  silverfish.subscribe((char*)"robot/turn/cw", handle_cw);
  silverfish.subscribe((char*)"robot/turn/ccw", handle_ccw);
}

void loop() {
  // noop
}
