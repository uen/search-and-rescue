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

    unsigned int position;
    unsigned int sensors[6];
    int offset_from_center;
    int power_difference;

    // if(distance > 0)
        // Serial1.println("Moving forward with distance of:" + String(distance));



    int totalDistance = 0;
    // reset the encoders
    encoders.getCountsAndResetLeft();
    
    while(1){
        int currentDistance = encoders.getCountsAndResetLeft();
        totalDistance += currentDistance;

        if(distance > 0 && totalDistance >= distance){
            motors.setSpeeds(0,0);
            delay(500);
            return;
        }
        // delay(100);
        // Serial1.println(String("zumo:Total distance so far is ") + String(totalDistance) + String(" as we moved ") + String(currentDistance));
            

        if(encoders.checkErrorLeft() || encoders.checkErrorRight()){
            Serial1.println("zumo:Encoder error detected");
            motors.setSpeeds(0,0);
            buzzer.play(">g32>>g32>>g32>");
            


        // An error occurred on the left encoder channel.
        // Display it on the LCD for the next 10 iterations and
        // also beep.
        
            return;
        }



        reflectanceSensors.readLine(sensors);
        if(sensors[1] > FRONT_SENSOR_THRESHOLD){
            motors.setSpeeds(0,0);
            Serial1.println("Hit a wall.");
            delay(100);


            break;
        } else if(sensors[0] > SENSOR_THRESHOLD){
            motors.setSpeeds(TURN_MODIFIER, -TURN_MODIFIER);
        } else if(sensors[2] > SENSOR_THRESHOLD){
            motors.setSpeeds(-TURN_MODIFIER, TURN_MODIFIER);
        } else {
            motors.setSpeeds(SPEED, SPEED);
        }

        // ();

        if(handleCommunication()) break;
    }
    
    // once done, call the encoder read distance thing.
    delay(100);
    
    if(!dontRecord) recordMovement(totalDistance);

    digitalWrite(13, LOW);

}
