/**
 * ----------------------------------------------------------------------------
 * ESP32 Remote Control with WebSocket
 * ----------------------------------------------------------------------------
 * © 2020 Stéphane Calderoni
 * ----------------------------------------------------------------------------
 */

#include <Preferences.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#define FASTLED_ALLOW_INTERRUPTS 0
//#define FASTLED_INTERRUPT_RETRY_COUNT 1
#include <FastLED.h>

#define DATA_PIN1 27
#define DATA_PIN2 12
#define DATA_PIN3 26
//#define NUM_LEDS1 629
#define NUM_LEDS1 628

//#define NUM_LEDS2 683
#define NUM_LEDS2 564
#define NUM_LEDS3 118

#define NUM_LEDS NUM_LEDS1 + NUM_LEDS2 + NUM_LEDS3
#define NUM_LEDS12 NUM_LEDS1 + NUM_LEDS2
//#define NUM_LEDS NUM_LEDS1 + NUM_LEDS2

CRGB leds1[NUM_LEDS];
//CRGB leds2[NUM_LEDS3];

//Preferences preferences;

//unidades Townhouse
int posicao_ini_leds1[] = {1193};
int posicao_fin_leds1[] = {1226};
int quantidade_seguimentos_1 = 1;

//unidades SuperDuplex
int posicao_ini_leds2[] = {23, 534};
int posicao_fin_leds2[] = {34, 545};
int quantidade_seguimentos_2 = 2;

//unidades Uira                                                                              //16                                                                                            35                                                                         50                  54                                                                                            73                                                        84                                 90
const int posicao_ini_leds3[] PROGMEM = {4, 8, 12, 17, 21, 69, 96, 184, 190, 195, 203, 244, 255, 264, 268, 274, 282, 287, 291, 295, 319, 323, 328, 332, 343, 347, 352, 356, 389, 397, 402, 406, 410, 443, 446, 450, 454, 465, 474, 478, 482, 496, 581, 602, 608, 613, 621, 625, 635, 639, 648, 652, 688, 694, 703, 708, 712, 718, 722, 727, 731, 740, 750, 762, 767, 771, 776, 780, 786, 790, 795, 799, 803, 812, 826, 832, 847, 856, 866, 874, 889, 903, 1008, 1039, 1043, 1048, 1052, 1056, 1081, 1149};
const int posicao_fin_leds3[] PROGMEM = {5, 9, 14, 18, 22, 72, 183, 187, 191, 200, 211, 252, 260, 265, 271, 279, 283, 288, 292, 316, 320, 324, 329, 338, 344, 348, 353, 362, 394, 399, 403, 407, 436, 443, 447, 451, 460, 471, 475, 479, 483, 499, 584, 605, 609, 618, 622, 632, 636, 644, 649, 683, 691, 700, 704, 709, 713, 719, 723, 728, 737, 745, 755, 764, 768, 772, 777, 781, 787, 791, 796, 800, 805, 817, 829, 844, 853, 863, 871, 885, 894, 990, 1011, 1040, 1044, 1049, 1053, 1057, 1110, 1171};
int quantidade_seguimentos_3 = 90;

//unidades Ybira                                                                               16                                                                                       34                                                                              50                                                                                             69                                                                              85                                 91
const int posicao_ini_leds4[] PROGMEM = {0, 6, 10, 15, 19, 35, 73, 188, 192, 201, 212, 253, 261, 266, 272, 280, 284, 289, 293, 317, 321, 325, 330, 339, 345, 349, 354, 363, 395, 400, 404, 408, 437, 444, 448, 452, 461, 472, 476, 480, 484, 500, 546, 552, 585, 606, 610, 619, 623, 633, 637, 645, 650, 684, 692, 701, 705, 710, 714, 720, 724, 729, 738, 746, 756, 765, 769, 773, 778, 782, 788, 792, 797, 801, 806, 818, 830, 845, 854, 864, 872, 886, 895, 991, 1012, 1041, 1045, 1050, 1054, 1058, 1172};
const int posicao_fin_leds4[] PROGMEM = {3, 7, 11, 16, 20, 68, 95, 189, 194, 202, 243, 254, 263, 267, 273, 281, 286, 290, 294, 318, 322, 327, 331, 342, 346, 351, 355, 388, 396, 401, 405, 409, 442, 445, 449, 453, 464, 473, 477, 481, 495, 533, 551, 580, 601, 607, 612, 620, 624, 634, 638, 647, 651, 687, 693, 702, 707, 711, 717, 721, 726, 730, 739, 749, 761, 766, 770, 775, 779, 785, 789, 794, 798, 802, 811, 825, 831, 846, 855, 865, 873, 888, 902, 1007, 1038, 1042, 1047, 1051, 1055, 1080, 1177};
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
#define HTTP_PORT 80

