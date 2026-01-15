//libraries
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

//IP address
IPAddress local_IP(192, 168, 6, 1);
IPAddress gateway(192, 168, 6, 1);
IPAddress subnet(255, 255, 255, 0);

//WiFi credentials
const char* ssidpw = "8266HVSG";
const String bat_disch_icon = "<svg version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"49.79047\" height=\"25.63636\" viewBox=\"0,0,49.79047,25.63636\"><g transform=\"translate(-212.80044,-167.18182)\"><g fill=\"#00ff60\" stroke=\"#000000\" stroke-width=\"2\" stroke-miterlimit=\"10\"><path d=\"M218.40909,191.81818v-23.63636h43.18182v23.63636z\"/><path d=\"M213.80044,184.66216v-9.32432h4.45946v9.32432z\"/></g></g></svg>";
const String bat_chg_icon = "<svg version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"49.79047\" height=\"25.63636\" viewBox=\"0,0,49.79047,25.63636\"><g transform=\"translate(-212.80044,-167.18182)\"><g fill=\"#ffae00\" stroke=\"#000000\" stroke-miterlimit=\"10\"><path d=\"M218.40909,191.81818v-23.63636h43.18182v23.63636z\" stroke-width=\"2\" stroke-linecap=\"butt\"/><path d=\"M213.80044,184.66216v-9.32432h4.45946v9.32432z\" stroke-width=\"2\" stroke-linecap=\"butt\"/><path d=\"M237.48929,179.94205l2.97304,-6.41219l0.00756,5.35216l2.04082,0.64935l-3.16327,6.93878v-5.88126z\" stroke-width=\"1\" stroke-linecap=\"round\"/></g></g></svg>";
const String bat_full_icon = "<svg version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"49.79047\" height=\"25.63636\" viewBox=\"0,0,49.79047,25.63636\"><g transform=\"translate(-212.80043,-167.18182)\"><g stroke=\"#000000\" stroke-miterlimit=\"10\"><path d=\"M218.40909,191.81818v-23.63636h43.18182v23.63636z\" fill=\"#00ccff\" stroke-width=\"2\" stroke-linecap=\"butt\"/><path d=\"M213.80044,184.66216v-9.32432h4.45946v9.32432z\" fill=\"#00ccff\" stroke-width=\"2\" stroke-linecap=\"butt\"/><path d=\"M237.48929,179.94205l2.97304,-6.41219l0.00756,5.35216l2.04082,0.64935l-3.16327,6.93878v-5.88126z\" fill=\"#ffae00\" stroke-width=\"1\" stroke-linecap=\"round\"/></g></g></svg>";

