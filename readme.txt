Description:
Continuous measurement tester for MyWeigh HD-300 RS232 scale.  Note that this has only been tested on Windows and changes may be necessary to ScaleLogger.py if running on another OS.

Setup Instructions:
1. Wire a TMP36 sensor to an arduino as described in SerialThermometer.ino and flash firmware to Arduino.
2. Place Arduino + temp sensor near scale
3. Attach RS232 between MyWeigh HD-300 scale and PC
4. Power up the scale and set to SCI.0 mode
5. Set desired values for tempPortName, scalePortName, logFile, and sampleInterval in ScaleLogger.py.
6. Run ScaleLogger.py

Files:
ScaleLogger.py - script to gather test samples
SerialThermometer.ino - Arduino firmware to act as a serial thermometer

Ken Sharp
December 2012
