#include <Arduino.h>
#include <Zumo32U4.h>

Zumo32U4Buzzer buzzer;
void lineSensorCalibration(){
    Serial1.println("zumo:Beginning calibration...#");
    delay(100);
    Serial1.println("state:Calibrating...#");

    // Set LED to yellow & play sound to indicate we are calibrating
    digitalWrite(13, HIGH);
    buzzer.play(">g32>>c32");

    delay(100);

    // Start motors to run over lines
    // motors.setSpeeds(-100, 100);
    motors.setSpeeds(-300, 300);

    // Calibrate for the next 7 seconds
    unsigned long startTime = millis();
	while(millis() - startTime < 5000) {
		reflectanceSensors.calibrate();

        if(handleCommunication()) return;
	}

    // Stop motors
    motors.setSpeeds(0, 0);

    // Play sound & turn LED off to indicate calibration is complete
    Serial1.println("zumo:Calibration complete.#");
    delay(100);
    Serial1.println("state:Idle#");
    
    digitalWrite(13, LOW);
    buzzer.play("L16 cdegreg4");

    

    
    
}
