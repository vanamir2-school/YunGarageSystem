/*
   This .ino file contains methods and macros that are supporting mostly
   SystemManager.ino and REST.ino. It is to maintain these files well-arranged.
 */

// macros from DateTime.h
#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24L)

/* Useful Macros for getting elapsed time */
#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN)
#define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)


unsigned long timeStampUtility = 0;
bool timeStampOn = false; // flag

bool isXSecondsGone(unsigned ms){
        // first time
        if(!timeStampOn) {
                timeStampOn = true;
                timeStampUtility = millis();
                return false;
        } // is x seconds gone?
        else if ( (timeStampUtility + ms*1000) < millis() ) {
                timeStampOn = false;
                return true;
        }
        return false;
}

int isGarageUnlocked(){
        return (state == UNLOCKED ? HIGH : LOW);
}

// ------- BUTTON to change state (lock/unclock)
int buttonState = 0; // 1 pressed, 0 unpressed
unsigned long timeStamp = 0;


// the smallest value is 1 sec
bool isButtonPressedFor(unsigned seconds){
        seconds *=10;
        unsigned holding = 0;
        do {
                if(!isButtonPressed())
                        return false;
                ++holding;
                delay(90); // wait ~ 0,1 sec and try out if still holding
        } while(seconds > holding);

        return true;
}

bool isButtonPressed(){
        return !digitalRead(BUTTON_PIN);
}

// change of state when using pushbutton is possible after specific amount of time (STATE_LOCKED_FOR)
bool isChangeAllowed( unsigned long timeStamp ){
        return (millis()-timeStamp) > STATE_LOCKED_FOR;
}

// check if the button was pressed
bool isChangeOfState(){
        if( isButtonPressed() && isChangeAllowed(timeStamp) ) {
                timeStamp = millis();
                buttonState = isButtonPressed();
                return true;
        }
        return false;
}

// print debug to serial line
void debugToSerialEveryXSeconds(unsigned x){
        if( isXSecondsGone(x) ) {
                Serial.print("*********  ");
                Serial.print(millis());
                Serial.println(" ms from boot ***********");

                Serial.print(smokeAndGasDebug());
                Serial.print(magneticSwitchDebug());
                Serial.print(PIRdebug());
        }
}

// check if there is request to perform software reset of garage system
// (performed by holding pushbutton for amount of time at SECONDS_TO_SOFT_RESET)
void checkSoftReset(){
        if(isButtonPressed()) {
                if( isButtonPressedFor(SECONDS_TO_SOFT_RESET) ) {
                        changeNextState(UNLOCKED);
                        Serial.println("UNLOCK - SOFT RESET button");
                        noTone(BUZZER_PIN);
                }
                // wait until user unpush button
                while(isButtonPressed()) {
                        delay(10);
                }
        }
}

void changeNextState(states newState){
        nextState = newState;
        timeStampSystemManager = millis();
}

// -------------------------- UTILITY FOR "REST.ino"

//return String with digital values for method "getValues"
String getValuesToStr(int NUM, bool * arrWithValues){
        String tmp = "";
        for(int i =0; i < NUM; ++i) {
                tmp += String(arrWithValues[i]);
                i==(NUM-1) ? tmp += String(";") : tmp += String(",");
        }
        return tmp;
}

//return String with analog values for method "getValues"
String getValuesToStr(int NUM, int * arrWithValues){
        String tmp = "";
        for(int i =0; i < NUM; ++i) {
                tmp += String(arrWithValues[i]);
                i==(NUM-1) ? tmp += String(";") : tmp += String(",");
        }
        return tmp;
}


// print time as "HH:MM:SS"
void msToTime(unsigned long val){
        int hours = numberOfHours(val);
        int minutes = numberOfMinutes(val);
        int seconds = numberOfSeconds(val);

        printDigits(hours);
        client.print(":");
        printDigits(minutes);
        client.print(":");
        printDigits(seconds);
        client.println();
}

// utility function for digital clock display: prints colon and leading 0
void printDigits(byte digits){
        if(digits < 10)
                client.print('0');
        client.print(digits);
}
