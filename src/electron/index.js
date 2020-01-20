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


// Websocket server and message handling
const server = io.listen(3001)
server.on("connection", (socket) => {
    console.log("someone co nnected electorn")
    socket.on("set-motors", (data) => {
        const speeds = {}
        speeds.left = `${data.left}`.padStart(3, "0");
        speeds.right = `${data.right}`.padStart(3, "0");

        const leftFrame = xbee.buildFrame({
            type : XBee.constants.FRAME_TYPE.ZIGBEE_TRANSMIT_REQUEST,
            data : `${3}${speeds.left}${speeds.right}`
        });

        const rightFrame = xbee.buildFrame({
            type : XBee.constants.FRAME_TYPE.ZIGBEE_TRANSMIT_REQUEST,
            data : `${0x02}${speeds.right}`
        });

        if(portOpen){
            serialPort.write(leftFrame, (err, res) => {
                console.log("writing frame..")
                if(err) return console.log(err);
            });
        }
    })
})

