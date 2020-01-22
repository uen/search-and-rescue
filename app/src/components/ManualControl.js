import React, {Component} from "react";
import ControlButton from "./ControlButton";
import {socket} from "../services/zumo";

class ManualControl extends Component{
    constructor(props){
        super(props);

        this.beginAutonomous = this.beginAutonomous.bind(this);
    }

    beginAutonomous(){
        socket.emit("begin-autonomous");
    }

    calibrateLineSensors(){
        socket.emit("calibrate-line-sensors");
    }

    stopZumo(){
        socket.emit("zumo-stop");
    }

    turnZumo(direction){
        socket.emit("zumo-turn", {direction})
    }

    searchRoom(direction){
        socket.emit("search-room", {direction})
    }

    render(){
        const {style} = this.props;

        return (

            <div style={{...style, ...styles.container}}>
                <div className="control-buttons" style={styles.manualControl}>
                    <div>
                        <ControlButton label="Rotate Left" onClick={() => this.turnZumo("L")} control secondary/>
                        <ControlButton label="Forward (W)" control />
                        <ControlButton label="Rotate Right" onClick={() => this.turnZumo("R")} control secondary/>
                    </div>
                    <div>
                        <ControlButton label="Left (A)" control />
                        <ControlButton label="Back (S)" control/>
                        <ControlButton label="Right (D)" control/>
                    </div>

                    <div>
                        <ControlButton label="Stop" danger onClick={this.stopZumo}/>
                    </div>
                </div>
                <div className="control-buttons" style={styles.roomControl}>
                    <ControlButton label="Search room (left)" room/>
                    <ControlButton label="Search room (right)" onClick={() => this.searchRoom("R")}/>
                </div>

                <div className="control-buttons">
                    <ControlButton label="Calibrate line sensors" procedure onClick={this.calibrateLineSensors}/>
                    <ControlButton label="Begin Autonomous mode" procedure onClick={this.beginAutonomous}/>

                </div>
            </div>

        )
    }
}

const styles = {
    container : {
        padding: 10
    },
    manualControl : {
        textAlign : "center",
        marginTop: 30,
        marginBottom: 40
    },
    roomControl : {
        textAlign: "center",
        marginTop: 30,
        marginBottom: 30
    }
}

export default ManualControl