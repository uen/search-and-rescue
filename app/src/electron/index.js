const io = require("socket.io");
const SerialPort = require('serialport');
const XBee = require('xbee-api');

const xbee = new XBee.XBeeAPI({
    api_mode : 1,
    module: "XBee"
});

const serialPort = new SerialPort("/dev/ttyUSB0", {
    baudRate : 9600,
    parser: xbee.rawParser()
});


const zumoCommandHandlers = {
    "state": (message) => {
        server.sockets.emit("state-update", message);
    },
    "zumo": (message) => {
        console.log("sneding to client:", message)
        server.sockets.emit("zumo-log", message);
    }
}

let portOpen = false;
serialPort.on("open", () => {
    portOpen = true;

    console.log("Serial port open");

    
    let dataBuffer = "";
    serialPort.on('data', (data) => {
        data = data.toString();

        for(let i = 0; i < data.length; i++){
            if(data[i] == "\n"){

                const [command, message] = dataBuffer.split(":")
                if(!zumoCommandHandlers[command]) console.log("Zumo sent unknown command:", `"${command}"`);
                zumoCommandHandlers["zumo"](dataBuffer)

                dataBuffer = "";
                break;
            }

            dataBuffer += data[i];
        }
    });
});


const OPCODES = {
    "zumo-stop": 1,
    "begin-autonomous" : 2,
    "set-speeds" : 3,
    "calibrate-line-sensors" : 4,
    "zumo-turn" : 5,
    "search-room" : 6,
    "zumo-record-start": 7,
    "zumo-record-stop": 8,
    "return-home": 9
}

const sendZumoData = (opcode, data = "") => {
    if(!OPCODES[opcode]) return console.error("Unknown opcode: ", opcode);

    const frame = xbee.buildFrame({
        type: XBee.constants.FRAME_TYPE.ZIGBEE_TRANSMIT_REQUEST,
        data: `${OPCODES[opcode]}${data}`
    });

    if(portOpen){
        serialPort.write(frame, (err, res) => {
            console.log("Sent frame: ", opcode)
            if(err) console.error("Error transmitting frame: ", err);
        })
    }
}


// Websocket server and message handling
const server = io.listen(3001)
server.on("connection", (socket) => {
    console.log("someone co nnected electorn")

    // Set motor speeds for manual control
    socket.on("set-motors", (data) => {
        const speeds = {}
        speeds.left = `${data.left}`.padStart(3, "0");
        speeds.right = `${data.right}`.padStart(3, "0");

        sendZumoData("set-speeds", `${speeds.left}${speeds.right}`);
    });

    socket.on("begin-autonomous", (data) => {
        sendZumoData("begin-autonomous");
    });

    socket.on("calibrate-line-sensors", () => {
        sendZumoData("calibrate-line-sensors");
    });

    socket.on("zumo-stop", () => {
        console.log("sending zumo stop")
        sendZumoData("zumo-stop"); 
    });

    socket.on("zumo-turn", (data) => {
        console.log("turning: ", data);
        sendZumoData("zumo-turn", data.direction);
    });

    socket.on("search-room", (data) => {
        sendZumoData("search-room", data.direction)
    });

    socket.on("zumo-record", (data) => {
        console.log("zumo record is:", data);
        sendZumoData("zumo-record-start", data.direction);
    });

    socket.on("zumo-stop-recording", () => {
        sendZumoData("zumo-record-stop");
    });
    
    socket.on("return-home", () => {
        console.log("is retunring home now..");
        sendZumoData("return-home");
    });

    socket.on("zumo-reset", () => {
        // TODO: Send stop command
        console.log("consoel log treset")
        socket.emit("reset-log")
        socket.emit("state-update", "Idle");
    });
})

