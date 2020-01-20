const io = require("socket.io");
const server = io.listen(3001)

console.log("electorn  connecteed")

server.on("connection", (socket) => {
    console.log("server connected")
})
