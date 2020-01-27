#include <Arduino.h>
#include <Zumo32U4.h>

#define SCAN_SIZE 80
#define SCAN_SECTION 20
#define ROOM_SEARCH_DISTANCE 250
#define ROOM_BACK_DISTANCE 220


// Check if objects detected
bool performScan(){
    proximity.read();
    uint8_t leftSensor = proximity.countsFrontWithLeftLeds();
    uint8_t rightSensor = proximity.countsFrontWithRightLeds();
    uint8_t reading = leftSensor > rightSensor ? leftSensor : rightSensor;

    bool objectDetected = reading > 5;

    return objectDetected;
}

// Search room (from serial)
void searchRoom(int len, String data){
    String direction = data.substring(0, 1);
    searchRoom(direction == "L" ? -1 : 1);
}


void searchRoom(int direction){
    // Turn the right way
    int angleModifier = -1;
    if(direction == -1)
        angleModifier = 1;


    turnZumo(angleModifier * 90, true);
    delay(500);

    // Drive forward into the room`
    driveZumo(ROOM_SEARCH_DISTANCE);

    // Turn half of the total scan size    
    turnZumo(-SCAN_SIZE / 2, true);
    delay(100);


    // Slowly turn the full scna size, scanning every time
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


    // If we found an object, turn the rest of the way immediately. Or turn back to middle
    turnZumo((-SCAN_SIZE / 2) - (objectFound ? 0 : SCAN_SECTION), true);
    delay(300);

    // Drive out of the room
    driveZumo(-ROOM_BACK_DISTANCE);

    delay(300);

    // Turn back to our original position
    turnZumo(angleModifier * -90, true);

    // If we found something, record it.
    if(objectFound){
        recordRoomSearch(direction);
    }
}
