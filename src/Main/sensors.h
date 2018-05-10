/*
   This header file includes methods to work with sensors and print usefull
   outputs.
 */

// S&G
int smokeValuesAnalog[NUM_OF_SMOKE_GAS] = {0};
bool smokeValuesDigital[NUM_OF_SMOKE_GAS] = {0};
int averageSmokeValues [NUM_OF_SMOKE_GAS] = {0};
// MagSwitches
bool switchesValues[NUM_OF_SWITCHES] = {0};
//PIR
bool pirValues [NUM_OF_PIRS] = {0};
int sumPIRs[NUM_OF_PIRS] = {0};


// reading of digital sensor values to array
void digitalReadToArray( const int * pins, int numOfPins, bool * resultArray ){
        for(int i =0; i < numOfPins; ++i)
                resultArray[i] = digitalRead(pins[i]);
}
// reading of analog sensor values to array
void analogReadToArray( const int * pins, int numOfPins, int * resultArray ){
        for(int i =0; i < numOfPins; ++i)
                resultArray[i] = analogRead(pins[i]);
}

// ----------------------------------------------- Smoke and Gas sensons
// compute average analog value (after inicialization)
void findAverageForSmoke(){
        averageSmokeValues [NUM_OF_SMOKE_GAS] = {0};

        const int READ_X_TIMES = 3; // read 3 times
        for(int i = 0; i < READ_X_TIMES; ++i) {
                analogReadToArray(SMOKE_PINS_ANALOG,NUM_OF_SMOKE_GAS,smokeValuesAnalog);
                for(int j =0; j< NUM_OF_SMOKE_GAS; ++j)  // from all smoke sensors
                        averageSmokeValues[j] += smokeValuesAnalog[j];
                delay(3333);
        }
        for(int j =0; j< NUM_OF_SMOKE_GAS; ++j)  // from all smoke sensors
                averageSmokeValues[j] /= READ_X_TIMES;
}

void printAverageForSmoke(){
        for(int j =0; j< NUM_OF_SMOKE_GAS; ++j)  // from all smoke sensors
                Serial.println(averageSmokeValues[j]);
}

// testing if actual value is not higher then average + variance
bool isSmokeOrGasOverLimit(int analogValue, int pos){
        return analogValue > (LIMIT_VALUES_DIFF[pos]+averageSmokeValues[pos]);
}

// MAESURED BY ANALOG VALUE
bool isGarageOnFire(){
        analogReadToArray(SMOKE_PINS_ANALOG,NUM_OF_SMOKE_GAS,smokeValuesAnalog);
        for(int i = 0; i < NUM_OF_SMOKE_GAS; ++i) {
                if(isSmokeOrGasOverLimit(smokeValuesAnalog[i], i))
                        return true;
        }
        return false;
}

// performs check if analog values are in possible range
bool areSmokeSensorsWorking(){
        analogReadToArray(SMOKE_PINS_ANALOG,NUM_OF_SMOKE_GAS,smokeValuesAnalog);
        for(int i = 0; i < NUM_OF_SMOKE_GAS; ++i) {
                if( smokeValuesAnalog[i] < MIN_VALUE )
                        return false;
                if( smokeValuesAnalog[i] > MAX_VALUE )
                        return false;
        }
        return true;
}

// returns debug output of values
String smokeAndGasDebug(){
        String tmp = "";
        tmp.concat(SMOKE_AND_GAS_DEBUG);
        digitalReadToArray(SMOKE_PINS_DIGIAL,NUM_OF_SMOKE_GAS,smokeValuesDigital);
        analogReadToArray(SMOKE_PINS_ANALOG,NUM_OF_SMOKE_GAS,smokeValuesAnalog);
        for(int i =0; i < NUM_OF_SMOKE_GAS; ++i) {
                tmp.concat(SMOKE_AND_GAS);
                tmp.concat(i);
                tmp.concat(ANALOG_VALUE_IS);
                tmp.concat(smokeValuesAnalog[i]);
                if(isSmokeOrGasOverLimit(smokeValuesAnalog[i], i))
                        tmp.concat(" -- LIMIT EXCEEDED\n");
                else
                        tmp.concat(" -- OK.\n");
        }
        return tmp;
}
// ----------------------------------------------- PIRs
// AND output of PIR to prevent false/fake alarms
bool isMotionEverywhere(){
        for(int i =0; i < NUM_OF_PIRS; ++i)
                if(pirValues[i] == LOW) //  no motion
                        return false;
        return true;
}

bool isThereMotion(){
        digitalReadToArray(PIR_PINS,NUM_OF_PIRS,pirValues);
        return isMotionEverywhere();
}

// returns debug output of values
String PIRdebug(){
        String tmp = "";
        tmp.concat(PIR_DEBUG);
        digitalReadToArray(PIR_PINS,NUM_OF_PIRS,pirValues);
        for(int i =0; i < NUM_OF_PIRS; ++i) {
                if(pirValues[i] == HIGH) {
                        tmp.concat(PIR);
                        tmp.concat(i);
                        tmp.concat(AT_PIN);
                        tmp.concat(PIR_PINS[i]);
                        tmp.concat(MOTION_IS_DETECTED);
                        sumPIRs[i]++;

                        tmp.concat(SWITCHED_ON_X_Y_TIMES);
                        for(int i =0; i<NUM_OF_PIRS; ++i) {
                                tmp.concat(sumPIRs[i]);
                                tmp.concat(" ");
                        }
                        tmp.concat("\n");
                }
                else{
                        tmp.concat(MOTION_IS_NOT_DETECTED);
                }
        }
        return tmp;
}
// ----------------------------------------------- MAGNETIC SWITCH
// 0 == switched on
// 1 == switched off (PULL-UP resistor makes the value HIGH (1))
bool isThereMagnet(bool state){
        return !state;
}
// returns true if all magnetic switches are switched on
bool areSwitchesClosed(){
        for(int i =0; i < NUM_OF_SWITCHES; ++i)
                if( !isThereMagnet(switchesValues[i]) ) // je zvednut pull up rezistorem
                        return false;
        return true;
}

// read magnetic switch values and returns state of doors (1 == closed)
bool areDoorsClosed(){
        digitalReadToArray(SWITCH_PINS,NUM_OF_SWITCHES,switchesValues);
        return areSwitchesClosed();
}

// returns debug output of values
String magneticSwitchDebug(){
        String str = "";
        str.concat(SWITCH_DEBUG);
        digitalReadToArray(SWITCH_PINS,NUM_OF_SWITCHES,switchesValues);

        for(int i =0; i < NUM_OF_SWITCHES; ++i) {
                str.concat(MAGNETIC_SWITCH);
                str.concat(i);
                str.concat(AT_PIN);
                str.concat(SWITCH_PINS[i]);
                if( isThereMagnet(switchesValues[i]) )
                        str.concat(IS_SWITCHED_ON);
                else
                        str.concat(IS_SWITCHED_OFF);
        }
        return str;
}
