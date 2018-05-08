/*
   This source code refers to bachelor thesis "Intelligent garage security system"
   Author: Miroslav Váňa
   URL: https://github.com/vanamir2/YunGarageSystem
 */

/* Arduino libraries */
#include <Arduino.h>
#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>

/* Own files, more info in header of particular file */
#include <constants.h>
#include <sensors.h>

/* There are also files "REST.ino", "SystemManager.ino" and "Utility.ino"
   which include usefull methods. More informations are included in the
   headers of each file.
 */

// Listen to the default port 5555, the Yún webserver
// will forward there all the HTTP requests you send
BridgeServer server;
BridgeClient client;

unsigned long timeStampSystemManager = 0; // save value of millis() when state is changed
states state, nextState;

void setup() {
        Serial.begin(9600);
        Bridge.begin();
        inicializationOfPins();
        inicializationOfComponents();

        // Listen for incoming connection only from localhost
        server.listenOnLocalhost();
        server.begin();
        state = nextState = UNLOCKED;
        timeStampSystemManager = millis();
}

void inicializationOfPins(){
        pinMode(LED_PIN, OUTPUT);
        pinMode(BUTTON_PIN, INPUT_PULLUP);
        pinMode(BUZZER_PIN, OUTPUT);
        noTone(BUZZER_PIN);

        // Inicialization of magnetic switches
        for(int i =0; i < NUM_OF_SWITCHES; ++i)
                pinMode(SWITCH_PINS[i], INPUT_PULLUP);

        // Inicialization of smoke and gas detector
        for(int i =0; i < NUM_OF_SMOKE_GAS; ++i) {
                pinMode(SMOKE_PINS_DIGIAL[i], INPUT);
                pinMode(SMOKE_PINS_ANALOG[i], INPUT);
        }
        // Inicialization of PIR sensors
        for(int i =0; i < NUM_OF_PIRS; ++i)
                pinMode(PIR_PINS[i], INPUT);
}

void inicializationOfComponents(){
        if( millis() < INICIALIZATION_TIME ) {
                Serial.println("Inicialization of the garage system, wait 3 minutes please.");
                while( millis() < INICIALIZATION_TIME ) {
                        if( millis() % 10000 == 0) {
                                Serial.print("... ");
                                Serial.print( map(millis(), 0, INICIALIZATION_TIME, 0, 100) );
                                Serial.println("%");
                                delay(1);
                        }
                }
                findAverageForSmoke();
                //  printAverageForSmoke();
                Serial.println("Inicialization done");
        }
        Serial.println("Running system - garage is unlocked");
}

void loop() {
        runSystem(); // goes to SystemManager.ino
        client = server.accept(); // check HTTP requests
        if (client)
                processClientAndStop(client); // goes to REST.ino
}
