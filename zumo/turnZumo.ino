#include <Arduino.h>
#include <Zumo32U4.h>


#define TURN_SPEED 200
void turnZumo(int degrees){    
    int directionModifier = -1;
    if(degrees < 0){
        directionModifier = 1;
    }
    
    motors.setSpeeds(TURN_SPEED * directionModifier, -TURN_SPEED * directionModifier);
  
    turnSensorReset();


// TODO FIX BAD CODE 
    if(degrees <= 0){
        int angle = 0;
        do {
            delay(1);
            turnSensorUpdate();
            angle = (((int32_t)turnAngle >> 16) * 360) >> 16;
        } while(angle > (degrees));
    }


    if(degrees > 0){
        int angle = 0;
        do {
            delay(1);
            turnSensorUpdate();
            angle = (((int32_t)turnAngle >> 16) * 360) >> 16;
        } while(angle <= (degrees));
    }

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
