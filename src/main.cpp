// ===== LIBRARIES =====
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

// ===== WEB SERVER =====
// Configuration
AsyncWebServer server(80);
const char *ssid = "WebChamber";
const char *password = "helloworld";

// ===== PARAMETERS =====
// Web Parameters
const char* PARAM_1 = "dayOfExperiment";
const char* PARAM_2 = "experimentAnimal";
const char* PARAM_3 = "explorationTime";
const char* PARAM_4 = "toneFrequency";
const char* PARAM_5 = "toneTime";
const  char* PARAM_6 = "stimulationTime";  
const char* PARAM_7 = "movementAnalysisTime";
const char* PARAM_8 = "intervalTime";
const char* PARAM_9 = "numberOfEvents";

// ===== INDEX.HTML  =====
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Conditioning Chamber v0.2</title>
    <style>
        body{
            font-family: Arial, Helvetica, sans-serif;
        }

        h1{
            font-size: 25px;
            align-content: center;
            padding: 20px;
            background-color: #669bbc;
            color: white;
        }

        input[type=text], select, textarea, input[type=number]{
            width: 100%;
            padding: 12px;
            border: 1px solid #ccc;
            border-radius: 4px;
            box-sizing: border-box;
            resize: vertical;
        }
        
        label {
            padding: 12px 12px 12px 0;
            display: inline-block;
        }
        
        input[type=submit] {
            background-color: #c1121f;
            color: white;
            padding: 12px 20px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            float: right;
            margin-top: 30px;;
        }
        
        .container {
            border-radius: 5px;
            padding-left: 20px;
            padding-right: 20px;
        }
        
        .col-25 {
            float: left;
            width: 25%;
            margin-top: 6px;
        }
        
        .col-75 {
            float: left;
            width: 75%;
            margin-top: 6px;
        }
        
        .row:after {
            content: "";
            display: table;
            clear: both;
        }
        
        @media screen and (max-width: 600px) {
            .col-25, .col-75, input[type=submit]{
            width: 100%;
            margin-top: 0;
            }
        }

        @media screen and (max-width: 600px) {
            input[type=submit]{
            width: 60%;
            margin-top: 30px;
            }
        }
    </style>
</head>
<body>
    <div >
        <h1>Conditioning Chamber v0.2</h1>
    </div>
    <div class="container">
        <form action="/get">
            <div class="row">
            <div class="col-25">
                <label>Experiment Day</label>
            </div>
            <div class="col-75">
                <input type="number" name="dayOfExperiment" placeholder="Day (n)">
            </div>
            </div>
            <div class="row">
                <div class="col-25">
                <label>Rat ID</label>
                </div>
                <div class="col-75">
                    <input type="text" name="experimentAnimal" placeholder="Rat ID">
                </div>
            </div>
            <div class="row">
                <div class="col-25">
                <label>Exploration Duration</label>
                </div>
                <div class="col-75">
                    <input type="number" name="explorationTime" placeholder="Duration (s)">
                </div>
            </div>
            <div class="row">
                <div class="col-25">
                <label>Tone Frequency</label>
                </div>
                <div class="col-75">
                    <input type="number" name="toneFrequency" placeholder="Frequency (Hz)">
                </div>
            </div>
            <div class="row">
                <div class="col-25">
                <label>Tone Duration</label>
                </div>
                <div class="col-75">
                    <input type="number" name="toneTime" placeholder="Duration (s)">
                </div>
            </div>
            <div class="row">
                <div class="col-25">
                <label>Shock Duration</label>
                </div>
                <div class="col-75">
                    <input type="number" name="stimulationTime" placeholder="Duration (s)">
                </div>
            </div>
            <div class="row">
                <div class="col-25">
                <label>Movement Analysis</label>
                </div>
                <div class="col-75">
                    <input type="number" name="movementAnalysisTime" placeholder="Duration (s)">
                </div>
            </div>
            <div class="row">
                <div class="col-25">
                <label>Wait Interval</label>
                </div>
                <div class="col-75">
                    <input type="number" name="intervalTime" placeholder="Duration (s)">
                </div>
            </div>
            <div class="row">
                <div class="col-25">
                <label>Event Repetition</label>
                </div>
                <div class="col-75">
                    <input type="number" name="numberOfEvents" placeholder="Events (n)">
                </div>
            </div>
            <div>
                <input type="submit" value="Start Experiment">
            </div>
        </form>
        </div>
</body>
</html>
)rawliteral";

// ===== 404 =====
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

// ===== SETUP =====
void setup() {

  // Serial Communication
  Serial.begin(115200);
  Serial1.begin(115200);

  // Access Point
  WiFi.softAP(ssid, password);

  // Web page request
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Parameters Request
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1, inputMessage2, inputMessage3, inputMessage4, inputMessage5, inputMessage6, inputMessage7, inputMessage8, inputMessage9;
    // Day of Experiment
    if (request->getParam(PARAM_1)->value() != "") {
      inputMessage1 = request->getParam(PARAM_1)->value();
    } else {
      inputMessage1 = "0";
    }
    // Experiment Animal
    if (request->getParam(PARAM_2)->value() != "") {
      inputMessage2 = request->getParam(PARAM_2)->value();
    } else {
      inputMessage2 = "0";
    }
    // Exploration Duration
    if (request->getParam(PARAM_3)->value() != "") {
      inputMessage3 = request->getParam(PARAM_3)->value();
    } else {
      inputMessage3 = "0";
    }
    // Tone Frequency
    if (request->getParam(PARAM_4)->value() != "") {
      inputMessage4 = request->getParam(PARAM_4)->value();
    } else {
      inputMessage4 = "0";
    }
    // Tone Time
    if (request->getParam(PARAM_5)->value() != "") {
      inputMessage5 = request->getParam(PARAM_5)->value();
    } else {
      inputMessage5 = "0";
    }
    // Stimulation Time
    if (request->getParam(PARAM_6)->value() != "") {
      inputMessage6 = request->getParam(PARAM_6)->value();
    } else {
      inputMessage6 = "0";
    }
    // Movement Analysis Time
    if (request->getParam(PARAM_7)->value() != "") {
      inputMessage7 = request->getParam(PARAM_7)->value();
    } else {
      inputMessage7 = "0";
    }
    // Interval Time
    if (request->getParam(PARAM_8)->value() != "") {
      inputMessage8 = request->getParam(PARAM_8)->value();
    } else {
      inputMessage8 = "0";
    }
    // Number of Events
    if (request->getParam(PARAM_9)->value() != "") {
      inputMessage9 = request->getParam(PARAM_9)->value();
    } else {
      inputMessage9 = "0";
    }

    // Send parameters via UART
    Serial1.println(inputMessage1 + "," + inputMessage2 + "," + inputMessage3 + "," + inputMessage4 + "," + inputMessage5 + "," + inputMessage6 + "," + inputMessage7 + "," + inputMessage8 + "," + inputMessage9);
    
    request->send(200, "text/html", "Done"); 
});
  server.onNotFound(notFound);
  server.begin();
}
void loop() {
  
}