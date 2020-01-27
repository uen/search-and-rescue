#include <Wire.h>
#include <Zumo32U4.h>

#define DRIVE_SPEED 100

// Drive Zumo a certain distance (used by rooms)
void driveZumo(int cm){

    int totalDistance = 0;

    // If the distance is negative, go backwards instead of forwards
    int directionModifier = 1;
    if(cm < 0){
        directionModifier = -1;
    }

    // Reset encoders
    encoders.getCountsAndResetLeft();

    while(1){
        totalDistance += encoders.getCountsAndResetLeft();

        motors.setSpeeds(DRIVE_SPEED * directionModifier, DRIVE_SPEED * directionModifier);
        
        if ((directionModifier == -1 && totalDistance < cm) ||
            (directionModifier == 1 && totalDistance > cm)){
            // If we have gone past the total distance, stop the zumo
            motors.setSpeeds(0, 0);
            break;
        }
    }
    
}