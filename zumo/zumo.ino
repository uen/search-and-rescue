#include <Wire.h>
#include <Zumo32U4.h>
#include "TurnSensor.h"

// Opcodes from serial:
#define STOP 0x31 // Stop
#define BEGIN_AUTONOMOUS 0x32 // Begin autonomous mode
#define SET_SPEEDS 0x33   // Set both motor speeds
#define CALIBRATION 0x34 // Calibrate line sensors
#define TURN 0x35 // Turn left/right
#define SEARCH_ROOM 0x36 // Search a room
#define START_RECORD 0x37 // Record a t junction and turn left/right
#define STOP_RECORD 0x38 // Stop recording and return to search the other side
#define RETURN_HOME 0x39 // Go home

#define SERIAL_DELAY 10


L3G gyro;
Zumo32U4Buzzer buzzer;
Zumo32U4Encoders encoders;
Zumo32U4ProximitySensors proximity;
Zumo32U4LineSensors reflectanceSensors;
Zumo32U4Motors motors;


void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);
	Serial1.begin(9600);

	turnSensorSetup();

	// Yellow LED
	pinMode(13, OUTPUT);

	// Initialize line sensors
	reflectanceSensors.initThreeSensors();

	// Initialize front sensors
	proximity.initFrontSensor();

	Serial1.println("Initialized.");
	delay(100);
}

void handleData(char commandByte, int len, String data){
	// Handle when data is sent through serial

	 if(commandByte == SET_SPEEDS){
		 manualControl(len, data);
	} else if(commandByte == BEGIN_AUTONOMOUS){
		autonomousMode();
	} else if(commandByte == CALIBRATION){
		lineSensorCalibration();
	} else if(commandByte == TURN){
		turnZumo(len, data);
	} else if(commandByte == SEARCH_ROOM){
		searchRoom(len, data);
	} else if(commandByte == START_RECORD){
		startRecording(len, data);
	} else if(commandByte == STOP_RECORD){
		stopRecording();
	} else if(commandByte == RETURN_HOME){
		returnHome();
	} else if(commandByte == STOP){
		stopZumo();
	}
}

// Handle communication from serial using x-bee frame format
bool handleCommunication(){
	if(Serial1.available()){
		if(Serial1.read() == 0x7E){ // Start of frame	 
			// read the length of data (first 4 bytes)
			int len = 0;
			for (int i = 0; i < 2; i++){
				int l = Serial1.read(); 
				len = len + l;
				delay(SERIAL_DELAY);
			}

			// length should never be 0, indicates a corrupt frame
			if(len > 0){ 
				// skip the next 14 bytes, I don't know what they do
				// probably packet type or something
				for (int i=0; i < 14; i++){
					Serial1.read();
					delay(SERIAL_DELAY);
				}
					
				// first byte of any data sent will be the command.
				char commandByte = Serial1.read();

				// Read the rest of the data, according to the length sent earlier
				String data = "";
				for (int i = 0; i < len - 14 - 1; i++) {
					delay(SERIAL_DELAY);
					char dataByte = Serial1.read();						
					data = data + dataByte;
				}	
			
				handleData(commandByte, len, data);

				return true;
			}
					
		}
	}

	return false;
}

int left_speed, right_speed;
void loop() {
	handleCommunication();
	delay(10);	 
}
