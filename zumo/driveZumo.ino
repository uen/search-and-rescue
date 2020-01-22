#include <Wire.h>
#include <Zumo32U4.h>

#define DRIVE_SPEED 100

void driveZumo(int cm){
    int totalDistance = 0;
    encoders.getCountsAndResetLeft();
    while(1){
        Serial1.println("zumo:Total distance is:" + String(totalDistance));
        delay(500);
        totalDistance =+ encoders.getCountsAndResetLeft();

        motors.setSpeeds(DRIVE_SPEED, DRIVE_SPEED);
        
        if(totalDistance > cm){
            motors.setSpeeds(0, 0);
            break;
        }
    }
    
}