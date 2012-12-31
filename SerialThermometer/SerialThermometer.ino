/*
  Serial Thermometer
 
 This program waits for a 't' character in the serial port, and 
 sends a temperature value (in degrees Fahrenheit) over the serial port.
 
 The circuit:
  temp sensor attached to analog input 0, powered directly
   from Arduino's 5V and GND.

  temp sensor is an Analog Devices TMP36.
  This sensor outputs 10mV per degree Celsius
 
  Copyright (c) 2012 Ken Sharp
  License: http://opensource.org/licenses/mit-license.php 
 */

char inByte = 0;         // incoming serial byte
float temperature;
int adcSample;

void setup()
{
  // start serial port
  Serial.begin(9600);
}

void loop()
{
  // if we get a valid byte, read analog ins:
  if (Serial.available() > 0) {
    // get incoming byte:
    inByte = Serial.read();
	if (inByte == 't') {
		// read raw temperature
		adcSample = analogRead(A0);
		// convert to mV (assume 5V Vref)
		temperature = adcSample * (5000.0/1024.0);
		// convert to degrees Celsius
		temperature = (temperature - 500.0) / 10;
		// convert to degrees Fahrenheit
		temperature = (temperature * 1.8) + 32;
		// send sensor values:
		Serial.println(temperature);
	}
  }
}
