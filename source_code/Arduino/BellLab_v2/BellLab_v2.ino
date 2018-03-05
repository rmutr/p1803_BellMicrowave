/*  NETPIE ESP32 basic sample                            */
/*  More information visit : https://netpie.io           */

#include <WiFi.h>
#include "MicroGear.h"

//----- Bell Lab v2 ----- [ 1/4 ] ------------------------
//#define ENVIRONMENTS_BELL
#define ENVIRONMENTS_JAB
//#define ENVIRONMENTS_EAK

#ifdef  ENVIRONMENTS_BELL

#endif

#ifdef  ENVIRONMENTS_JAB

const char* ssid     = "Jab";
const char* password = "11111111";

#define APPID   "TESTJAB"
#define KEY     "cXwY6W281vrgZ8k"
#define SECRET  "6riXNrryEylaLkhDzmwejxk6E"

#endif

#ifdef  ENVIRONMENTS_EAK

#endif

//--------------------------------------------------------

#define ALIAS   "esp8266"

WiFiClient client;

int timer = 0;
MicroGear microgear(client);

/* If a new message arrives, do this */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
    Serial.print("Incoming message --> ");
    msg[msglen] = '\0';
    Serial.println((char *)msg);
}

void onFoundgear(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.print("Found new member --> ");
    for (int i=0; i<msglen; i++)
        Serial.print((char)msg[i]);
    Serial.println();  
}

void onLostgear(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.print("Lost member --> ");
    for (int i=0; i<msglen; i++)
        Serial.print((char)msg[i]);
    Serial.println();
}

/* When a microgear is connected, do this */
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.println("Connected to NETPIE...");
    /* Set the alias of this microgear ALIAS */
    microgear.setAlias(ALIAS);
}

//----- Bell Lab v2 ----- [ 2/4 ] ------------------------
unsigned long t_old = 0;
int d_raw = 0;
int d_val = 0;

void plotRaw() {
  Serial.println(d_raw);
}

void plotVolt() {
  Serial.println((float)d_val / 1000);
}

void serialTest() {
  Serial.print(d_raw);
  Serial.print(" ");
  Serial.print(d_val);
  Serial.println();
}

//--------------------------------------------------------

void setup() {

//----- Bell Lab v2 ----- [ 3/4 ] ------------------------
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println("\nBell Lab v2 starting...");

  analogReadResolution(12);

//--------------------------------------------------------

    /* Add Event listeners */

    /* Call onMsghandler() when new message arraives */
    microgear.on(MESSAGE,onMsghandler);

    /* Call onFoundgear() when new gear appear */
    microgear.on(PRESENT,onFoundgear);

    /* Call onLostgear() when some gear goes offline */
    microgear.on(ABSENT,onLostgear);

    /* Call onConnected() when NETPIE connection is established */
    microgear.on(CONNECTED,onConnected);

    /* Initial WIFI, this is just a basic method to configure WIFI on ESP8266.                       */
    /* You may want to use other method that is more complicated, but provide better user experience */
    if (WiFi.begin(ssid, password)) {
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
    }

    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    /* Initial with KEY, SECRET and also set the ALIAS here */
    microgear.init(KEY,SECRET,ALIAS);

    /* connect to NETPIE to a specific APPID */
    microgear.connect(APPID);
}

void loop() {
    /* To check if the microgear is still connected */
    if (microgear.connected()) {
        Serial.println("connected");

        /* Call this method regularly otherwise the connection may be lost */
        microgear.loop();

        if (timer >= 1000) {
            Serial.println("Publish...");

            /* Chat with the microgear named ALIAS which is myself */
            microgear.chat(ALIAS,"Hello");
            timer = 0;
        } 
        else timer += 100;
    }
    else {
        Serial.println("connection lost, reconnect...");
        if (timer >= 5000) {
            microgear.connect(APPID);
            timer = 0;
        }
        else timer += 100;
    }
    delay(100);
}
