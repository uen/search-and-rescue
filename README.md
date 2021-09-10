# Search and Rescue
Programming 'things' assignment for Sheffield Hallam University. A GUI and Zumo robot code to navigate cooridors and search rooms, and then return home autonomously.

Feel free to steal.

https://www.youtube.com/watch?v=L2vuAfGVpTY&t=68s

---

To begin with, I decided to use Electron and React. This is because it easily allowed me to have access to NodeJS and a way to create a user interface with React, all in one package. I run a socket.io server using NodeJS & Express, which the React frontend connects to and forwards messages to the Zumo through the serial-port npm module and the xbee-api npm module. Here is an image of the GUI I created:
![gui](https://i.imgur.com/CL1GV3R.png)

#### Task 1
My robot is able to be controlled with W, A, S, D.

#### Task 2
After spinning on the spot and calibrating after being switched on, the robot automatically keeps within the corridor walls by using the line sensors. If it sees a line on the left it adjusts its left motor speed, and so on. If it sees a line infront of it, it stops.

#### Task 3
Once the Zumo hits a wall infront of it (detected using the line sensors), it stops and waits for a command from the user. The turn buttons can be pressed or the robot can be manually turned.

#### Task 4
The GUI has buttons 'Turn left' and 'Turn right'. Once pressed a turn message is send to the zumo. The Zumo sets its motors to the right speeds to turn the given way, and the gyroscope is used to wait until the desired angle is reached.

#### Task 5
When the robot is in autonomous mode, the user can press the 'Stop' button on the GUI, causing the robot to stop. They can then press 'Search room (right)' or 'Search room (left)' on the GUI. Pressing one of these will make the robot turn in the relevant direction, move into the room, and scan the room by swiping from left to right. If an object is detected, this is made clear in the GUI. Once the scan is complete, the robot returns to its original position before the button is pressed.

#### Task 6
The user can press 'T Junction Left' or 'T Junction Right' to turn down a T junction. Once the end of the T Junction has been reached, the user can press 'Search other side' to search the other side of the t junction.

#### Task 7
Once the Zumo is at the end of the second half of the t junction, the user can press 'Return home'. The robot will then turn 180 degrees and search every room that it previously found someone in in reverse order. If it finds someone, it displays a message on the GUI, plays a tone and activates the LED. Then it returns to its original starting position autonomously.

#### Extensions
The Zumo returns to its original position ('home') completely autonomomously.

#### Running
Run npm run


#### Other
Github Repository - https://github.com/vrondakis/search-and-rescue

Video - https://www.youtube.com/watch?v=L2vuAfGVpTY

This project uses TurnSensor code from https://github.com/pololu/romi-32u4-arduino-library/blob/master/examples/RotationResist/TurnSensor.cpp