//HTML
const String standard_UI_A = "<head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>ESP8266 HVSG</title><style>body{font-family:Arial,sans-serif;text-align:center;margin:50px;}input[type=\"range\"]{width:300px;}button{margin:10px;padding:10px 20px;}</style></head><body><h1>Current Limit</h1><input type=\"range\" id=\"slider\" min=\"0\" max=\"";
//Maximum PWM value here
const String standard_UI_B = "\" step=\"1\" value=\"";
//Current PWM value here
const String standard_UI_C = "\"><div class=\"slider-value\" id=\"sliderValue\">";
//Current PWM value here
const String standard_UI_D = "</div><div><button id=\"add1\">+1</button><button id=\"unlock_unsafe\">";
//Lock / Unlock button text (Unlock Current Limit(Advanced))
const String standard_UI_E = "</button><button id=\"subtract1\">-1</button></div><div><button id=\"mode1\" data-mode=\"1\">Mode 1</button><button id=\"mode2\" data-mode=\"2\">Mode 2</button><button id=\"mode3\" data-mode=\"3\">Mode 3</button></div><h1>Frequency Control</h1><input type=\"range\" id=\"frequencySlider\" min=\"50\" max=\"1000\" step=\"1\" value=\"";
//Freq
const String standard_UI_F = "\"><div class=\"frequency-value\" id=\"frequencyValue\">";
//Freq
const String standard_UI_G = "</div><div><button id=\"increaseFrequency\">+1</button><button id=\"decreaseFrequency\">-1</button></div><script>let lastSliderValue=parseFloat(document.getElementById('slider').value);let lastFrequencyValue = parseInt(document.getElementById('frequencySlider').value);let lastMode=null;let timeout=null;function sendRequest(climit,mode,frequency){const xhr=new XMLHttpRequest();const params=new URLSearchParams();if(climit!==undefined)params.append('climit',climit);if(mode!==null)params.append('mode',mode);if (frequency !== undefined) params.append('frequency', frequency);xhr.open('GET','http://192.168.6.1?'+params.toString(),true);xhr.send();}function handleRequest(climit,mode,frequency){clearTimeout(timeout);timeout=setTimeout(()=>{sendRequest(climit,mode,frequency);},50);}function updateSlider(value){lastSliderValue=value;document.getElementById('slider').value=lastSliderValue;document.getElementById('sliderValue').textContent=`${lastSliderValue}`;handleRequest(lastSliderValue,lastMode, lastFrequencyValue);}function updateFrequency(value) {lastFrequencyValue = value;document.getElementById('frequencySlider').value = lastFrequencyValue;document.getElementById('frequencyValue').textContent = `${lastFrequencyValue}`;handleRequest(lastSliderValue, lastMode, lastFrequencyValue);}document.getElementById('slider').addEventListener('input',function(){const currentSliderValue=parseFloat(this.value);if(currentSliderValue!==lastSliderValue){updateSlider(currentSliderValue);}});document.getElementById('frequencySlider').addEventListener('input', function() {const currentFrequencyValue = parseInt(this.value);if (currentFrequencyValue !== lastFrequencyValue) {updateFrequency(currentFrequencyValue);}});document.querySelectorAll('button[id^=\"mode\"]').forEach(button=>{button.addEventListener('click',function(){lastMode=this.getAttribute('data-mode');handleRequest(lastSliderValue,lastMode);});});document.getElementById('add1').addEventListener('click',()=>updateSlider(lastSliderValue+1));document.getElementById('subtract1').addEventListener('click',()=>updateSlider(lastSliderValue-1));document.getElementById('unlock_unsafe').addEventListener('click',function(){window.location.replace(\"http://192.168.6.1/?unlkcur=";
//Lock / Unlock current limit
const String standard_UI_H = "\");});document.getElementById('increaseFrequency').addEventListener('click', () => updateFrequency(lastFrequencyValue + 1));document.getElementById('decreaseFrequency').addEventListener('click', () => updateFrequency(lastFrequencyValue - 1));</script></body></html>";

//GPIO Pins
const int OnBoardLED = 2;
const int pwmPin = 13;       // IO13
const int hBridge1Pin = 5;   // IO5
const int hBridge2Pin = 4;   // IO4
const int chargingPin = 14;  // IO14
const int chargedPin = 12;   // IO12

//Default safety limits and hardware limits.
const int SafetyLimitPWM = 487;
const int MaximumLimitPWM = 1023;
const int MaxFreq = 1000;
const int MinFreq = 10;

//User settings
int CurrentLimitPWM = 0;
int CurrentLimitUnlocked = 0;
int CurrentMode = 1;
int IsUsingExtWiFi = 0;  //Not yet finished (Not working yet) >_<
int PulseFreq = 500;
int HBDutyCycle = 50;

//Check for client respond.
int RespondIsEmpty = 0;

//Counter for pulses
int HBMA = 0;

//Variable current and frequeny based on the user's setting
int VarPulseFreq = 0;
int VarCurrentPWM = 0;

//Counter for the LED
int LEDBlinkInterv = 0;

//Firmware information
String prod_name = "8266HVSG";
String prod_ver = "0.0.0.1-Test_Only";

//UI
ESP8266WebServer server(80);

