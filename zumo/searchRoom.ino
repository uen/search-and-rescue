#include <Arduino.h>
#include <Zumo32U4.h>

#define SCAN_SIZE 60
#define SCAN_SECTION 10
#define ROOM_SEARCH_DISTANCE 100

void searchRoom(int len, String data){
    String direction = data.substring(0, 1);

    if(direction == "R"){
        turnZumo(-90);
        delay(500);
        driveZumo(ROOM_SEARCH_DISTANCE);
        turnZumo(-SCAN_SIZE / 2);
        for(int i=SCAN_SECTION; i < SCAN_SIZE; i = i + SCAN_SECTION){
            delay(200);
            turnZumo(SCAN_SECTION);
            delay(200);

        }

        turnZumo((-SCAN_SIZE) / 2);
        // turnZumo(SCAN_SIZE);
        // turnZumo(SCAN_SIZE);
    } else
        turnZumo(90);

              
    motors.setSpeeds(0, 0);

}