// ----------------------------------------------------------------------------
// Definition of global constants
// ----------------------------------------------------------------------------

// Button debouncing
const uint8_t DEBOUNCE_DELAY = 10; // in milliseconds

// WiFi credentials
//const char *WIFI_SSID = "Azimolab";
//const char *WIFI_PASS = "zealotsgrill";

// ----------------------------------------------------------------------------
// Definition of the LED component
// ----------------------------------------------------------------------------

struct Led
{ // state variables
  bool on;
};

// ----------------------------------------------------------------------------
// Definition of the Button component
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Definition of global variables
// ----------------------------------------------------------------------------

//Led onboard_led = {LED_BUILTIN, false};
Led led = {false};
Led led1 = {false};
Led led2 = {false};
Led led3 = {false};
Led led4 = {false};
Led led5 = {false};
//Button button = {BTN_PIN, HIGH, 0, 0};

AsyncWebServer server(HTTP_PORT);
AsyncWebSocket ws("/ws");

TaskHandle_t Task1;
TaskHandle_t Task2;

// ----------------------------------------------------------------------------
// SPIFFS initialization
// ----------------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);
  delay(500);

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
      Task1code, /* Task function. */
      "Task1",   /* name of task. */
      10000,     /* Stack size of task */
      NULL,      /* parameter of the task */
      1,         /* priority of the task */
      &Task1,    /* Task handle to keep track of created task */
      0);        /* pin task to core 0 */
  delay(500);

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
      Task2code, /* Task function. */
      "Task2",   /* name of task. */
      10000,     /* Stack size of task */
      NULL,      /* parameter of the task */
      1,         /* priority of the task */
      &Task2,    /* Task handle to keep track of created task */
      1);        /* pin task to core 1 */
  delay(500);

  // preferences.begin("btn1", false);
  // preferences.begin("btn2", false);
  // preferences.begin("btn3", false);
  //  preferences.begin("btn4", false);
  // preferences.begin("btn5", false);
  // preferences.begin("btn6", false);

  // led.on = preferences.getBool("state1", false);
  //led1.on = preferences.getBool("state2", false);
  // led2.on = preferences.getBool("state3", false);
  // led3.on = preferences.getBool("state4", false);
  // led4.on = preferences.getBool("state5", false);
  // led5.on = preferences.getBool("state6", false);

  //  Serial.printf("LED state before reset: %d \n", led.on);
  // Serial.printf("LED state before reset: %d \n", led1.on);
  //  Serial.printf("LED state before reset: %d \n", led2.on);
  //  Serial.printf("LED state before reset: %d \n", led3.on);
  //  Serial.printf("LED state before reset: %d \n", led4.on);
  // Serial.printf("LED state before reset: %d \n", led5.on);
}

