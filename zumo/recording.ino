#include <Arduino.h>
#include <Zumo32U4.h>


struct Step {
    String command;
    int value;
};

bool isRecording = false;

void startRecording(int len, String data){
    String direction = data.substring(0, 1);

    if(direction == "R") turnZumo(-90);
    else turnZumo(90);

    delay(100);
    isRecording = true;
    autonomousMode();
     

    //haha:  autotonomousMode();
    Serial1.println("zumo: Started recording. autonomous mode activated.");
    delay(500);
    
    // go into automous mode
    // every movement
}


Step tJunctionRecord[10];
int tJunctionLength = 0;
int firstTJunctionLength = 0;



Step pathRecord[10];
int pathRecordLength = 0;

int angleReturnTJunction = 0;

bool isReturningHome = false;

// Step invertStep(Step step){
//     if(step.command == "MOVE")
//         return step;
// }

void stopRecording(){
    Serial1.println("zumo: stop recording... turning round");

    isReturningHome = true;
    turnZumo(90);
    turnZumo(90);
    isReturningHome = false;

    Serial1.println("zumo: has the array had any steps where they have found anyone in the room??");
    // SEARCHROOM -1 IS LEFT 1 IS RIGHT 0 IS not
    delay(100);
    Serial1.println("zumo: lets assume not. Returning back to t junction. size of thing is - " + String(tJunctionLength));

    for(int i = tJunctionLength-1; i >= 0; i--){
        if(tJunctionRecord[i].command == "MOVE"){
            delay(500);
            Serial1.println("Next command is MOVE. Moving units - " + String(tJunctionRecord[i].value));
            autonomousMode(tJunctionRecord[i].value, true);
        } else if(pathRecord[i].command == "TURN"){
            Serial1.println("Next command is TURN. Turning units - " + String(-pathRecord[i].value));
            turnZumo(-1 * pathRecord[i].value);
        }
    }

    // If we found a person in the room, we can now add everything to the other array. - but this wont work :()

    for(int i = 0; i > tJunctionLength - 1; i++){
        if(tJunctionRecord[i].command == "TURN")
            tJunctionRecord[i].value = tJunctionRecord[tJunctionLength + i].value * -1;
        
        pathRecord[pathRecordLength + i] = tJunctionRecord[i];
    }

    tJunctionLength = 0;
    autonomousMode();



    // We are back in the middle and we are facing 
    
    // check if someones in the room, if they are then add the journey onto the final list of steps.
    // go back to center of t junction
    // autonomously start again and record in the t junction array
    // once at end, turn round. if someone found in room, rescan it, if not ignore the room
    // follow the return journey,
}


void recordMovement(int distance){
    if(isReturningHome) return;
    Step movement = {String("MOVE"), distance};
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

void recordTurn(int degrees){
    if(isReturningHome) return;
    Serial1.println(String(String("zumo: JUST TURNED ") + String((degrees))));

    Step turn = {String("TURN"), degrees};

    if(isRecording){
        tJunctionRecord[tJunctionLength] = turn; 
        tJunctionLength++;
    } else {
        pathRecord[pathRecordLength] = turn;
        pathRecordLength++;
    }    

}

void recordRoomSearch(int direction){
    if(isReturningHome) return;
    Serial1.println("zumo: FOund someone in the room. Recording it. Direction is "+ String(direction));

    Step step = {String("ROOM"), direction};
  
    if(isRecording){
        tJunctionRecord[tJunctionLength] = step; 
        tJunctionLength++;
    } else {
        pathRecord[pathRecordLength] = step;
        pathRecordLength++;
    }

}

void returnHome(){
    isReturningHome = true;
    turnZumo(90);
    delay(1000);
    turnZumo(90);
    delay(1000);
    for(int i = tJunctionLength-1; i >= 0; i--){
        if(tJunctionRecord[i].command == "MOVE"){
            Serial1.println("Next command is MOVE. Moving units - " + String(pathRecord[i].value));
            autonomousMode(tJunctionRecord[i].value,true);
        } else if(tJunctionRecord[i].command == "TURN"){
            Serial1.println("Next command is TURN. Turning units - " + String(-pathRecord[i].value));
            turnZumo(-tJunctionRecord[i].value);
        } else if(tJunctionRecord[i].command == "ROOM"){
            Serial1.println("Next command is ROOM. Going into room");
            searchRoom(tJunctionRecord[i].value * -1);
        }
        // driveZumo(pathRecord[i].distance);
        delay(3000);
    }


    for(int i = pathRecordLength-1; i >= 0; i--){
        if(pathRecord[i].command == "MOVE"){
            Serial1.println("Next command is MOVE. Moving units - " + String(pathRecord[i].value));
            autonomousMode(pathRecord[i].value,true);
        } else if(pathRecord[i].command == "TURN"){
            Serial1.println("Next command is TURN. Turning units - " + String(-pathRecord[i].value));
            turnZumo(-pathRecord[i].value);
        } else if(pathRecord[i].command == "ROOM"){
            Serial1.println("Next command is ROOM. Going into room");
            searchRoom(pathRecord[i].value * -1);
        }
        // driveZumo(pathRecord[i].distance);
        delay(3000);
    }

    buzzer.play(">g32>>c32");
}

