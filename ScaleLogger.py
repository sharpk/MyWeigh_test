# This script logs readings from a MyWeigh HD-150 or HD-300.
# The scale is expected to be in SCI.0 mode (see scale manual)
# The script also reads the temperature from an arduino board
# which is configured to respond on the serial port
# with the current temperature every time it recives a 't' character
# The samples of both temperature and weight are logged to a CSV formatted file
#
# Note that the pySerial module is required.
#
# Copyright (c) 2012 Ken Sharp
# License: http://opensource.org/licenses/mit-license.php

import serial
import time
from datetime import datetime

tempPortName = "COM4"
scalePortName = "COM1"
sampleInterval = 10  # in minutes
logFile = "C:\\weight_log.txt"

print "Starting scale logger"
tempPort = serial.Serial(tempPortName, 9600, timeout=2)
scalePort = serial.Serial(scalePortName, 9600, timeout=2)
f = open(logFile, 'wc')

firstIteration = True

# sleep for a few seconds to give the arduino time to init
# (it resets after opening a serial port to it)
time.sleep(5)

while True:
	try:
		# get a temperature
		tempPort.write("t")
		tempSample = tempPort.readline()
		# get a weight (Example: ":W 150.40lbS")
		scalePort.write("\r")
		scaleSample = scalePort.read(15)
		# Construct sample string
		dt = datetime.now()
		dateStr = dt.strftime("%m/%d/%y %H:%M")
		sampleStr = dateStr + "," + tempSample.strip() + "," + scaleSample[3:9] + "\n"
		# Write samples to the log file
		if firstIteration:
			# Print the column labels
			f.write("DateTime,Temperature(degF),Weight(" + scaleSample[9:11] + ")\n")
			firstIteration = False
		f.write(sampleStr)
		f.flush()
		# Sleep until next sample
		time.sleep(sampleInterval*60)
	except KeyboardInterrupt:
		print "Received Ctrl-C! Exiting..."
		break

tempPort.close()
scalePort.close()
f.close()
