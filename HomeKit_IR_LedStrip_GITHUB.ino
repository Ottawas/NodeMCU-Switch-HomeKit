/* Nom du projet: Apple Homebridge – MQTT IR REMOTE
   GitHub: https://github.com/Ottawas
   Description: Permet de simuler un SWITCH sur HomeKit et d'envoyer un signal infrarouge à la place.
   Projet terminé le: 26/07/2019 21:05
   Version: 0.1.0
   License: MIT
*/

//Importation des librairies.
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <IRsend.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "SSDI DU WIFI"; //SSDI de votre WiFi
const char* password = "MOT DE PASSE DU WIFI"; //Mot de passe de votre WiFi
const char* mqtt_server = "192.168.0.X";

/*
  Votre serveur MQQT, comment l'installer ? Voici un tutoriel: https://www.studiopieters.nl/homebridge-homekit-2-mqtt/
  mqtt_server = "l'adresse ip du raspberry/machine"
*/


int STATE;

const uint16_t ir_led = 4; //On déclare la led infrarouge (4 = D2)
IRsend irsend(ir_led);

WiFiClient espClient;
PubSubClient client(espClient);


void setup_wifi() {
  delay(10);
  // On démarre la connexion au réseau WiFi.
  Serial.println();
  Serial.print("[WIFI] Connexion à ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());
  Serial.println("");
  Serial.println("[WIFI] Connexion réussie !");
  Serial.println("\nAdresse IP: ");
  Serial.println(WiFi.localIP());
}


void callback(char* topic, byte* payload, unsigned int length) {

  if (String(topic) == "switch/bedroom/bureau/state") {     //SI le topic qui répond est "switch/bedroom/bureau/state":
    for (int i = 0; i < length; i++) {                       //On décode la réponse.
      payload[length] = '\0';                                //On la transforme en String puis en Integer.
      String s = String((char*)payload);                     //On la transforme en String puis en Integer.
      int HomeKit_State = s.toInt();                         //On la transforme en String puis en Integer.

      
      if (HomeKit_State == 0 && STATE == 1) {    //DONC AVANT ALLUME
        irsend.sendNEC(0xFF02FD, 32);                     //Envoyer code infrarouge.
 
        Serial.print("\n[INFO] Statut: ");
        Serial.print(HomeKit_State);
        STATE = 0;
        }                                                 //MAINTENANT ETEINT


      else if (HomeKit_State == 1 && STATE == 0) {    //DONC AVANT ETEINT
        irsend.sendNEC(0xFF02FD, 32);                     //Envoyer code infrarouge.
        
        Serial.print("\n[INFO] Statut: ");
        Serial.print(HomeKit_State);
        STATE = 1;
        }                                                  //MAINTENANT ALLUM
         
    }
  }
  
  else {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
  }
}



void reconnect() {
  //Fonction pour se connecter au serveur HomeBridge.
  while (!client.connected()) {
    Serial.print("\n[HomeKit] Connexion au serveur HomeBridge...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);


    //Tentative de connexion.
    if (client.connect(clientId.c_str())) {
      Serial.println("\n[HomeKit] Connexion au serveur réussie !");

      //Connexion réussie, on s'abonne aux topics réglés sur homekit2mqtt.
      client.subscribe("switch/bedroom/bureau/+"); //METTRE VOTRE CONFIG HOMEKIT2MQTT


    } else {
      Serial.print("impossible, rc=");
      Serial.print(client.state());
      Serial.println(" nouvelle tentative dans 5 secondes...");
      delay(5000);
    }
  }
}


void setup() {
  Serial.begin(115200);

  
  irsend.begin(); //On itinie la fonction pour l'infrarouge.

  setup_wifi();
  
  ArduinoOTA.setHostname("ESP-SWITCH"); //Nom pour reconaissance OTA (Over The Air)
  ArduinoOTA.setPassword("VOTRE MDP"); //Mot de passe pour sécurisation.
  ArduinoOTA.begin(); //On initialise l'OTA
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);


}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  ArduinoOTA.handle();
  client.loop();
}