//Bootup Logo
void bootuplogo() {
  Serial.println(F(",                                                                             "));
  Serial.println(F("                                                                            ,,, "));
  Serial.println(F("                                                                           ,,,, "));
  Serial.println(F("                                                                           ,,,, "));
  Serial.println(F("                                                                          ,,,,, "));
  Serial.println(F("                                                                         ,,,,,, "));
  Serial.println(F("                                                                        ,,,,,,,, "));
  Serial.println(F("                                                                       ,,,,,,,,, "));
  Serial.println(F("                                                                      .,,,,,,,,, "));
  Serial.println(F("                                                                      ,,,,,,,,,, "));
  Serial.println(F("                                                                     ,,,,,..,,,,. "));
  Serial.println(F("                                                                    ,,,,,...,,,,, "));
  Serial.println(F("                                                                   ,,,,,....,,,,, "));
  Serial.println(F("                                       .,.                           ,,,,,....**,,,              .,.                     ,,,.               .,,,. "));
  Serial.println(F("              */////////////      /////////////      //////////////   ,,,,*////////////      */////////////         ,//////////         *////////// "));
  Serial.println(F("              *///               ///*                ////       ,////,,,,////....,,,,///      /.        ////      ,///,               *///. "));
  Serial.println(F("              *///              .///                 ////        ////,,,,*///....,,,,///                *///     *///                //// "));
  Serial.println(F("              *///               */////              ////        ////,,,,..////*.,/////                 ///*     ///   //////,       ///  .//////. "));
  Serial.println(F("              */////////////        ////////.        ////,,,,///////,,,,.....///////*,                ////      ./////*    ,////    */////*    *//// "));
  Serial.println(F("              *///                       ./////      ///////////  ,,,,,...////....,/////            ////        ,///.        ////   ////         //// "));
  Serial.println(F("              *///                          ////     ////        ,,,,,...///......,,,*///        ////.           ///         .///    ///         ,/// "));
  Serial.println(F("              *///                          ////     ////       ,,,,,....///......,,,*///      ////              *///        ////    ////        //// "));
  Serial.println(F("              */////////////    //////////////*      ////      ,,,,,,...../////,,,*/////    ,////////////////      /////**/////,       /////**/////. "));
  Serial.println(F("              */////////////       *//////.          ////      ,,,,,.........,//////*,,     ,////////////////         .////*              ,////* "));
  Serial.println(F("                                                              ,,,,,................,,,,, "));
  Serial.println(F("                                                             ,,,,,.................,,,,, "));
  Serial.println(F("                                                            ,,,,,..................,,,,,,,,,,,,,,,,,,,, "));
  Serial.println(F("                                                           ,,,,,,,,,,,,,,,,,,,,....,,,,,,,,,,,,,,,,,,, "));
  Serial.println(F("                                                           ,,,,,,,,,,,,,,,,,,,,.................,,,,,. "));
  Serial.println(F("                                                                          .,,,,.................,,,,, "));
  Serial.println(F("                                ,///            ////    ////            ////,,,,/////////////..,,,,,  ,/////////////// "));
  Serial.println(F("                                ,///            ////     ////          ////,,,,////...........,,,,, ///// "));
  Serial.println(F("                                ,///            ////      ///,        //// ,,,,///,..........,,,,, //// "));
  Serial.println(F("                                ,///            ////       ///       *///  ,,,,*/////.......,,,,, //// "));
  Serial.println(F("                                ,///////////////////       ,///      ///   ,,,,,..////////..,,,,, ////       .////////, "));
  Serial.println(F("                                ,///            ////        ////    ///.    ,,,,.......,/////,,,  ////             ///, "));
  Serial.println(F("                                ,///            ////         ////  ///*     ,,,,,.........////,   ,///*            ///, "));
  Serial.println(F("                                ,///            ////          ///*////      ,,,,,........,////     *////           ///, "));
  Serial.println(F("                                ,///            ////           //////       ,,///////////////        *////////////////, "));
  Serial.println(F("                                ,///            ////            ////        ,,,,*////////,,,,            .////////*. "));
  Serial.println(F("                                                                            ,,,,,......,,,,, "));
  Serial.println(F("                                                                             ,,,,.....,,,,, "));
  Serial.println(F("                                                                             ,,,,,...,,,,, "));
  Serial.println(F("                                                                             ,,,,,..,,,,, "));
  Serial.println(F("                                                                             ,,,,,.,,,,,, "));
  Serial.println(F("                                                                             ,,,,,.,,,,, "));
  Serial.println(F("                                                                             ,,,,,,,,,, "));
  Serial.println(F("                                                                              ,,,,,,,, "));
  Serial.println(F("                                                                              ,,,,,,, "));
  Serial.println(F("                                                                              ,,,,,,. "));
  Serial.println(F("                                                                              ,,,,,, "));
  Serial.println(F("                                                                              ,,,,, "));
  Serial.println(F("                                                                              ,,,, "));
  Serial.println(F("                                                                               ,, "));
  Serial.println(F("                                                                               ,. "));
  Serial.println(F("                                                                               , "));
}

