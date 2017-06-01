/*
 Name:		GPS_Logger.ino
 Created:	6/1/2017 7:57:36 PM
 Author:	Sven Lautenschläger
*/

// Includes
#include <Adafruit_GPS.h>

// Defines
#define SERIAL_BAUDRATE 115200	// Default baud rate for Edison serial USB port
#define GPS_BAUDRATE 9600		// Default baud rate for Adafruit Ultimate GPS v3 module

// Global Classes
Adafruit_GPS GPS(&Serial1);		// GPS module @ UART1

// Runs once when you power the board
void setup()
{
	// Begin serial console / GPS module communication
	Serial.begin(SERIAL_BAUDRATE);
	GPS.begin(GPS_BAUDRATE);

	// Turn on RMC (recommended minimum) and GGA (fix data) including altitude
	GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

	// Set the update rate to 1 Hz / for the parsing code and have time to sort thru the data
	// Don't use a higher frequency than 1 Hz
	GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

	Serial.println("GPS Logger");
}

// Global Variables
static const unsigned int ReportingInterval = 20000;	// Updateperiod (in millis)
unsigned long LastReport = 0;							// Keep track of when we last sent data
String Location = "";									// Will build the Location string here

// Runs over and over, until power down or reset
void loop()
{
	if (millis() >= LastReport + ReportingInterval) 
	{
		LastReport = millis();  // Reset the timer

		Serial.print("\nTime: ");
		// TODO: check current timezone / central european summer time
		Serial.print(GPS.hour, DEC); 
		Serial.print(':');
		Serial.print(GPS.minute, DEC); 
		Serial.print(':');
		Serial.print(GPS.seconds, DEC); 
		Serial.print('.');
		Serial.println(GPS.milliseconds);
		Serial.print("Date: ");
		Serial.print(GPS.day, DEC); 
		Serial.print('/');
		Serial.print(GPS.month, DEC); 
		Serial.print("/20");
		Serial.println(GPS.year, DEC);
		Serial.print("Fix: "); 
		Serial.print((int)GPS.fix);
		Serial.print(" quality: "); 
		Serial.println((int)GPS.fixquality);
		
		if (GPS.fix) 
		{
			Serial.print("Location: ");
			Serial.print(GPS.latitude, 4); 
			Serial.print(GPS.lat);
			Serial.print(", ");
			Serial.print(GPS.longitude, 4); 
			Serial.println(GPS.lon);
			Serial.print("Location (in degrees, works with Google Maps): ");
			Serial.print(GPS.latitudeDegrees, 4);
			Serial.print(", ");
			Serial.println(GPS.longitudeDegrees, 4);

			Serial.print("Speed (knots): "); 
			Serial.println(GPS.speed);
			Serial.print("Angle: "); 
			Serial.println(GPS.angle);
			Serial.print("Altitude: "); 
			Serial.println(GPS.altitude);
			Serial.print("Satellites: "); 
			Serial.println((int)GPS.satellites);
		}
	}

	GPS.read();							// Receive output from the GPS
	if (GPS.newNMEAreceived())			// If a sentence is received, we can check the checksum
	{	// We try to parse the new output
		if (!GPS.parse(GPS.lastNMEA()))	return;
	}
}
