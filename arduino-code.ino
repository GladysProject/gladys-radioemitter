/*
 * Receiver is not mandatory !!
 * You can just have a emitter.
 * 
 * Configuration  
 * Pin 2 => interruption 0 => radio receiver
 * Pin 10 => 433Mhz emitter
 * 
 * Wait for a JSON on serial port. When a JSON is received, it calls the right function
 * Code example : (the % is for the end of the command). Put thats code into the serial to test
 * {"function_name":"SendRadioCode","code":"16129807"}%
 * => call the SendRadioCode function with code "16129807"
 */
 
#include <ArduinoJson.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void SendRadioCode(long code) {
   mySwitch.send(code, 24);
}

// Serial buffer
String command = "";

// End of command marker
char endMarker = '%';

/*
 * Execute the right function
 */
void executeFunction(String json_data) {
  StaticJsonDocument<200> jsonDocument;
  deserializeJson(jsonDocument, json_data);
  if ( jsonDocument["function_name"] == String("SendRadioCode") ) {
    SendRadioCode(jsonDocument["code"]);
  } 
}

/**
 * This function is automatically called when data is received on serial port
 */
void serialEvent() {
    //lit toutes les données (vide le buffer de réception)
    char last_readed_char = Serial.read();
    if ( last_readed_char == endMarker ) {
      executeFunction(command);
      command = "";
    } else {
      command += last_readed_char;
    }
}


void setup() {
  
  // Open serial communications and wait for port to open: 
  Serial.begin(9600);

  // Receiver is connected on Arduino Pin #2
  mySwitch.enableReceive(0);

  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(10);
  
  // Optional set pulse length.
  mySwitch.setPulseLength(310);
  
  // Optional set protocol (default is 1, will work for most outlets)
  mySwitch.setProtocol(1);
  
  // Optional set number of transmission repetitions.
  mySwitch.setRepeatTransmit(15);

  delay(1000);
}

void loop() {
  
  if (mySwitch.available()) {
    int value = mySwitch.getReceivedValue();
    
    if (value == 0) {
      Serial.print("Unknown encoding");
    } else {
      Serial.print("{\"action\":\"received\",\"value\":");
      Serial.print( mySwitch.getReceivedValue() );
      Serial.println("}");
    }
    delay(200); 
    mySwitch.resetAvailable();
  }
}
