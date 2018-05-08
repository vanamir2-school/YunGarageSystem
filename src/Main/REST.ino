/*
   This .ino file includes methods to handle REST commands.
 */

// check all HTTP commands from REST API and process them
void processClientAndStop(BridgeClient client){
        String command = client.readString(); // read the command
        command.trim(); // remove whitespaces

        if (command == "state" )
                getState(client);
        else if (command == "values")
                getValues(client);
        else if (command == "debug")
                debug(client);
        else if (command == "time")
                timeFromStateChange(client);
        else if (command == "valuesHelp")
                getValuesHelp(client);
        else if (command == "commands")
                commands(client);
        else
                client.print(F("Wrong command"));
        client.stop();
}

// actual state of the system
void getState(BridgeClient client) {
        client.println(statesString[state]);
}

// current values from all sensors in specific output format
void getValues(BridgeClient client) {
        // PIR1_D,PIR2_D,...;S&G1_A,S&G2_A,...;S&G1_D,S&G2_D,...;MS1_D,MS2_D,...;

        String outputStr = getValuesToStr(NUM_OF_PIRS,pirValues);
        outputStr += getValuesToStr(NUM_OF_SMOKE_GAS,smokeValuesAnalog);
        outputStr += getValuesToStr(NUM_OF_SMOKE_GAS,smokeValuesDigital);
        outputStr += getValuesToStr(NUM_OF_SWITCHES,switchesValues);
        client.println(outputStr);
}

// help dialog for command "getValues"
void getValuesHelp(BridgeClient client){
        client.print(F("Output is formatted in this way:\n"));
        client.print(F("PIR1_D,PIR2_D,...;S&G1_A,S&G2_A,...;S&G1_D,S&G2_D,...;MS1_D,MS2_D,...\n"));
        client.print(F("\nPIRx_D = digital value from PIR number x\n"));
        client.print(F("S&Gx_A = analog value from smoke and gas sensor number x\n"));
        client.print(F("S&G2_D = digital value from smoke and gas sensor number x\n"));
        client.print(F("MSx_D = digital value from magnetic switch number x\n"));
        client.print(F(", = separator in same group of sensors\n"));
        client.print(F("; = another group of measurement\n"));
        client.print(F("... = etc (depends how many sensors are connected)") );
}

// list all acceptable commands
void commands(BridgeClient client){
        client.print(F("state\nvalues\ndebug\ntime\nvaluesHelp\ncommands\n"));
}

// print debug output of all sensors
void debug(BridgeClient client) {
        client.print ( smokeAndGasDebug() );
        client.print ( magneticSwitchDebug() );
        client.print ( PIRdebug() );
}

// print time since last state change
void timeFromStateChange(BridgeClient client){
        unsigned long timeFromLastChange  = millis() - timeStampSystemManager;
        msToTime(timeFromLastChange/1000); // pass seconds instead of ms
}
