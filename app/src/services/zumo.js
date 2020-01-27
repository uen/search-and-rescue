import io from "socket.io-client";
import env from "../config"

const socketConnection = io(env.WS_URL)

socketConnection.on("connect", () => {
    // console.log("connected")
})


export {socketConnection as socket}