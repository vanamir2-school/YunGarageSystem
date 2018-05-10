/*
   This header file includes major part of contants used in this program.
   Format of constants is UPPER case with _ separator ( EXAMPLE_USEGE )
 */

// --------------------------------------------- PIN DEFITION
const int NUM_OF_SWITCHES = 2, NUM_OF_PIRS = 2, NUM_OF_SMOKE_GAS = 2;
const int SWITCH_PINS [] = {11,10}, PIR_PINS [] = {9,8};
const int SMOKE_PINS_DIGIAL [] = {7,6,}, SMOKE_PINS_ANALOG [] = {A1,A0};
const int BUZZER_PIN =3, LED_PIN = 13, BUTTON_PIN = 12;// pushButton ( 1 == pushed )

// --------------------------------------------- SMOKE & GAS
const int LIMIT_VALUES_DIFF [NUM_OF_SMOKE_GAS] = {50,80};  // based on measured data and tests
const int MAX_VALUE = 900;
const int MIN_VALUE = 75;
const char SMOKE_AND_GAS_DEBUG [] = "-------------------------- SMOKE & GAS debug\n";
const char SMOKE_AND_GAS [] = "Smoke&Gas detector: ";
const char ANALOG_VALUE_IS [] = " has analog value: ";

// --------------------------------------------- MAGNETIC SWITCH
const char SWITCH_DEBUG [] = "-------------------------- MAGNETIC SWITCH debug\n";
const char MAGNETIC_SWITCH [] = "Magnetic switch: ";
const char IS_SWITCHED_ON [] = " --> is switched ON - OK.\n";
const char IS_SWITCHED_OFF [] = " is switched OFF - DANGER.\n";

// --------------------------------------------- PIRs
const char PIR_DEBUG [] = "-------------------------- PIR debug\n";
const char PIR [] = "PIR ";
const char MOTION_IS_DETECTED [] = " --> 1 - is detecting motion.\n";
const char SWITCHED_ON_X_Y_TIMES [] = "were switched on X Y times: ";
const char MOTION_IS_NOT_DETECTED [] = "... motion not detected.\n";

// --------------------------------------------- SYSTEM MANAGER
const unsigned int BUZZER_FREQUENCY = 1500, SECONDS_TO_SOFT_RESET = 2;
const unsigned int BUZZER_FREQUENCY_EMERG = 6000;
const char FIRE_SOS [] = "SOS - smoke/gas. Call fire-fighters.";
const char DISRUPTION_SOS [] = "SOS - Physical disruption of the object. Call the owner or cops!";

// --------------------------------------------- UTILITTY
const unsigned long INICIALIZATION_TIME = 170000; // 170s -> ~ 3 minutes
const unsigned STATE_LOCKED_FOR = 3000;  // ms

enum states {
        LOCKED, UNLOCKED, FIRE, DISRUPTION, EMERGENCY
};

const char * statesString [] = { "LOCKED", "UNLOCKED", "FIRE", "DISRUPTION", "EMERGENCY"};
const char AT_PIN [] = " at pin: ";