void setup() {

  //Setup console
  Serial.begin(115200);
  Serial.println("");

  //Print the firmware information
  bootuplogo();
  Serial.println("Welcome to ESP8266HVSG!");
  Serial.println("");
  Serial.print("Device : ");
  Serial.println(prod_name);
  Serial.print("Firmware Version ");
  Serial.println(prod_ver);

  //Setup AP.
  Serial.print("Setting AP...");
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(ssidpw, ssidpw);
  Serial.println("Done.");

  //Setup GPIO pins.
  Serial.print("Setting GPIO...");
  pinMode(OnBoardLED, OUTPUT);
  pinMode(hBridge1Pin, OUTPUT);
  pinMode(hBridge2Pin, OUTPUT);
  pinMode(pwmPin, OUTPUT);
  pinMode(chargingPin, INPUT);
  pinMode(chargedPin, INPUT);
  Serial.println("Done.");

  //Set PWM frequency to 30Khz for less voltage ripple.
  Serial.print("Setting PWM...");
  analogWriteFreq(30000);

  //Use 10bit PWM for smooth control.
  analogWriteRange(1023);
  Serial.println("Done.");

  // Start the web server.
  Serial.print("Starting HTTP server...");
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Done.");
}

//Arduino main loop.
void loop() {

  //Counter(s)
  HBMA++;

  //Check for new requests
  server.handleClient();

  //Directly update current limit if not using mode 3.
  if (!CurrentMode == 3) {
    analogWrite(pwmPin, CurrentLimitPWM);
  }

  //What to do if using mode 3.
  if (CurrentMode == 3) {
    if (CurrentLimitPWM - 50 < 0) {
      CurrentLimitPWM = 50;
    }
    if (VarPulseFreq <= 39) {
      pulse(PulseFreq - 39 + VarPulseFreq);
      VarPulseFreq++;
    } else if (HBMA <= 100) {
      VarCurrentPWM++;
      analogWrite(pwmPin, CurrentLimitPWM - 100 + VarCurrentPWM);
      pulse(PulseFreq - 39 + VarPulseFreq);
    } else {
      HBMA = 0;
      VarCurrentPWM = 0;
      VarPulseFreq = 0;
    }
  }

  //Blink the LED per 50 pulses.
  if (LEDBlinkInterv >= 25) {
    digitalWrite(OnBoardLED, 1);
    if (LEDBlinkInterv >= 50) {
      digitalWrite(OnBoardLED, 0);
      LEDBlinkInterv = 0;
    }
  }

  //What to do if using mode 1.
  if (CurrentMode == 1) {
    HBMA = 0;
    pulse(PulseFreq);
  }

  //What to do if using mode 2.
  if (CurrentMode == 2) {
    if (HBMA >= 0) {
      if (HBMA >= 100) {
        pulse(PulseFreq);
        if (HBMA >= 200) {
          HBMA = 0;
        }
      } else {
        WaitPulsePeriod(PulseFreq);
      }
    }
  }
}


//Function for sending a single pulse.
void pulse(int infreq) {

  //Add one to the led blinker counter.
  LEDBlinkInterv++;

  //Only accept if the frequency is higher than 50.
  if (infreq >= 50) {

    //Calculate total pulse width based on infreq.
    int totalPulseWidth = map(infreq, MinFreq, MaxFreq, 50000, 500);

    //Calculate pulse widths for ON and OFF times based on duty cycle
    int pulseWidthA = totalPulseWidth * (HBDutyCycle / 100.0);
    int pulseWidthB = totalPulseWidth - pulseWidthA;

    //Send pulse to H-Bridge 1.1
    digitalWrite(hBridge1Pin, HIGH);
    delayMicroseconds(pulseWidthA);
    digitalWrite(hBridge1Pin, LOW);
    delayMicroseconds(pulseWidthB);

    //Send pulse to H-Bridge 1.2
    digitalWrite(hBridge2Pin, HIGH);
    delayMicroseconds(pulseWidthA);
    digitalWrite(hBridge2Pin, LOW);
    delayMicroseconds(pulseWidthB);
  }
}

