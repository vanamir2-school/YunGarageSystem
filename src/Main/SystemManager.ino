/*
   This .ino file contains manager of whole system. Primary usage is to
   handle state logic.
 */

// perform check of smoke sensors and move to stateManage
void runSystem(){
        if( !areSmokeSensorsWorking() && state != EMERGENCY ) {
                Serial.println("EMERGENCY - Propably sensor failure, or circuit cut off.");
                state = EMERGENCY;
                changeNextState(EMERGENCY);
                tone(BUZZER_PIN, BUZZER_FREQUENCY_EMERG);
        }
        stateManage();
        digitalWrite(LED_PIN, isGarageUnlocked());
}

// performs operations for current garage state
void stateManage(){
        switch (state) {
        case LOCKED:
                if( isGarageOnFire() ) {
                        changeNextState(FIRE);
                        tone(BUZZER_PIN, BUZZER_FREQUENCY);
                        Serial.println("FIRE - gas escape or fire.");
                }
                else if( isThereMotion() ) {
                        changeNextState(DISRUPTION);
                        tone(BUZZER_PIN, BUZZER_FREQUENCY);
                        Serial.println("DISRUPTION - motion.");
                }
                else if( !areDoorsClosed() ) {
                        changeNextState(DISRUPTION);
                        tone(BUZZER_PIN, BUZZER_FREQUENCY);
                        Serial.println("DISRUPTION - opened doors.");
                }
                else if( isChangeOfState() ) {
                        changeNextState(UNLOCKED);
                        Serial.println("UNLOCK - lock possible after 3 sec.");
                }
                break;

        case UNLOCKED:
                if( isChangeOfState() ) { // in "utility" file
                        changeNextState(LOCKED);
                        Serial.println("LOCK - you can unlock it again after 3 sec");
                }
                break;

        case FIRE:
                debugToSerialEveryXSeconds(4);
                if( millis() % 4000 == 0) {
                        Serial.println(FIRE_SOS);
                        delay(1);
                }

                checkSoftReset();
                break;

        case DISRUPTION:
                debugToSerialEveryXSeconds(4);
                if( millis() % 4000 == 0) {
                        Serial.println(DISRUPTION_SOS);
                        delay(1);
                }
                checkSoftReset();
                break;

        case EMERGENCY:
                debugToSerialEveryXSeconds(4);
                checkSoftReset();
                break;
        }
        state = nextState;
}
