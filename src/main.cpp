// ===== LIBRARIES =====
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// ===== UART =====
#define RXp2 16
#define TXp2 17

// ===== WEB SERVER =====
// Configuration
AsyncWebServer server(80);
const char *ssid = "WebChamber";
const char *password = "helloworld";

// ===== PARAMETERS =====
// Web Parameters
const char* PARAM_1 = "toneFrequency";
const char* PARAM_2 = "toneTime";
const  char* PARAM_3 = "stimulationTime";  
const char* PARAM_4 = "movementAnalysisTime";
const char* PARAM_5 = "intervalTime";
const char* PARAM_6 = "numberOfEvents";

// ===== INDEX.HTML  =====
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Conditioning Chamber v0.1</title>
<style>
body{ margin: 0;padding: 0;font-family: Arial, Helvetica, sans-serif;background-color: #d6d6d6;}
.box{ width: 100%; padding: 40px; position: absolute; top: 50%; left: 50%; transform: translate(-50%,-50%); background-color: #43506C; color: white; text-align: center; border-radius: 24px;}
h1{ text-transform: uppercase; font-size: 1.5em;}
input{ border: 0; display: block; background: #E9E9EB; margin: 20px auto; text-align: center; border: 2px solid #3d619b; padding: 14px 10px; width: 50%; outline: none; border-radius: 5px; color: rgb(0, 0, 0); font-size: smaller; transition: 0.3s;}
input:focus{ width: 90%; border-color:#3d619b ;}
input[type='submit']{ border: 0; display: block; background:  #ef4b4c; margin: 20px auto; text-align: center;; padding: 14px 10px; width: 140px; outline: none; border-radius: 15px; color: white; transition: 0.3s; cursor: pointer;}
</style>
</head>
<body>
<form action="/get" class="box" id="my-form">
<p>Tone</p>
<div class="part">
<input name="toneFrequency" type="text" placeholder="Frequency (Hz)">
</div>
<div class="part">
<input name="toneTime" type="text" placeholder="Duration (s)">
</div>
<p>Stimulation</p>
<div class="part">
<input name="stimulationTime" type="text" placeholder="Duration (s)">
</div>
<p>Experiment</p>
<div class="part">
<input name="movementAnalysisTime" placeholder="Movement Analysis (s)">
</div>
<div class="part">
<input name="intervalTime" placeholder="Interval Duration (s)">
</div>
<div class="part">
<input name="numberOfEvents" placeholder="Number of Events (n)">
</div>
<input type="submit" value="SET PARAMETERS">
</form></body></html>
)rawliteral";

// ===== 404 =====
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

// ===== SETUP =====
void setup() {

  // Serial Communication
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXp2, TXp2);

  // Access Point
  WiFi.softAP(ssid, password);

  // Web page request
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Parameters Request
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1,inputMessage2,inputMessage3, inputMessage4, inputMessage5, inputMessage6;
    // Tone Frequency
    if (request->getParam(PARAM_1)->value() != "") {
      inputMessage1 = request->getParam(PARAM_1)->value();
    } else {
      inputMessage1 = "0";
    }
    // Tone Time
    if (request->getParam(PARAM_2)->value() != "") {
      inputMessage2 = request->getParam(PARAM_2)->value();
    } else {
      inputMessage2 = "0";
    }
    // Stimulation Time
    // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
    if (request->getParam(PARAM_3)->value() != "") {
      inputMessage3 = request->getParam(PARAM_3)->value();
    } else {
      inputMessage3 = "0";
    }
    // Movement Analysis Time
    if (request->getParam(PARAM_4)->value() != "") {
      inputMessage4 = request->getParam(PARAM_4)->value();
    } else {
      inputMessage4 = "0";
    }
    // Interval Time
    if (request->getParam(PARAM_5)->value() != "") {
      inputMessage5 = request->getParam(PARAM_5)->value();
    } else {
      inputMessage5 = "0";
    }
    // Number of Events
    if (request->getParam(PARAM_6)->value() != "") {
      inputMessage6 = request->getParam(PARAM_6)->value();
    } else {
      inputMessage6 = "0";
    }

    // Send parameters via UART
    Serial2.println(inputMessage1 + "," + inputMessage2 + "," + inputMessage3 + "," + inputMessage4 + "," + inputMessage5 + "," + inputMessage6);
    
    request->send(200, "text/html", "Done"); 
});
  server.onNotFound(notFound);
  server.begin();
}
void loop() {
  
}