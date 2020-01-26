#include <Arduino.h>
#include <Zumo32U4.h>

// t junction
#define DEGREE_MODIFIER 1 // Remove 10 degrees from the total degrees to turn. As the gyro is inaccurate
#define TURN_SPEED 100

void turnZumo(int degrees){
    turnZumo(degrees, false);
}

void turnZumo(int degrees, bool doNotStore){
    int directionModifier = -1;
    if(degrees < 0){
        directionModifier = 1;
    }

    degrees = degrees * DEGREE_MODIFIER;

    Serial1.println("zumo: Turning " + String(degrees));
    
    motors.setSpeeds(TURN_SPEED * directionModifier, -TURN_SPEED * directionModifier);
  
    turnSensorReset();


    // TODO FIX BAD CODE 
    if(degrees <= 0){
        int angle = 0;
        do {
            delay(1);
            turnSensorUpdate();
            angle = (((int32_t)turnAngle >> 16) * 360) >> 16;
        } while(angle > degrees);
    }


    if(degrees > 0){
        int angle = 0;
        do {
            delay(1);
            turnSensorUpdate();
            angle = (((int32_t)turnAngle >> 16) * 360) >> 16;
            // delay(100);
            // Serial1.println("zumo:We have currently turned" + String(angle));
        } while(angle <= (degrees));
    }

    if(!doNotStore) recordTurn(degrees);
    delay(100);
    

    motors.setSpeeds(0, 0);
}

void turnZumo(int len, String data){
    String direction = data.substring(0, 1);
    
    if(direction == "R"){
        turnZumo(-90);
    } else
        turnZumo(90);

    // autonomousMode();
    // TODO: 
}
