#include <Arduino.h>
#include <Zumo32U4.h>

void manualControl(int len, String data){
    String leftSpeedTrimmed = data.substring(0, 3);
    String rightSpeedTrimmed = data.substring(3);
   
    int leftSpeed = leftSpeedTrimmed.toInt();
    int rightSpeed = rightSpeedTrimmed.toInt();

              
    motors.setSpeeds(leftSpeed, rightSpeed);

}
