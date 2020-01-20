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


let portOpen = false;
serialPort.on("open", () => {
    portOpen = true;

    console.log("Serial port open");

    

    serialPort.on('data', function(data){

   
        console.log("got data:" , data);
    });
});


const OPCODES = {
    "begin-autonomous" : 2,
    "set-speeds" : 3
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

        sendZumoData("set-speeds", `${speeds.left}${speeds.right}`)
    });

    socket.on("begin-autonomous", (data) => {
        sendZumoData("begin-autonomous")
    })
})

