#include <Wire.h>
#include <Zumo32U4.h>
#include "TurnSensor.h"

//#include "manualControl.cpp"
#define THROTTLE_PIN   4 // throttle channel from RC receiver
#define STEERING_PIN   5 // steering channel from RC receiver
#define LED_PIN       13 // user LED pin

#define MAX_SPEED             400 // max motor speed
#define PULSE_WIDTH_DEADBAND   25 // pulse width difference from 1500 us (microseconds) to ignore (to compensate for control centering offset)
#define PULSE_WIDTH_RANGE     350 // pulse width difference from 1500 us to be treated as full scale input (for example, a value of 350 means

// Opcodes:
#define STOP 0x31 // Stop
#define BEGIN_AUTONOMOUS 0x32 // Begin autonomous mode
#define SET_SPEEDS 0x33   // Set both motor speeds
#define CALIBRATION 0x34 // Calibrate line sensors
#define TURN 0x35
#define SEARCH_ROOM 0x36

#define SERIAL_DELAY 10


L3G gyro;
Zumo32U4LCD lcd;
Zumo32U4Encoders encoders;

Zumo32U4ProximitySensors proximity;






Zumo32U4LineSensors reflectanceSensors;
Zumo32U4Motors motors;


void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);
	Serial1.begin(9600);

	
	turnSensorSetup();

	Serial.println("Initialized...");

	// Yellow LED
	pinMode(13, OUTPUT);

	// Initialize line sensors
	reflectanceSensors.initThreeSensors();

	proximity.initFrontSensor();

	Serial1.println("zumo:Initialized.");
	delay(100);
	Serial1.println("state:Idle");



    



}

void handleData(char commandByte, int len, String data){
	 if(commandByte == SET_SPEEDS){
		 manualControl(len, data);
					}
		

		 if(commandByte == BEGIN_AUTONOMOUS){
				autonomousMode();
		 }

		 else if(commandByte == CALIBRATION){
			 lineSensorCalibration();
		 } else if(commandByte == TURN){
			 turnZumo(len, data);
		 } else if(commandByte == SEARCH_ROOM){
			 searchRoom(len, data);
		 }

		 else if(commandByte == STOP){
			 stopZumo();
		 }
}

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
			 
//     Serial.print("\nSpeeds:");
//     Serial.print(left_speed);
//     Serial.print(",");
//     
//     Serial.print(right_speed);

// 
		 }


		 return false;


}

int left_speed, right_speed;
void loop() {
	// put your main code here, to run repeatedly:
	//int throttle = pulseIn(THROTTLE_PIN, HIGH);
	//int steering = pulseIn(STEERING_PIN, HIGH);
//
//  

		 
		handleCommunication();
		delay(10);
		 
}