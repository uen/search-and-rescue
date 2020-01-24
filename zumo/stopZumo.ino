#include <Arduino.h>
#include <Zumo32U4.h>

void stopZumo(){
    digitalWrite(13, LOW);
    motors.setSpeeds(0, 0);
    // driveZumo(300);

    // delay(4000);
    // driveZumo(-300);
    
    // Serial1.println("state:Idle#");
    // delay(100);
    // Serial1.println("zumo:Stopped.#");
}