//Precise delay.
void WaitPulsePeriod(int wppus) {
  if (wppus >= 50) {
    int delmms = map(wppus, MinFreq, MaxFreq, 100000, 1000);
    delayMicroseconds(delmms);
  }
}

//Run this if any respond from the client is received.
void RespondNotEmpty() {
  RespondIsEmpty = 0;
}

//Run this if nothing is received.
void RespondEmpty() {
  RespondIsEmpty = 1;
}

//Process the commands.
void handleRoot() {
  digitalWrite(OnBoardLED, 1);
  //Assuming nothing being written to the UI because of error.
  String response = "Unknown Error";

  //Assuming the args is empty.
  RespondEmpty();

  //Respond from the client / UI.
  //Client requesting product name.
  if (server.hasArg("vf_app_prod_name")) {
    response = prod_name;
    RespondNotEmpty();
  }

  //Client requesting firmware version.
  if (server.hasArg("vf_app_prod_ver")) {
    response = prod_ver;
    RespondNotEmpty();
  }

  //Client requesting current limit locking state.
  if (server.hasArg("vf_app_cl_unlocked")) {
    response = CurrentLimitUnlocked;
    RespondNotEmpty();
  }

  //Client requesting current limit locking state.
  if (server.hasArg("vf_app_bat")) {
    if (digitalRead(chargingPin)) {
      response = "BAT_CHG";
    } else if (digitalRead(chargedPin)) {
      response = "BAT_FULL";
    } else {
      response = "BAT_DSCH";
    }
    RespondNotEmpty();
  }

  //Set current limit.
  if (server.hasArg("climit")) {
    String clvs = server.arg("climit");
    CurrentLimitPWM = clvs.toInt();
    RespondNotEmpty();
    //Skipping UI for faster processing.
    Serial.print("Set Current to:");
    Serial.println(CurrentLimitPWM);
    response = "P";
  }

  //Set H-Bridge control mode
  if (server.hasArg("mode")) {
    String mode = server.arg("mode");
    CurrentMode = mode.toInt();
    RespondNotEmpty();
    response = "P";
    Serial.print("Set Current mode to:");
    Serial.println(CurrentMode);
  }

  if (server.hasArg("unlkcur")) {
    String cllm = server.arg("unlkcur");
    CurrentLimitUnlocked = cllm.toInt();
    if (!CurrentLimitUnlocked) {
      if (CurrentLimitPWM >= SafetyLimitPWM) {
        CurrentLimitPWM = SafetyLimitPWM - 50;
      }
    }
    RespondNotEmpty();
    response = "<script>window.location.replace(\"http://192.168.6.1\");</script>";
  }

  if (server.hasArg("frequency")) {
    String cufrq = server.arg("frequency");
    PulseFreq = cufrq.toInt();
    RespondNotEmpty();
    response = "P";
    Serial.print("Set pulse frequency to:");
    Serial.println(PulseFreq);
  }

  //Sending the main UI to the client due to empty respond.
  if (RespondIsEmpty) {
    Serial.print("Respond is empty , compiling UI...");
    //Show battery icon./Check battery state.
    if (digitalRead(chargingPin)) {
      response = bat_chg_icon;
    } else if (digitalRead(chargedPin)) {
      response = bat_full_icon;
    } else {
      response = bat_disch_icon;
    }
    response += standard_UI_A;
    if (!CurrentLimitUnlocked) {
      response += SafetyLimitPWM;
    } else {
      response += MaximumLimitPWM;
    }
    response += standard_UI_B;
    response += CurrentLimitPWM;
    response += standard_UI_C;
    response += CurrentLimitPWM;
    response += standard_UI_D;
    if (!CurrentLimitUnlocked) {
      response += "Unlock Current Limit(Advanced)";
    } else {
      response += "Relock Current Limit";
    }
    response += standard_UI_E;
    response += PulseFreq;
    response += standard_UI_F;
    response += PulseFreq;
    response += standard_UI_G;
    if (!CurrentLimitUnlocked) {
      response += "1";
    } else {
      response += "0";
    }
    response += standard_UI_H;
    response += "<p>";
    response += prod_name;
    response += " Version ";
    response += prod_ver;
    response += "</p>";
    Serial.println("Done.");
  }

  //Finish the http request.
  Serial.print("Send HTML to client...");
  server.send(200, "text/html", response);
  Serial.println("Done.");
}