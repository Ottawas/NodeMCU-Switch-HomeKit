# HomeKit - Simple Led IR

Materiel utilisé:
- 1X Raspberry Pi Zero (aliexpress.com/item/32895891785.html)
- 1X Dongle WiFi pour Raspberry Pi (aliexpress.com/item/32798387984.html)

- 1X NodeMCU (aliexpress.com/item/32665100123.html)
- 1X Émetteur Infrarouge (aliexpress.com/item/1972945414.html)
- 1X Pack de cables DUPONT (aliexpress.com/item/32848619855.html)

Pour le branchement, référez-vous à ce qui est marqué sur l'émetteur infrarouge.

# Fonctionnement
  - Simuler une lumière simple via HomeKit2MQTT hébergé sur un Raspberry Pi.
  - Envoyer une requête infrarouge à l'allumage et extinction avec un ESP8266.
  - Possibilité de modifier le code via OTA (Over The Air).
 

# Installation

Préparer HomeKit2MQTT:
  - Personnellement j'utilise un Raspberry Pi Zero avec une carte SD de 16Go.
  - Tutoriel pour l'installer HomeKit2MQTT:
 https://www.studiopieters.nl/homebridge-homekit-2-mqtt/

  - Tutoriel pour configurer le démarrage automatique du serveur HomeKit2MQTT:
 https://www.studiopieters.nl/homebridge-mqtt-auto-start/

 
Préparer le code sur l'ESP8266:

- Ouvrir l'IDE Arduino
- "Fichier" > "Préférences"
- "URL de gestionnaire de cartes supplémentaires:" > Ajoutez l'adresse suivante: http://arduino.esp8266.com/stable/package_esp8266com_index.json

- "Outils" > "Type de carte" > "Gestionnaire de carte"
- Recherchez "ESP8266" et installez la.
- "Outils" > "Type de carte" > "NodeMCU 1.0"

Il vous faudra maintenant installer les librairies requises:
- PubSubClient: https://github.com/knolleary/pubsubclient/releases
- IRSend: https://github.com/crankyoldgit/IRremoteESP8266/releases
- ArduinoOTA: https://github.com/jandrassy/ArduinoOTA/releases


Puis vous n'avez plus qu'à upload le code sur l'ESP8266 et modifier quelques lignes:
-	`const char* ssid = "SSID DU WIFI";`
-	`const char* password = "MOT DE PASSE DU WIFI";` 
-	`const char* mqtt_server = "IP Locale du Raspberry";`	
-	`irsend.sendNEC(0xFF02FD, 32);` (Paquet IR à envoyer)
-	`client.subscribe("hue/set/lights/lampe");` (Modifier par la configuration de votre HomeKit2MQTT)
-	`ArduinoOTA.setHostname("ESP-LED");` (Nom de l'ESP8266 pour OTA)
-	`ArduinoOTA.setPassword("led");` (Mot de passe pour l'OTA)
	
# Utiliser l'OTA
![OTA](https://image.prntscr.com/image/p9bYNkNvTX6ShD0IJjHk_Q.png)

### ToDo

 - Ajouter la fonction RGB

License
----

MIT
