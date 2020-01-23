#include <Wire.h>
#include <Zumo32U4.h>

#define DRIVE_SPEED 100

void driveZumo(int cm){
    int totalDistance = 0;
    int directionModifier = 1;
    if(cm < 0){
        directionModifier = -1;
    }

    encoders.getCountsAndResetLeft();

    while(1){
        totalDistance =+ encoders.getCountsAndResetLeft();
        delay(250);
        Serial1.println(String("zumo:total distance travelled is: ") + String(totalDistance));
        motors.setSpeeds(DRIVE_SPEED * directionModifier, DRIVE_SPEED * directionModifier);
            /*
                total distance has to be less than distance to travel amount
                to stop the zumo.
             */
            if (
                (directionModifier == -1 && totalDistance < cm) ||
                (directionModifier == 1 && totalDistance > cm)
            ){
                motors.setSpeeds(0, 0);
                break;
        }
    }
    
}