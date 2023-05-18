/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-web-server-outputs-momentary-switch/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/


#include <FastLED.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>

#include <ESPAsyncWebServer.h>

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "motor";
const char* password = "12345678";

Servo myservo;
boolean noisego = false;
int noiseProcess = 0;
int noisespeed = 5;
const int output = D1;
int pwm = 90;
// HTML web page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
  <head>
    <title>ESP Pushbutton Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 30px;}
      .button {
        padding: 10px 20px;
        font-size: 24px;
        text-align: center;
        outline: none;
        color: #fff;
        background-color: #2f4468;
        border: none;
        border-radius: 5px;
        box-shadow: 0 6px #999;
        cursor: pointer;
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        -webkit-tap-highlight-color: rgba(0,0,0,0);
      }  
      .button:hover {background-color: #1f2e45}
      .button:active {
        background-color: #1f2e45;
        box-shadow: 0 4px #666;
        transform: translateY(2px);
      }
    </style>
  </head>
  <body>
    <h1>ESP Pushbutton Web Server</h1>
   
    <button class="button"  ontouchstart="toggleCheckbox('on');" onmouseup="toggleCheckbox('off');" ontouchend="toggleCheckbox('off');">LED PUSHBUTTON</button>
    <button class="button"  ontouchstart="stopMotor('stop');">STOP</button>   
   <script>
   
   function toggleCheckbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
   }

      function stopMotor(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
   }
  </script>
  </body>
</html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

AsyncWebServer server(80);

void setup() {
  myservo.attach(D7);
  myservo.write(90);
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());
  pinMode(D7, OUTPUT);
  //WiFi.begin(ssid, password);


  //  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
  //    Serial.println("WiFi Failed!");
  //    return;
  //  }
  //  Serial.println();
  //  Serial.print("ESP IP Address: http://");
  //  Serial.println(WiFi.localIP());

  pinMode(output, OUTPUT);
  //digitalWrite(D7);
  //analogWrite(D7, 127);
  // Send web page to client

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html);
  });

  // Receive an HTTP GET request
  server.on("/on", HTTP_GET, [] (AsyncWebServerRequest * request) {
    digitalWrite(output, HIGH);
    //Serial.println("HIGH");
    //pwm = random(0, 180);
    noisego = true;
    Serial.println(pwm);
    request->send(200, "text/plain", "ok");
  });

  server.on("/stop", HTTP_GET, [] (AsyncWebServerRequest * request) {
    noisego = false;
    pwm = 90;
    Serial.println(pwm);
    request->send(200, "text/plain", "ok");
  });

  // Receive an HTTP GET request
  server.on("/off", HTTP_GET, [] (AsyncWebServerRequest * request) {
    digitalWrite(output, LOW);
    //Serial.println("LOW");
    request->send(200, "text/plain", "ok");
  });

  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  //analogWrite(D7, pwm);
  int n = inoise8(noiseProcess);
  
  noiseProcess+=noisespeed;
  int num = wifi_softap_get_station_num();
  if(num == 0){
    noisego = false;
  }
  Serial.println(num);
  int speeds = map(n, 0, 255, 15, 165);
  if (noisego) {
    myservo.write(speeds);
  } else {
    myservo.write(90);
  }

}
//<button class="button" onmousedown="toggleCheckbox('on');" ontouchstart="toggleCheckbox('on');" onmouseup="toggleCheckbox('off');" ontouchend="toggleCheckbox('off');">LED PUSHBUTTON</button>
