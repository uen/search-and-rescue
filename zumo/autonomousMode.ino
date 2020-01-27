#include <Arduino.h>
#include <Zumo32U4.h>

#define SENSOR_THRESHOLD 600

#define FRONT_SENSOR_THRESHOLD 300
#define SPEED 100
#define TURN_MODIFIER 200

void autonomousMode(){
    return autonomousMode(0);
}

void autonomousMode(bool dontRecord){
    autonomousMode(0, dontRecord);
}

void autonomousMode(int distance, bool dontRecord){

    // Set LED to yellow to indicate we are in autonomous mode
    digitalWrite(13, HIGH);

    // Init sensors
    unsigned int sensors[6];

    // Init the total distance travelled so far
    int totalDistance = 0;

    // Reset the encoders
    encoders.getCountsAndResetLeft();
    
    while(1){
        // Add the distance travelled since last update to the total distance
        int currentDistance = encoders.getCountsAndResetLeft();
        totalDistance += currentDistance;

        // If we have a distance and we have travelled more than it, stop
        if(distance > 0 && totalDistance >= distance){
            motors.setSpeeds(0,0);
            delay(500);
            return;
        }

        // Play the buzzer and stop the motors when an encoder error is deteced
        if(encoders.checkErrorLeft() || encoders.checkErrorRight()){
            Serial1.println("zumo:Encoder error detected");
            motors.setSpeeds(0,0);
            buzzer.play(">g32>>g32>>g32>");        
            return;
        }

        // Read the sensor values
        reflectanceSensors.readLine(sensors);

        if(sensors[1] > FRONT_SENSOR_THRESHOLD){
            // If the front sensor is hit it means we hit a wall

            motors.setSpeeds(0,0);
            Serial1.println("Hit a wall.");
            delay(100);

            break;
        } else if(sensors[0] > SENSOR_THRESHOLD){
            // Left sensor hit, adjust motors
            motors.setSpeeds(TURN_MODIFIER, -TURN_MODIFIER);
        } else if(sensors[2] > SENSOR_THRESHOLD){
            // Right sensor hit, adjust motors
            motors.setSpeeds(-TURN_MODIFIER, TURN_MODIFIER);
        } else {
            // No sensors active, go forward
            motors.setSpeeds(SPEED, SPEED);
        }

        // Check if there's an incoming serial message
        if(handleCommunication()) break;
    }
    
    delay(100);

    // Record the final movement distance to the path  
    if(!dontRecord) recordMovement(totalDistance);

    // Turn the LED Off
    digitalWrite(13, LOW);

}
