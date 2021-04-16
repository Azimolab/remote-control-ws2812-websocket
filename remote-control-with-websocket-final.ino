/**
 * ----------------------------------------------------------------------------
 * ESP32 Remote Control with WebSocket
 * ----------------------------------------------------------------------------
 * © 2020 Stéphane Calderoni
 * ----------------------------------------------------------------------------
 */

#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

#define DATA_PIN1 27
#define DATA_PIN2 25
//#define DATA_PIN3 12
#define NUM_LEDS1 629
#define NUM_LEDS2 683
//#define NUM_LEDS2 564
//#define NUM_LEDS3 130
//#define NUM_LEDS NUM_LEDS1 + NUM_LEDS2 + NUM_LEDS3
#define NUM_LEDS NUM_LEDS1 + NUM_LEDS2
CRGB leds1[NUM_LEDS];

//unidades Townhouse
int posicao_ini_leds1[] = {1193};
int posicao_fin_leds1[] = {1226};
int quantidade_seguimentos_1 = 1;

//unidades SuperDuplex
int posicao_ini_leds2[] = {23, 534};
int posicao_fin_leds2[] = {34, 545};
int quantidade_seguimentos_2 = 2;

//unidades Uira                                                                              //16                                                                                            35                                                                         50                  54                                                                                            73                                                        84                                 90
int posicao_ini_leds3[] = {4, 8, 12, 17, 21, 69, 96, 184, 190, 195, 203, 244, 255, 264, 268, 274, 282, 287, 291, 295, 319, 323, 328, 332, 343, 347, 352, 356, 389, 397, 402, 406, 410, 443, 446, 450, 454, 465, 474, 478, 482, 496, 581, 602, 608, 613, 621, 625, 635, 639, 648, 652, 688, 694, 703, 708, 712, 718, 722, 727, 731, 740, 750, 762, 767, 771, 776, 780, 786, 790, 795, 799, 803, 812, 826, 832, 847, 856, 866, 874, 889, 903, 1008, 1039, 1043, 1048, 1052, 1056, 1081, 1149};
int posicao_fin_leds3[] = {5, 9, 14, 18, 22, 72, 183, 187, 191, 200, 211, 252, 260, 265, 271, 279, 283, 288, 292, 316, 320, 324, 329, 338, 344, 348, 353, 362, 394, 399, 403, 407, 436, 443, 447, 451, 460, 471, 475, 479, 483, 499, 584, 605, 609, 618, 622, 632, 636, 644, 649, 683, 691, 700, 704, 709, 713, 719, 723, 728, 737, 745, 755, 764, 768, 772, 777, 781, 787, 791, 796, 800, 805, 817, 829, 844, 853, 863, 871, 885, 894, 990, 1011, 1040, 1044, 1049, 1053, 1057, 1110, 1171};
int quantidade_seguimentos_3 = 90;

//unidades Ybira                                                                               16                                                                                       34                                                                              50                                                                                             69                                                                              85                                 91
int posicao_ini_leds4[] = {0, 6, 10, 15, 19, 35, 73, 188, 192, 201, 212, 253, 261, 266, 272, 280, 284, 289, 293, 317, 321, 325, 330, 339, 345, 349, 354, 363, 395, 400, 404, 408, 437, 444, 448, 452, 461, 472, 476, 480, 484, 500, 546, 552, 585, 606, 610, 619, 623, 633, 637, 645, 650, 684, 692, 701, 705, 710, 714, 720, 724, 729, 738, 746, 756, 765, 769, 773, 778, 782, 788, 792, 797, 801, 806, 818, 830, 845, 854, 864, 872, 886, 895, 991, 1012, 1041, 1045, 1050, 1054, 1058, 1172};
int posicao_fin_leds4[] = {3, 7, 11, 16, 20, 68, 95, 189, 194, 202, 243, 254, 263, 267, 273, 281, 286, 290, 294, 318, 322, 327, 331, 342, 346, 351, 355, 388, 396, 401, 405, 409, 442, 445, 449, 453, 464, 473, 477, 481, 495, 533, 551, 580, 601, 607, 612, 620, 624, 634, 638, 647, 651, 687, 693, 702, 707, 711, 717, 721, 726, 730, 739, 749, 761, 766, 770, 775, 779, 785, 789, 794, 798, 802, 811, 825, 831, 846, 855, 865, 873, 888, 902, 1007, 1038, 1042, 1047, 1051, 1055, 1080, 1177};
int quantidade_seguimentos_4 = 91;

