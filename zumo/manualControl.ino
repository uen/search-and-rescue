#include <Arduino.h>
#include <Zumo32U4.h>

// Manually control the Zumo with speeds given from serial
void manualControl(int len, String data){
    // Format is 100200 for 100 left 200 right
    String leftSpeedTrimmed = data.substring(0, 3);
    String rightSpeedTrimmed = data.substring(3);
   
    int leftSpeed = leftSpeedTrimmed.toInt();
    int rightSpeed = rightSpeedTrimmed.toInt();

              
    motors.setSpeeds(leftSpeed, rightSpeed);

}