// ----------------------------------------------------------------------------
// Main control loop
// ----------------------------------------------------------------------------
void Task1code(void *pvParameters)
{
  initSPIFFS();
  initWiFi();
  initWebSocket();
  initWebServer();

  for (;;)
  {
     Serial.print("Task1 running on core ");
     //Serial.println(xPortGetCoreID());
     vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}


void Task2code(void *pvParameters)
{
  FastLED.addLeds<WS2812B, DATA_PIN1, GRB>(leds1, 0, NUM_LEDS1);
  FastLED.addLeds<WS2812B, DATA_PIN2, GRB>(leds1, NUM_LEDS1, NUM_LEDS2);
  FastLED.addLeds<WS2812B, DATA_PIN3, GRB>(leds1, NUM_LEDS12, NUM_LEDS3);

  FastLED.setBrightness(255);
  FastLED.clear();

  for (;;)
  {
    Serial.print("Task2 running on core ");
    Serial.println(xPortGetCoreID());
    //Serial.println(xPortGetFreeHeapSize());
    //Serial.println(ESP.getFreeHeap());
    rotina_fitas1();
    rotina_fitas2();
    rotina_fitas3();
    rotina_fitas4();
    rotina_fitas5();
    rotina_fitas6();

    // estado_botoes();
  }
}

void loop()
{

}

void initSPIFFS()
{
  if (!SPIFFS.begin())
  {
    Serial.println("Cannot mount SPIFFS volume...");
   
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

  WiFi.softAP("Maquete-Artem", "");
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
  //return String(var == "STATE" && led.on ? "on" : "off");
  //return String(var == "STATE1" && led1.on ? "on" : "off");
  //return String(var == "STATE2" && led2.on ? "on" : "off");
  //return String(var == "STATE3" && led3.on ? "on" : "off");
  //return String(var == "STATE4" && led4.on ? "on" : "off");
  //return String(var == "STATE5" && led5.on ? "on" : "off");
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
  const uint8_t size = JSON_OBJECT_SIZE(6);
  StaticJsonDocument<size> json;
  json["status"] = led.on ? "on" : "off";
  json["status"] = led1.on ? "on" : "off";
  json["status"] = led2.on ? "on" : "off";
  json["status"] = led3.on ? "on" : "off";
  json["status"] = led4.on ? "on" : "off";
  json["status"] = led5.on ? "on" : "off";

  ws.printfAll("{\"rank\":\"%u\",\"status\":\"%s\"}", 1, led.on ? "on" : "off");
  ws.printfAll("{\"rank\":\"%u\",\"status\":\"%s\"}", 2, led1.on ? "on" : "off");
  ws.printfAll("{\"rank\":\"%u\",\"status\":\"%s\"}", 3, led2.on ? "on" : "off");
  ws.printfAll("{\"rank\":\"%u\",\"status\":\"%s\"}", 4, led3.on ? "on" : "off");
  ws.printfAll("{\"rank\":\"%u\",\"status\":\"%s\"}", 5, led4.on ? "on" : "off");
  ws.printfAll("{\"rank\":\"%u\",\"status\":\"%s\"}", 6, led5.on ? "on" : "off");

  char buffer[400];
  size_t len = serializeJson(json, buffer);
  ws.textAll(buffer, len);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {

    const uint8_t size = JSON_OBJECT_SIZE(6);
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
 // notifyClients();
}

void initWebSocket()
{
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

// ----------------------------------------------------------------------------
// Initialization
// ----------------------------------------------------------------------------

//void estado_botoes()
//{
  // preferences.putBool("state1", led.on);
  // preferences.putBool("state2", led1.on);
  // preferences.putBool("state3", led2.on);
  // preferences.putBool("state4", led3.on);
  // preferences.putBool("state5", led4.on);
  // preferences.putBool("state6", led5.on);
  // Serial.printf("State saved btn1: %d \n", led.on);
  // Serial.printf("State saved btn2: %d \n", led1.on);
  // Serial.printf("State saved btn3: %d \n", led2.on);
  // Serial.printf("State saved btn4: %d \n", led3.on);
  // Serial.printf("State saved btn5: %d \n", led4.on);
  // Serial.printf("State saved btn6: %d \n", led5.on);
  //}

  void rotina_fitas1()
  {
    if (led.on == 1)
    {
      for (uint8_t h = 0; h < quantidade_seguimentos_1; h++)
      {
        for (int i = posicao_ini_leds1[h]; i <= posicao_fin_leds1[h]; i++)
        {
          leds1[i] = CRGB::White;
        }
      }
      FastLED.show();
      delay(100);
    }
    else
    {
      for (uint8_t h = 0; h < quantidade_seguimentos_1; h++)
      {
        for (int i = posicao_ini_leds1[h]; i <= posicao_fin_leds1[h]; i++)
        {
          leds1[i] = CRGB::Black;
        }
      }
      FastLED.show();
      delay(100);
    }
  }

  void rotina_fitas2()
  {

    if (led1.on == 1)
    {
      for (uint8_t h = 0; h < quantidade_seguimentos_2; h++)
      {
        for (int i = posicao_ini_leds2[h]; i <= posicao_fin_leds2[h]; i++)
        {
          leds1[i] = CRGB::White;
        }
      }
      FastLED.show();
      delay(100);
    }
    else
    {
      for (uint8_t h = 0; h < quantidade_seguimentos_2; h++)
      {
        for (int i = posicao_ini_leds2[h]; i <= posicao_fin_leds2[h]; i++)
        {
          leds1[i] = CRGB::Black;
        }
      }
      FastLED.show();
      delay(100);
    }
  }

  void rotina_fitas3()
  {

    if (led2.on == 1)
    {
      for (uint8_t h = 0; h < quantidade_seguimentos_3; h++)
      {
        for (int i = pgm_read_word(&posicao_ini_leds3[h]) ; i <= pgm_read_word(&posicao_fin_leds3[h]); i++)
        {
          leds1[i] = CRGB::White;
        }
      }
      FastLED.show();
      delay(100);
    }
    else
    {
      for (uint8_t h = 0; h < quantidade_seguimentos_3; h++)
      {
        for (int i = pgm_read_word(&posicao_ini_leds3[h]); i <= pgm_read_word(&posicao_fin_leds3[h]); i++)
        {
          leds1[i] = CRGB::Black;
        }
      }
      FastLED.show();
      delay(100);
    }
  }

  void rotina_fitas4()
  {

    if (led3.on == 1)
    {
      for (uint8_t h = 0; h < quantidade_seguimentos_4; h++)
      {
        for (int i = pgm_read_word(&posicao_ini_leds4[h]); i <= pgm_read_word(&posicao_fin_leds4[h]); i++)
        {
          leds1[i] = CRGB::White;
        }
      }
      FastLED.show();
      delay(100);
    }
    else
    {

      for (uint8_t h = 0; h < quantidade_seguimentos_4; h++)
      {
        for (int i = pgm_read_word(&posicao_ini_leds4[h]); i <= pgm_read_word(&posicao_fin_leds4[h]); i++)
        {
          leds1[i] = CRGB::Black;
        }
      }
      FastLED.show();
      delay(100);
    }
  }

  void rotina_fitas5()
  {

    if (led4.on == 1)
    {
      for (uint8_t h = 0; h < 3; h++)
      {
        for (int i = posicao_ini_leds5[h]; i <= posicao_fin_leds5[h]; i++)
        {
          leds1[i] = CRGB::White;
        }
      }

      for (int i = posicao_ini_leds5[3]; i <= posicao_fin_leds5[3]; i++)
      {
       // leds1[i] = CRGB::FairyLight;
        leds1[i] = CRGB::White;
      }

      FastLED.show();
      delay(100);
    }

    else
    {
      for (uint8_t h = 0; h < 4; h++)
      {
        for (int i = posicao_ini_leds5[h]; i <= posicao_fin_leds5[h]; i++)
        {
          leds1[i] = CRGB::Black;
        }
      }
      FastLED.show();
      delay(100);
    }
  }

  void rotina_fitas6()
  {
    if (led5.on == 1)
    {
      for (uint8_t h = 0; h < quantidade_seguimentos_6; h++)
      {
        for (int i = posicao_ini_leds6[h]; i <= posicao_fin_leds6[h]; i++)
        {
          leds1[i] = CRGB::White;
        }
      }
      FastLED.show();
      delay(100);
    }
    else
    {
      for (uint8_t h = 0; h < quantidade_seguimentos_6; h++)
      {
        for (int i = posicao_ini_leds6[h]; i <= posicao_fin_leds6[h]; i++)
        {
          leds1[i] = CRGB::Black;
        }
      }
      FastLED.show();
      delay(100);
    }
  }
