#include <Arduino.h>
#include <Zumo32U4.h>

// Struct to store a step in such as MOVE, TURN, ROOM
struct Step {
    String command;
    int value;
};


// This is true when a t junction first starts
bool isRecording = false;

// Array to store the total path into
Step pathRecord[20];
int pathRecordLength = 0;

// Called when you press 'T Junction Left/Right'
void startRecording(int len, String data){
    // First turn the zumo left/right depending on selection
    String direction = data.substring(0, 1);
    if(direction == "R") turnZumo(-90);
    else turnZumo(90);

    // Invert the turn for coming back later.
    pathRecord[pathRecordLength - 1].value = pathRecord[pathRecordLength - 1].value * -1; 


    delay(100);
    isRecording = true;
    
    // Go forward until told to stop or we hit a wall.
    autonomousMode();
     
    delay(500);
}


// Array to store the steps on a specific t junction
Step tJunctionRecord[10];
int tJunctionLength = 0;

// If we're returning home or not
bool isReturningHome = false;

// Called when the zumo is told to go and search the other side of the t junction
void stopRecording(){
    // Turn around
    turnZumo(90);
    turnZumo(90);

    // Check if we found anything in the t junction
    bool foundSomething = false;
    for(int i = 0; i < tJunctionLength-1; i++){
        if(tJunctionRecord[i].command == "ROOM"){
            foundSomething = true;
            break;
        }
    }

    if(foundSomething){
        delay(500);
        // If we did find something, invert the first turn we did onto the t junction as we will be facing the other way        
        pathRecord[pathRecordLength-1].value = pathRecord[pathRecordLength-1].value * -1; 
        
    }

    delay(1000);

    // Refollow the path we took to get into the t junction
    for(int i = tJunctionLength-1; i >= 0; i--){
        // Only respond to Move commands, we do not want to search rooms as we will when we return home
        if(tJunctionRecord[i].command == "MOVE"){
            delay(500);
            autonomousMode(tJunctionRecord[i].value, true);

            // If we found something, add the step to the final pathRecord
            if(foundSomething){
                pathRecord[pathRecordLength] = Step{tJunctionRecord[i].command, tJunctionRecord[i].value};
                pathRecordLength++;
            }
        }

    }

    pathRecordLength++;
    if(foundSomething){
        for(int i = tJunctionLength-1; i >= 0; i--){
            delay(500);

            // Invert the t junction room direction so we can visit it later if we found something
            int value = tJunctionRecord[i].value;
            if(tJunctionRecord[i].command == "ROOM"){
                value = value * -1;
            }
            
            pathRecord[pathRecordLength] = Step{tJunctionRecord[i].command, value};
            pathRecordLength++;
        }
    }

    // Reset the t junction array for the next side of the t junction
    tJunctionLength = 0;

    // We are at the middle of the t junction facing the other side. Go forward to the other side
    autonomousMode();
}


// Called to record an autonomous mode movement
void recordMovement(int distance){
    // Don't record if we're returning home
    if(isReturningHome) return;
    Step movement = {String("MOVE"), distance};

    Serial1.println(String(String("Recorded distance of ") + String((distance))));

    // If recording add it ot the t junction, if not add it to the regular path record
    if(isRecording){
        tJunctionRecord[tJunctionLength] = movement; 
        tJunctionLength++;
    } else {
        pathRecord[pathRecordLength] = movement;
        pathRecordLength++;
    }
};

// Record when a turn happens
void recordTurn(int degrees){
    // Don't record if we're Turning
    if(isReturningHome) return;

    Step turn = {String("TURN"), degrees};

    if(isRecording){
        tJunctionRecord[tJunctionLength] = turn; 
        tJunctionLength++;
    } else {
        pathRecord[pathRecordLength] = turn;
        pathRecordLength++;
    }    

}

// Total count of rooms
int roomCount = 0;

// If we should play the return song or not
bool shouldPlayReturnSong = false;

void recordRoomSearch(int direction){
    // Only called if someone is found
    if(isReturningHome){
        Serial1.println("zumo: Someone still present in room "+String(roomCount)+".");
        shouldPlayReturnSong = true;
        roomCount--;
    } else {
        roomCount++;
        Serial1.println("zumo: Found someone in room "+String(roomCount));
    }

    if(isReturningHome) return;

    // Construct step to add to path
    Step step = {String("ROOM"), direction};
  
    if(isRecording){
        tJunctionRecord[tJunctionLength] = step; 
        tJunctionLength++;
    } else {
        pathRecord[pathRecordLength] = step;
        pathRecordLength++;
    }

}

// If the buzzer is not playing and we are returning home, play the follow me sound
void playReturnSound(){
    if(shouldPlayReturnSong){
        if(!buzzer.isPlaying()){
            buzzer.play("!T240 L8 MS >e<eb>c>d>e16>d16>cba<aa>c>e<a>d>cb<g+b>c>d<e>e<e>c<aarae<bc d>d<d>f>ad16d16>g>f>e<cr>c>e<g>d>cb<g+b>c>d<e>e<e>c<aa<aa");
        }
    }
}

void returnHome(){
    isReturningHome = true;
    
    // Turn zumo around
    turnZumo(90);
    delay(1000);
    turnZumo(90);
    delay(1000);

    // Turn LED on
    digitalWrite(13, HIGH);

    // Get to the center of the T junction, including rooms
    for(int i = tJunctionLength-1; i >= 0; i--){
        if(tJunctionRecord[i].command == "MOVE"){
            autonomousMode(tJunctionRecord[i].value,true);
        } else if(tJunctionRecord[i].command == "TURN"){
            turnZumo(-tJunctionRecord[i].value);
        } else if(tJunctionRecord[i].command == "ROOM"){
            // Serial1.println("Next command is ROOM. Going into room");
            searchRoom(tJunctionRecord[i].value * -1);
            delay(500);
        }


        playReturnSound();
        delay(1000);
    }

    // We are at the center of the t junction, if there is soemone at the other side of the t junction
    // that will be in the path record.
    // Follow the path record home
    for(int i = pathRecordLength-1; i >= 0; i--){
        if(pathRecord[i].command == "MOVE"){
            autonomousMode(pathRecord[i].value,true);
        } else if(pathRecord[i].command == "TURN"){
            turnZumo(-pathRecord[i].value);
        } else if(pathRecord[i].command == "ROOM"){
            searchRoom(pathRecord[i].value * -1);
            delay(500);
        }
        delay(1000);

        playReturnSound();        
    }

    // We're home, turn the LED off and play success buzzer
    digitalWrite(13, LOW);    
    buzzer.play(">g32>>c32");
}
