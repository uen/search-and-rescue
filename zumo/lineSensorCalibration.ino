#include <Arduino.h>
#include <Zumo32U4.h>

// Calibrate the line sensors by spinning on the spot
void lineSensorCalibration(){
    Serial1.println("Beginning calibration...");
    delay(100);

    // Set LED to yellow & play sound to indicate we are calibrating
    digitalWrite(13, HIGH);
    buzzer.play(">g32>>c32");

    delay(100);

    // Start motors to run over lines
    // motors.setSpeeds(-100, 100);
    motors.setSpeeds(-200, 200);

    // Calibrate for the next 5 seconds
    unsigned long startTime = millis();
	while(millis() - startTime < 5000) {
		reflectanceSensors.calibrate();

        if(handleCommunication()) return;
	}

    // Stop motors
    motors.setSpeeds(0, 0);

    // Play sound & turn LED off to indicate calibration is complete
    Serial1.println("Calibration complete.");
    
    digitalWrite(13, LOW);
    buzzer.play("L16 cdegreg4");

    

    
    
}
