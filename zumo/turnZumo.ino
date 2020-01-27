#include <Arduino.h>
#include <Zumo32U4.h>


#define DEGREE_MODIFIER 1 // * by the total degrees to turn. As the gyro is inaccurate it sometimes needs adjusting
#define TURN_SPEED 70

void turnZumo(int degrees){
    turnZumo(degrees, false);
}

void turnZumo(int degrees, bool doNotStore){
    // Turn the right way
    int directionModifier = -1;
    if(degrees < 0){
        directionModifier = 1;
    }

    degrees = degrees * DEGREE_MODIFIER;    
    motors.setSpeeds(TURN_SPEED * directionModifier, -TURN_SPEED * directionModifier);

    // Reset turn sensors
    turnSensorReset();

    // Turn until we have gone far enough
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
        } while(angle <= (degrees));
    }

    // Record the turn for the path home
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
}