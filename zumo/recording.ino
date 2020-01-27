#include <Arduino.h>
#include <Zumo32U4.h>


struct Step {
    String command;
    int value;
};

bool isRecording = false;



Step pathRecord[20];
int pathRecordLength = 0;

void startRecording(int len, String data){
    String direction = data.substring(0, 1);

    if(direction == "R") turnZumo(-90);
    else turnZumo(90);

    pathRecord[pathRecordLength - 1].value = pathRecord[pathRecordLength - 1].value * -1; // Invert the turn as we will be facing the other way.


    delay(100);
    isRecording = true;
    autonomousMode();
     

    //haha:  autotonomousMode();
    // Serial1.println("zumo: Started recording. autonomous mode activated.");
    delay(500);
    
    // go into automous mode
    // every movement
}


Step tJunctionRecord[10];
int tJunctionLength = 0;
int firstTJunctionLength = 0;



int angleReturnTJunction = 0;

bool isReturningHome = false;

// Step invertStep(Step step){
//     if(step.command == "MOVE")
//         return step;
// }
void stopRecording(){
    turnZumo(90);
    turnZumo(90);

    // tJunctionLength++;

    // TODO: IF SOMETHING IN ROOM
    // OKAY SO REVERSE THE TURN DIRECTION AS IT WILL BE FACING THE OTHER WAY.
    bool foundSomething = false;
    for(int i = 0; i < tJunctionLength-1; i++){
        if(tJunctionRecord[i].command == "ROOM"){
            foundSomething = true;
            break;
        }
    }

    if(foundSomething){
        // Serial1.println("We found something so lets add it :)");
        delay(500);
        
        // Invert the first turn as the zumo will be on thr other side
        pathRecord[pathRecordLength-1].value = pathRecord[pathRecordLength-1].value * -1; 
        
    }
    // Serial1.println("zumo: The path record last thing is-  (should be turn right) - " + String(pathRecord[pathRecordLength-1].command) + "with the value" + String(pathRecord[pathRecordLength-1].value));
    delay(100);
    // Serial1.println("zumo: first command for tjunction is "+String(tJunctionRecord[0].command)+" t junction length is:" + String(tJunctionLength));
    // SEARCHROOM -1 IS LEFT 1 IS RIGHT 0 IS not
    delay(100);


    delay(500);

    // Retrace out t junction move
    for(int i = tJunctionLength-1; i >= 0; i--){
        if(tJunctionRecord[i].command == "MOVE"){
            delay(500);
            // Serial1.println("retracing t junction move. Next command is MOVE. Moving units - " + String(tJunctionRecord[i].value));
            autonomousMode(tJunctionRecord[i].value, true);

            // Record steps to go back to center after re-searching room.
            if(foundSomething){
                pathRecord[pathRecordLength] = Step{tJunctionRecord[i].command, tJunctionRecord[i].value};
                pathRecordLength++;
            }
        }

    }

    
    // Serial1.println("zumo: COMPLETED THE T JUNCTION STEPS. MOVING ONTO THE REGULAR PATH STEPS.");
    // Add the steps we recorded to the main path record.
    // TODO: ONLY DO THIS IF IT HAS A PERSON FOUND

    // Add all move commands on the way back
    // for(int i = tJunctionLength-1; i >= 0; i--){
    //     if(tJunctionRecord[i].command == "MOVE"){
    //         pathRecord[pathRecordLength] = Step{tJunctionRecord[i].command, tJunctionRecord[i].value};
    //         pathRecordLength++;
    //     }
    // }


    pathRecordLength++;
    if(foundSomething){
        for(int i = tJunctionLength-1; i >= 0; i--){
            // if(tJunctionRecord[i].command == "TURN")
                // tJunctionRecord[i].value = tJunctionRecord[tJunctionLength + i].value * -1;


            delay(500);
            int value = tJunctionRecord[i].value;
            if(tJunctionRecord[i].command == "ROOM"){
                value = value * -1;
            }
            
            pathRecord[pathRecordLength] = Step{tJunctionRecord[i].command, value};
            pathRecordLength++;

            // Serial1.println("zumo: Adding step to pathRecord from tJunctionRecord. " + String(tJunctionRecord[i].command) + "" + String(tJunctionRecord[i].value));
        }
    }

    // pathRecord[pathRecordLength] = Step{tJunctionRecord[0].command, tJunctionRecord[0].value};

    // doesnt come back after turning 90 degrees

    // If we found a person in the room, we can now add everything to the other array. - but this wont work :()



    tJunctionLength = 0;

    // We should be at the middle of the junction atm.
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

    Serial1.println(String(String("Recorded distance of ") + String((distance))));
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
    // Serial1.println(String(String("zumo: JUST TURNED ") + String((degrees))));

    Step turn = {String("TURN"), degrees};

    if(isRecording){
        tJunctionRecord[tJunctionLength] = turn; 
        tJunctionLength++;
    } else {
        pathRecord[pathRecordLength] = turn;
        pathRecordLength++;
    }    

}

