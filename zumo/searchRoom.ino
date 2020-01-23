#include <Arduino.h>
#include <Zumo32U4.h>

#define SCAN_SIZE 80
#define SCAN_SECTION 20
#define ROOM_SEARCH_DISTANCE 150    
#define ROOM_BACK_DISTANCE 200

bool performScan(){
    proximity.read();
    uint8_t leftSensor = proximity.countsFrontWithLeftLeds();
    uint8_t rightSensor = proximity.countsFrontWithRightLeds();
    uint8_t reading = leftSensor > rightSensor ? leftSensor : rightSensor;

    bool objectDetected = reading > 5;

    Serial1.println(String("zumo:") + String(objectDetected ? "OBJECT DETECTED": "OFF"));

    return objectDetected;
}

void searchRoom(int len, String data){
    // performScan();
    String direction = data.substring(0, 1);
    int angleModifier = -1;
    if(direction == "L")
        angleModifier = 1;

        turnZumo(angleModifier * 90);
        delay(500);
        driveZumo(ROOM_SEARCH_DISTANCE);
        
        
        // no change for both directions
        turnZumo(-SCAN_SIZE / 2);
        delay(100);

        bool objectFound = false;
        for(int i=0; i < SCAN_SIZE; i = i + SCAN_SECTION){
            delay(500);
            turnZumo(SCAN_SECTION); 
            objectFound = performScan();
            if(objectFound){
                buzzer.play(">g32>");
                delay(1000);
                turnZumo(SCAN_SIZE - i  - SCAN_SECTION);
                break;
            }
        }



        turnZumo(-SCAN_SIZE / 2);
        delay(2000);
        driveZumo(-ROOM_BACK_DISTANCE);

        delay(3000);

        turnZumo(angleModifier * -90);
        // turnZumo(SCAN_SIZE);
        // turnZumo(SCAN_SIZE);

    



              
    // motors.setSpeeds(0, 0);

}
