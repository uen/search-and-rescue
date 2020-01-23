#include <Arduino.h>

struct Step {
    String command;
    int distance;
    char direction;
};

bool isRecording = false;

void startRecording(){
    isRecording = true;
    //haha:  autotonomousMode();
    autonomousMode();
    // go into automous mode
    // every movement
}

Step tJunctionRecord[30];
int tJunctionLength = 0;

Step pathRecord[30];
int pathRecordLength = 0;


void recordMovement(int distance){
    Step movement = {String("MOVE"), distance, 'F'};
    Serial.println("moved:");
    Serial.println(distance);
    Serial1.println(String(String("zumo: JUST MOVED ") + String((distance))));
    if(isRecording){
        tJunctionRecord[tJunctionLength] = movement; 
        tJunctionLength++;
    } else {
        pathRecord[pathRecordLength] = movement;
        pathRecordLength++;
    }
};

void getPathHome(){
    for(int i = pathRecordLength-1; i >= 0; i--){
        Serial1.println(String("zumo:The distance is ") + String(pathRecord[i].distance) + String(", the command is: ") +  String(pathRecord[i].command));
        driveZumo(-pathRecord[i].distance);
        delay(3000);
    }
}

void stopRecording(){

}