//Area de Lazer
//1111,
//1148,

int posicao_ini_leds5[] = {1178, 1111, 1247, 1293};
int posicao_fin_leds5[] = {1192, 1148, 1292, 1311};
int quantidade_seguimentos_5 = 3;

// Lojas
int posicao_ini_leds6[] = {1227};
int posicao_fin_leds6[] = {1246};
int quantidade_seguimentos_6 = 1;

// ----------------------------------------------------------------------------
// Definition of macros
// ----------------------------------------------------------------------------

#define LED_PIN 10
#define BTN_PIN 22
#define HTTP_PORT 80

// ----------------------------------------------------------------------------
// Definition of global constants
// ----------------------------------------------------------------------------

// Button debouncing
const uint8_t DEBOUNCE_DELAY = 10; // in milliseconds

// WiFi credentials
const char *WIFI_SSID = "Azimolab";
const char *WIFI_PASS = "zealotsgrill";

// ----------------------------------------------------------------------------
// Definition of the LED component
// ----------------------------------------------------------------------------

struct Led
{
  // state variables
  uint8_t pin;
  bool on;

  // methods
  void update()
  {

    //digitalWrite(pin, on ? HIGH : LOW);
  }
};

// ----------------------------------------------------------------------------
// Definition of the Button component
// ----------------------------------------------------------------------------

struct Button
{
  // state variables
  uint8_t pin;
  bool lastReading;
  uint32_t lastDebounceTime;
  uint16_t state;

  // methods determining the logical state of the button
  bool pressed() { return state == 1; }
  bool released() { return state == 0xffff; }
  bool held(uint16_t count = 0) { return state > 1 + count && state < 0xffff; }

  // method for reading the physical state of the button
  void read()
  {
    // reads the voltage on the pin connected to the button
    bool reading = digitalRead(pin);

    // if the logic level has changed since the last reading,
    // we reset the timer which counts down the necessary time
    // beyond which we can consider that the bouncing effect
    // has passed.
    if (reading != lastReading)
    {
      lastDebounceTime = millis();
    }

    // from the moment we're out of the bouncing phase
    // the actual status of the button can be determined
    if (millis() - lastDebounceTime > DEBOUNCE_DELAY)
    {
      // don't forget that the read pin is pulled-up
      bool pressed = reading == LOW;
      if (pressed)
      {
        if (state < 0xfffe)
          state++;
        else if (state == 0xfffe)
          state = 2;
      }
      else if (state)
      {
        state = state == 0xffff ? 0 : 0xffff;
      }
    }

    // finally, each new reading is saved
    lastReading = reading;
  }
};

// ----------------------------------------------------------------------------
// Definition of global variables
// ----------------------------------------------------------------------------

Led onboard_led = {LED_BUILTIN, false};
Led led = {LED_PIN, false};
Led led1 = {LED_PIN, false};
Led led2 = {LED_PIN, false};
Led led3 = {LED_PIN, false};
Led led4 = {LED_PIN, false};
Led led5 = {LED_PIN, false};
Button button = {BTN_PIN, HIGH, 0, 0};

AsyncWebServer server(HTTP_PORT);
AsyncWebSocket ws("/ws");

// ----------------------------------------------------------------------------
// SPIFFS initialization
// ----------------------------------------------------------------------------

void initSPIFFS()
{
  if (!SPIFFS.begin())
  {
    Serial.println("Cannot mount SPIFFS volume...");
    while (1)
    {
      onboard_led.on = millis() % 200 < 50;
      onboard_led.update();
    }
  }
}

// ----------------------------------------------------------------------------
// Connecting to the WiFi network
// ----------------------------------------------------------------------------

void initWiFi()
{
  // WiFi.mode(WIFI_STA);
  // WiFi.begin(WIFI_SSID, WIFI_PASS);
  // Serial.printf("Trying to connect [%s] ", WiFi.macAddress().c_str());
  // while (WiFi.status() != WL_CONNECTED) {
  //     Serial.print(".");
  //     delay(500);
  // }
  // Serial.printf(" %s\n", WiFi.localIP().toString().c_str());

  WiFi.softAP("Maquete-Porte", "");
  Serial.println("softap");
  Serial.println("");
  Serial.println(WiFi.softAPIP());
  Serial.println(WiFi.localIP());
}

// ----------------------------------------------------------------------------
// Web server initialization
// ----------------------------------------------------------------------------

