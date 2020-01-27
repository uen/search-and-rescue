#include <Arduino.h>
#include <Zumo32U4.h>

// Stops the zumo
void stopZumo(){
    digitalWrite(13, LOW);
    motors.setSpeeds(0, 0);
}
