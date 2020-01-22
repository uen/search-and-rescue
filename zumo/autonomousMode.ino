#include <Arduino.h>
#include <Zumo32U4.h>

#define SENSOR_THRESHOLD 600

#define FRONT_SENSOR_THRESHOLD 150
#define SPEED 100
#define TURN_MODIFIER 200
void autonomousMode(){
    Serial1.println("zumo:Autonomous mode activated...#");
    delay(100);
    Serial1.println("state:Autonomous...#");

    // Set LED to yellow to indicate we are in autonomous mode
    digitalWrite(13, HIGH);

    unsigned int position;
    unsigned int sensors[6];
    int offset_from_center;
    int power_difference;
    
    while(1){
        reflectanceSensors.readLine(sensors);
        if(sensors[1] > FRONT_SENSOR_THRESHOLD){
            motors.setSpeeds(0,0);
            Serial1.println("zumo:Hit a wall. Please indicate direction#");
            delay(100);

            Serial1.println("state:Stopped");

            break;
        } else if(sensors[0] > SENSOR_THRESHOLD){
            motors.setSpeeds(TURN_MODIFIER, -TURN_MODIFIER);
        } else if(sensors[2] > SENSOR_THRESHOLD){
            motors.setSpeeds(-TURN_MODIFIER, TURN_MODIFIER);
        } else {
            motors.setSpeeds(SPEED, SPEED);
        }

        if(handleCommunication()) return;
    }

    digitalWrite(13, LOW);

}