String processor(const String &var)
{
  return String(var == "STATE" && led.on ? "on" : "off");
  return String(var == "STATE1" && led1.on ? "on" : "off");
  return String(var == "STATE2" && led2.on ? "on" : "off");
  return String(var == "STATE3" && led3.on ? "on" : "off");
  return String(var == "STATE4" && led4.on ? "on" : "off");
  return String(var == "STATE5" && led5.on ? "on" : "off");
}

void onRootRequest(AsyncWebServerRequest *request)
{
  request->send(SPIFFS, "/index.html", "text/html", false, processor);
}

void initWebServer()
{
  server.on("/", onRootRequest);
  server.serveStatic("/", SPIFFS, "/");

  server.on("/logo", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/logo.png", "image/png");
  });
  server.begin();
}

// ----------------------------------------------------------------------------
// WebSocket initialization
// ----------------------------------------------------------------------------

void notifyClients()
{
  const uint8_t size = JSON_OBJECT_SIZE(1);
  StaticJsonDocument<size> json;
  // json["status"] = led.on ? "on" : "off";

  ws.printfAll("{\"rank\":\"%u\",\"status\":\"%s\"}", 1, led.on ? "on" : "off");
  ws.printfAll("{\"rank\":\"%u\",\"status\":\"%s\"}", 2, led1.on ? "on" : "off");
  ws.printfAll("{\"rank\":\"%u\",\"status\":\"%s\"}", 3, led2.on ? "on" : "off");
  ws.printfAll("{\"rank\":\"%u\",\"status\":\"%s\"}", 4, led3.on ? "on" : "off");
  ws.printfAll("{\"rank\":\"%u\",\"status\":\"%s\"}", 5, led4.on ? "on" : "off");
  ws.printfAll("{\"rank\":\"%u\",\"status\":\"%s\"}", 6, led5.on ? "on" : "off");

  char buffer[80];
  size_t len = serializeJson(json, buffer);
  ws.textAll(buffer, len);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {

    const uint8_t size = JSON_OBJECT_SIZE(1);
    StaticJsonDocument<size> json;
    DeserializationError err = deserializeJson(json, data);
    if (err)
    {
      Serial.print(F("deserializeJson() failed with code "));
      Serial.println(err.c_str());
      return;
    }

    const char *action = json["action"];
    if (strcmp(action, "toggle1") == 0)
    {
      led.on = !led.on;
      notifyClients();
    }
    if (strcmp(action, "toggle2") == 0)
    {
      led1.on = !led1.on;
      notifyClients();
    }
    if (strcmp(action, "toggle3") == 0)
    {
      led2.on = !led2.on;
      notifyClients();
    }
    if (strcmp(action, "toggle4") == 0)
    {
      led3.on = !led3.on;
      notifyClients();
    }

    if (strcmp(action, "toggle5") == 0)
    {
      led4.on = !led4.on;
      notifyClients();
    }

    if (strcmp(action, "toggle6") == 0)
    {
      led5.on = !led5.on;
      notifyClients();
    }
  }
}

