# Gladys radioemitter

RadioEmitter is used to control radio devices and receive datas from sensors. This version is now compatible with various brand such as DI-O.

## Installation

If you had previous version of this module(and had serial module installed), you need to uninstall it as it is not needed anymore and can cause this module to fail(two modules can't connect to the same serial interface).

- Then, install this module in Gladys
- Push the [arduino code](https://github.com/isokar/gladys-radioemitter/blob/master/new_433_gladys.ino) to your arduino with the [Arduino IDE](https://www.arduino.cc/en/main/software). You need to have `ArduinoJson` and [`433MHzForArduino`](https://bitbucket.org/fuzzillogic/433mhzforarduino/src) libraries installed. To install the libraries, in the arduino IDE (version > 1.6) go to "Sketch" => "Include Library" => "Manage Libraries". Then search "ArduinoJson" and click on "install".
- Connect your arduino in USB to your Raspberry Pi
- Reboot Gladys
- check your console for the correct serial_port ton configure for your Arduino(if you change the parameter, Gladys need to reboot to take it into account)
- To add devices to Gladys you can just activate it(via remote control for exemple). Otherwise, you can create manually in the Device view with the following info => 

### For old fashion

| name | identifier | protocol | service 
| ---| ---| ---| ---| 
| My Switch | YOUR_DECIMAL_CODE_FOR_THIS_SWITCH_OFF | radio | radioemitter 

### For New fashion with non+1 code

| name | identifier | protocol | service 
| ---| ---| ---| ---| 
| My Switch | YOUR_DECIMAL_CODE_FOR_THIS_SWITCH_OFF.YOUR_DECIMAL_CODE_FOR_THIS_SWITCH_ON | diffRF | radioemitter 

### For DIO compatible modules

| name | identifier | protocol | service 
| ---| ---| ---| ---| 
| My Switch | YOUR_DECIMAL_ADRESS.YOUR_DECIMAL_UNIT | DIO | radioemitter 



- Then, inside this device, create a deviceType :

| type | min | max 
| ---| ---| ---|
| binary | 0 | 1