int roomCount = 0;
bool shouldPlayReturnSong = false;

void recordRoomSearch(int direction){

    if(isReturningHome){
        Serial1.println("zumo: Someone still present in room "+String(roomCount)+".");
        shouldPlayReturnSong = true;
        roomCount--;
    } else {
        roomCount++;
        Serial1.println("zumo: Found someone in room "+String(roomCount));
    }

    if(isReturningHome) return;

    Step step = {String("ROOM"), direction};
  
    if(isRecording){
        tJunctionRecord[tJunctionLength] = step; 
        tJunctionLength++;
    } else {
        pathRecord[pathRecordLength] = step;
        pathRecordLength++;
    }

}


void playReturnSound(){
        if(shouldPlayReturnSong){
            if(!buzzer.isPlaying()){
                buzzer.play("!T240 L8 MS >e<eb>c>d>e16>d16>cba<aa>c>e<a>d>cb<g+b>c>d<e>e<e>c<aarae<bc d>d<d>f>ad16d16>g>f>e<cr>c>e<g>d>cb<g+b>c>d<e>e<e>c<aa<aa");
            }
        }
}

void returnHome(){
    isReturningHome = true;
    turnZumo(90);
    delay(1000);
    turnZumo(90);
    delay(1000);

    digitalWrite(13, HIGH);
    for(int i = tJunctionLength-1; i >= 0; i--){
        if(tJunctionRecord[i].command == "MOVE"){
            // Serial1.println("Next command is MOVE. Moving units - " + String(pathRecord[i].value));
            autonomousMode(tJunctionRecord[i].value,true);
        } else if(tJunctionRecord[i].command == "TURN"){
            // Serial1.println("Next command is TURN. Turning units - " + String(-pathRecord[i].value));
            turnZumo(-tJunctionRecord[i].value);
        } else if(tJunctionRecord[i].command == "ROOM"){
            // Serial1.println("Next command is ROOM. Going into room");
            searchRoom(tJunctionRecord[i].value * -1);
            delay(500);
        }


        playReturnSound();
        delay(1000);

        // driveZumo(pathRecord[i].distance);
        // delay(3000);
    }


    for(int i = pathRecordLength-1; i >= 0; i--){
        if(pathRecord[i].command == "MOVE"){
            // Serial1.println("Next command is MOVE. Moving units - " + String(pathRecord[i].value));
            autonomousMode(pathRecord[i].value,true);
        } else if(pathRecord[i].command == "TURN"){
            // Serial1.println("Next command is TURN. Turning units - " + String(-pathRecord[i].value));
            turnZumo(-pathRecord[i].value);
        } else if(pathRecord[i].command == "ROOM"){
            // Serial1.println("Next command is ROOM. Going into room");
            searchRoom(pathRecord[i].value * -1);
            delay(500);
        }
        delay(1000);

        playReturnSound();

        // driveZumo(pathRecord[i].distance);
        // delay(3000);
        
    }

    digitalWrite(13, LOW);    

    buzzer.play(">g32>>c32");
}
