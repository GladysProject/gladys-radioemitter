/*
 * This sketch demonstrates how to use InterruptChain to receive and
 * decode remote switches (old and new) and remote sensors.
 *
 * Basically, this sketch combines the features of the ShowReceivedCode
 * and ShowReceivedCodeNewKaku examples of RemoteSwitch and the
 * ThermoHygroReceiver of RemoteSensor all at the same time!
 *
 * After uploading, enable the serial monitor at 115200 baud.
 * When you press buttons on a 433MHz remote control, as supported by 
 * RemoteSwitch or NewRemoteSwitch, the code will be echoed.
 * At the same time, if data of a remote thermo/hygro-sensor is
 * received, as supported by RemoteSensor, it will be echoed as well.
 *
 * Configuration  
 * Pin 2 => interruption 0 => radio receiver
 * Pin 10 => 433Mhz emitter
 * 
 * Wait for a JSON on serial port. When a JSON is received, it calls the right function
 * Code example : (the % is for the end of the command). Put thats code into the serial to test
 * {"function_name":"SendRadioCode","code":"16129807"}%
 * => call the SendRadioCode function with code "16129807"
 * {"function_name":"SendNewCode","Addr":7856123,"group":0,"unit":1,"value":0,"period":268}%
 * => call the SendNewCode function with code 7856123 unit 1 to off.
 */

#include <ArduinoJson.h>
#include <RemoteReceiver.h>
#include <RemoteTransmitter.h>
#include <NewRemoteReceiver.h>
#include <NewRemoteTransmitter.h>
#include <InterruptChain.h>

void SendRadioCode(unsigned int period, long code) {

  RemoteReceiver::disable();

  // Need interrupts for delay()
  interrupts();

  // Wait 0.5 seconds before sending.
  delay(500);
   // Retransmit the signal 8 times ( == 2^3) on pin 10. Note: no object was created!
  RemoteTransmitter::sendCode(10, code, period, 3);//old period : 536

  RemoteReceiver::enable();
}


void SendNewCode(unsigned int period, unsigned long address, boolean groupBit, int switchType, byte unit) {

  NewRemoteReceiver::disable();

  // Need interrupts for delay()
  interrupts();

  // Wait 0.5 seconds before sending.
  delay(500);
  

  // Create a new transmitter with the received address and period, use digital pin 10 as output pin

  NewRemoteTransmitter transmitter(address, 10, period);

  
  // On/Off signal received
  bool isOn = switchType == NewRemoteCode::on;

  if (groupBit) {
    // Send to the group
    transmitter.sendGroup(isOn);
  } 
  else {
    // Send to a single unit
    transmitter.sendUnit(unit, isOn);
  }

  NewRemoteReceiver::enable();

}


// Serial buffer
String command = "";

// End of command marker
char endMarker = '%';

/*
 * Execute the right function
 */
void executeFunction(String json_data) {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& v = jsonBuffer.parseObject(json_data);
  //on décompose la chaine de cartère
  if ( v["function_name"] == String("SendRadioCode") ) {
    SendRadioCode(v["period"], v["code"]);
  }else if ( v["function_name"] == String("SendNewCode") ) {
    //Serial.print("sending new code");
    SendNewCode(v["period"], v["Addr"], v["group"], v["value"], v["unit"]);
  } 
}

// shows the received code sent from an old-style remote switch
void showOldCode(unsigned long receivedCode, unsigned int period) {
  // Print the received code.
  if (receivedCode == 0) {
    Serial.print("Unknown encoding");
  } else {
    Serial.print("{\"action\":\"received\",\"value\":");
    Serial.print(receivedCode);
	Serial.print(",\"period\":");
    Serial.print(period);
    Serial.println("}");
  }
}

// Shows the received code sent from an new-style remote switch
void showNewCode(NewRemoteCode receivedCode) {
  // Print the received code.

  if (receivedCode.address == 0) {
    Serial.print("Unknown encoding");
  } else {
    Serial.print("{\"action\":\"received\",\"Addr\":");
    Serial.print(receivedCode.address);
    Serial.print(",\"group\":");
    Serial.print(receivedCode.groupBit);
    Serial.print(",\"unit\":");
    Serial.print(receivedCode.unit);
    Serial.print(",\"value\":");
    Serial.print(receivedCode.switchType);
	Serial.print(",\"period\":");
    Serial.print(receivedCode.period);
    Serial.println("}");
  }
}


void setup() {
  Serial.begin(9600);

  // Interrupt -1 to indicate you will call the interrupt handler with InterruptChain
  RemoteReceiver::init(-1, 2, showOldCode);

  // Again, interrupt -1 to indicate you will call the interrupt handler with InterruptChain
  NewRemoteReceiver::init(-1, 2, showNewCode);

  // Set interrupt mode CHANGE, instead of the default LOW.
  InterruptChain::setMode(0, CHANGE);

  // On interrupt, call the interrupt handlers of remote and sensor receivers
  InterruptChain::addInterruptCallback(0, RemoteReceiver::interruptHandler);
  InterruptChain::addInterruptCallback(0, NewRemoteReceiver::interruptHandler);
}

void loop() {
  // You can do other stuff here!
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