void onEvent(AsyncWebSocket *server,
             AsyncWebSocketClient *client,
             AwsEventType type,
             void *arg,
             uint8_t *data,
             size_t len)
{

  switch (type)
  {
  case WS_EVT_CONNECT:
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    break;
  case WS_EVT_DISCONNECT:
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
    break;
  case WS_EVT_DATA:
    handleWebSocketMessage(arg, data, len);
    break;
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
}

void initWebSocket()
{
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

// ----------------------------------------------------------------------------
// Initialization
// ----------------------------------------------------------------------------

void setup()
{

  FastLED.addLeds<WS2812B, DATA_PIN1>(leds1, 0, NUM_LEDS1);
  FastLED.addLeds<WS2812B, DATA_PIN2>(leds1, NUM_LEDS1, NUM_LEDS2);
  //FastLED.addLeds<WS2812B, DATA_PIN3>(leds1, 1193, NUM_LEDS3);
  // FastLED.addLeds<WS2812B, DATA_PIN1>(leds1, NUM_LEDS1);
  //FastLED.addLeds<WS2812B, DATA_PIN2>(leds2, NUM_LEDS2);
  FastLED.setBrightness(255);
  //FastLED.clear();

  pinMode(onboard_led.pin, OUTPUT);
  pinMode(led.pin, OUTPUT);
  pinMode(button.pin, INPUT);

  Serial.begin(115200);
  delay(500);

  initSPIFFS();
  initWiFi();
  initWebSocket();
  initWebServer();
}

// ----------------------------------------------------------------------------
// Main control loop
// ----------------------------------------------------------------------------

void loop()
{
  ws.cleanupClients();

  button.read();

  if (button.pressed())
  {
    led.on = !led.on;
    notifyClients();
  }

  onboard_led.on = millis() % 1000 < 50;

  //led.update();
  rotina_fitas1();
  rotina_fitas2();
  rotina_fitas3();
  rotina_fitas4();
  rotina_fitas5();
  rotina_fitas6();

  onboard_led.update();
}

void rotina_fitas1()
{
  if (led.on == 1)
  {
    for (int h = 0; h < quantidade_seguimentos_1; h++)
    {
      for (int i = posicao_ini_leds1[h]; i <= posicao_fin_leds1[h]; i++)
      {
        leds1[i] = CRGB::White;
      }
    }
    FastLED.show();
  }
  else
  {
    for (int h = 0; h < quantidade_seguimentos_1; h++)
    {
      for (int i = posicao_ini_leds1[h]; i <= posicao_fin_leds1[h]; i++)
      {
        leds1[i] = CRGB::Black;
      }
    }
    FastLED.show();
  }
}

void rotina_fitas2()
{

  if (led1.on == 1)
  {
    for (int h = 0; h < quantidade_seguimentos_2; h++)
    {
      for (int i = posicao_ini_leds2[h]; i <= posicao_fin_leds2[h]; i++)
      {
        leds1[i] = CRGB::White;
      }
    }
    FastLED.show();
  }
  else
  {
    for (int h = 0; h < quantidade_seguimentos_2; h++)
    {
      for (int i = posicao_ini_leds2[h]; i <= posicao_fin_leds2[h]; i++)
      {
        leds1[i] = CRGB::Black;
      }
    }
    FastLED.show();
  }
}

void rotina_fitas3()
{

  if (led2.on == 1)
  {
    for (int h = 0; h < quantidade_seguimentos_3; h++)
    {
      for (int i = posicao_ini_leds3[h]; i <= posicao_fin_leds3[h]; i++)
      {
        leds1[i] = CRGB::White;
      }
    }
    FastLED.show();
    delay(30);
  }
  else
  {
    for (int h = 0; h < quantidade_seguimentos_3; h++)
    {
      for (int i = posicao_ini_leds3[h]; i <= posicao_fin_leds3[h]; i++)
      {
        leds1[i] = CRGB::Black;
      }
    }
    FastLED.show();
    delay(30);
  }
}

void rotina_fitas4()
{

  if (led3.on == 1)
  {
    for (int h = 0; h < quantidade_seguimentos_4; h++)
    {
      for (int i = posicao_ini_leds4[h]; i <= posicao_fin_leds4[h]; i++)
      {
        leds1[i] = CRGB::White;
      }
    }
    FastLED.show();
  }
  else
  {

    for (int h = 0; h < quantidade_seguimentos_4; h++)
    {
      for (int i = posicao_ini_leds4[h]; i <= posicao_fin_leds4[h]; i++)
      {
        leds1[i] = CRGB::Black;
      }
    }
    FastLED.show();
  }
}

void rotina_fitas5()
{

  if (led4.on == 1)
  {
    for (int h = 0; h < 3; h++)
    {
      for (int i = posicao_ini_leds5[h]; i <= posicao_fin_leds5[h]; i++)
      {
        leds1[i] = CRGB::White;
      }
    }

    for (int i = posicao_ini_leds5[3]; i <= posicao_fin_leds5[3]; i++)
    {
      //leds1[i] = CRGB::FairyLight;
      leds1[i] = CRGB::White;
    }

    FastLED.show();
  }

  else
  {
    
    
      for (int i = posicao_ini_leds5[0]; i <= posicao_fin_leds5[4]; i++)
      {
        leds1[i] = CRGB::Black;
      }
    
    FastLED.show();
  }
}

void rotina_fitas6()
{
  if (led5.on == 1)
  {
    for (int h = 0; h < quantidade_seguimentos_6; h++)
    {
      for (int i = posicao_ini_leds6[h]; i <= posicao_fin_leds6[h]; i++)
      {
        leds1[i] = CRGB::White;
      }
    }
    FastLED.show();
  }
  else
  {
    for (int h = 0; h < quantidade_seguimentos_6; h++)
    {
      for (int i = posicao_ini_leds6[h]; i <= posicao_fin_leds6[h]; i++)
      {
        leds1[i] = CRGB::Black;
      }
    }
    FastLED.show();
  }
}
