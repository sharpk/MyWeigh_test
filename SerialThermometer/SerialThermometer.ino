/*
  Serial Thermometer
 
 This program waits for a 't' character in the serial port, and 
 sends a temperature value (in degrees Fahrenheit) over the serial port.
 
 This sketch can optionally read from two different temperature sensors.
 You can pick which temperature sensor to use at compile time by uncommenting
 one of the #define's below
 
 The circuit for TMP36 option:
  temp sensor attached to analog input 0, powered directly
   from Arduino's 5V and GND.

  temp sensor is an Analog Devices TMP36.
  This sensor outputs 10mV per degree Celsius
  
  The circuit for the thermistor option:
  Create a voltage divider from a 10K ohm thermistor and a 10K ohm resistor, like this:
  [Ground] ---- [10k Resistor] -------|------- [Thermistor] ---- [+5v]
                                      |
                                analog input 0
								
  Measure the bias resistor and put the value (in ohms) in the #define biasResistance
  
  analog input 0 should be connected between the thermistor and the resistor.
 
  Parts of the thermistor code are from: http://playground.arduino.cc/ComponentLib/Thermistor2
 
  Copyright (c) 2012 Ken Sharp
  License: http://opensource.org/licenses/mit-license.php 
 */

//#define TMP36
#define THERMISTOR

// actual voltage of the 5V supply (Vref) in mV; only needed for TMP36
#define VOLTAGE_5V 5040.0

// in ohms; this is only needed if you're using the thermistor
#define THERMISTOR_BIAS_RESISTANCE 9910.0

#if defined(TMP36) && defined(THERMISTOR)
	#error "Error: define only one temp sensor option"
#endif

#if !defined(TMP36) && !defined(THERMISTOR)
	#error "Error: define a temp sensor option"
#endif
 
#ifdef THERMISTOR
#include <math.h>
#endif
 
char inByte = 0;         // incoming serial byte
float temperature;
int adcSample;
#ifdef THERMISTOR
long resistance;
#endif

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
#ifdef TMP36
		// convert to mV (assume 5V Vref)
		temperature = adcSample * (VOLTAGE_5V/1024.0);
		// convert to degrees Celsius
		temperature = (temperature - 500.0) / 10;
#endif
#ifdef THERMISTOR
		// Utilizes the Steinhart-Hart Thermistor Equation:
		//    Temperature in Kelvin = 1 / {A + B[ln(R)] + C[ln(R)]^3}
		//    where A = 0.001129148, B = 0.000234125 and C = 8.76741E-08
		resistance=(((1024 * THERMISTOR_BIAS_RESISTANCE)/adcSample) - THERMISTOR_BIAS_RESISTANCE);
		temperature = log(resistance);
		temperature = 1 / (0.001129148 + (0.000234125 * temperature) + (0.0000000876741 * temperature * temperature * temperature));
		temperature = temperature - 273.15;  // Convert Kelvin to Celsius
#endif
		// convert to degrees Fahrenheit
		temperature = (temperature * 1.8) + 32.0;
		// send sensor values:
		Serial.println(temperature);
	}
  }
}
