//#include <Zumo32U4.h>

#include <Wire.h>
#include <ZumoShield.h>

#define THROTTLE_PIN   4 // throttle channel from RC receiver
#define STEERING_PIN   5 // steering channel from RC receiver
#define LED_PIN       13 // user LED pin

#define MAX_SPEED             400 // max motor speed
#define PULSE_WIDTH_DEADBAND   25 // pulse width difference from 1500 us (microseconds) to ignore (to compensate for control centering offset)
#define PULSE_WIDTH_RANGE     350 // pulse width difference from 1500 us to be treated as full scale input (for example, a value of 350 means

// Opcodes:
#define STOP 0x31   // Stop
#define BEGIN_AUTONOMOUS 0x32 // Begin autonomous mode
#define SET_SPEEDS 0x33   // Set both motor speeds

#define SERIAL_DELAY 10

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);

  Serial.println("Initialized...");
  Serial1.println("init");

  
    ZumoMotors::setSpeeds(50, 50);


}

void handleData(char commandByte, int len, String data){
   if(commandByte == SET_SPEEDS){
            // get first 4 bytes of data
            Serial.println("setting both speeds");
            String leftSpeedTrimmed = data.substring(0, 3);
            String rightSpeedTrimmed = data.substring(3);
   
            int leftSpeed = leftSpeedTrimmed.toInt();
            int rightSpeed = rightSpeedTrimmed.toInt();
            
            Serial.print("(");
            Serial.print(len-14);
            Serial.print(",");
            Serial.print(leftSpeedTrimmed);
            Serial.print(",");
            Serial.print(rightSpeedTrimmed);
            Serial.print(",");
            Serial.print(leftSpeed);
            Serial.print(",");
            Serial.print(rightSpeed);
            Serial.print(")\n");          


              
            ZumoMotors::setSpeeds(leftSpeed, rightSpeed);
          }

     if(commandByte == BEGIN_AUTONOMOUS){
        ZumoMotors::setSpeeds(100, 100);
     
     }
}

int left_speed, right_speed;
void loop() {
  // put your main code here, to run repeatedly:
  //int throttle = pulseIn(THROTTLE_PIN, HIGH);
  //int steering = pulseIn(STEERING_PIN, HIGH);
//
//  

     
     
     if(Serial1.available()){
      if(Serial1.read() == 0x7E){ // Start of frame
     
         
        // Read the length of data
        int len = 0;
        for (int i = 0; i < 2; i++){
          int l = Serial1.read(); 
          len = len + l;

          delay(SERIAL_DELAY);
        }

        // It should never be 0, indicates communication failure
        if(len > 0){ 
      
          
          // Skip the next 14 bytes, I don't know what they do
          // Probably packet type or something
          for (int i=0; i < 14; i++){
            Serial1.read();
            delay(SERIAL_DELAY);
          }
          
          // I decided the first byte of any data sent will be the command.
          char commandByte = Serial1.read();
  
  
          // Read the rest of the data
          String data = "";
          for (int i = 0; i < len - 14 - 1; i++) {
            delay(SERIAL_DELAY);
            char dataByte = Serial1.read();
            
            data = data + dataByte;
  //          strcat(data, commandByte);
          }
  
          
          String a = "0";

          Serial.print("command is:");
          Serial.print(commandByte);
          Serial.print("\n");
  
          handleData(commandByte, len, data);
       }
          
   }
       
//     Serial.print("\nSpeeds:");
//     Serial.print(left_speed);
//     Serial.print(",");
//     
//     Serial.print(right_speed);

// 
     }

    delay(10);
     
}
