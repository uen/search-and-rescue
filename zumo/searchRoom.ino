#include <Arduino.h>
#include <Zumo32U4.h>

#define SCAN_SIZE 80
#define SCAN_SECTION 20
#define ROOM_SEARCH_DISTANCE 250
#define ROOM_BACK_DISTANCE 220

bool performScan(){
    proximity.read();
    uint8_t leftSensor = proximity.countsFrontWithLeftLeds();
    uint8_t rightSensor = proximity.countsFrontWithRightLeds();
    uint8_t reading = leftSensor > rightSensor ? leftSensor : rightSensor;

    bool objectDetected = reading > 5;

    // Serial1.println(String("zumo:") + String(objectDetected ? "OBJECT DETECTED": "OFF"));

    return objectDetected;
}

void searchRoom(int len, String data){
    String direction = data.substring(0, 1);
    searchRoom(direction == "L" ? -1 : 1);
}

void searchRoom(int direction){
    // performScan();


    int angleModifier = -1;
    if(direction == -1)
        angleModifier = 1;

    turnZumo(angleModifier * 90, true);
    delay(500);
    driveZumo(ROOM_SEARCH_DISTANCE);
    
    
    // no change for both directions
    turnZumo(-SCAN_SIZE / 2, true);
    delay(100);


    // Serial1.println("SEARCH ROOMSEARCH ROOM " + String(direction));

    bool objectFound = false;
    for(int i=0; i < SCAN_SIZE; i = i + SCAN_SECTION){
        objectFound = performScan();
        if(objectFound){
            buzzer.play(">g32>");
            delay(500);
            turnZumo(SCAN_SIZE - i  - SCAN_SECTION, true);
            break;
        }

        delay(500);
        turnZumo(SCAN_SECTION, true); 
        
    }



    turnZumo((-SCAN_SIZE / 2) - (objectFound ? 0 : SCAN_SECTION), true);
    delay(300);
    driveZumo(-ROOM_BACK_DISTANCE);

    delay(300);

    turnZumo(angleModifier * -90, true);

    if(objectFound){
        recordRoomSearch(direction);
    }
        // turnZumo(SCAN_SIZE);
        // turnZumo(SCAN_SIZE);

    



              
    // motors.setSpeeds(0, 0);

}
