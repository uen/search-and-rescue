import React, {Component} from "react";
import ControlButton from "./ControlButton";
import {socket} from "../services/zumo";

class ManualControl extends Component{
    constructor(props){
        super(props);

        this.beginAutonomous = this.beginAutonomous.bind(this);
    }

    beginAutonomous(){
        socket.emit("begin-autonomous")
    }

    render(){
        const {style} = this.props;

        return (

            <div style={{...style, ...styles.container}}>
                <div className="control-buttons" style={styles.manualControl}>
                    <div>
                        <ControlButton label="Rotate Left" control secondary/>
                        <ControlButton label="Forward" control />
                        <ControlButton label="Rotate Right" control secondary/>
                    </div>
                    <div>
                        <ControlButton label="Left" control />
                        <ControlButton label="Back" control/>
                        <ControlButton label="Right" control/>
                    </div>

                    <div>
                        <ControlButton label="Stop" danger/>
                    </div>
                </div>
                <div className="control-buttons" style={styles.roomControl}>
                    <ControlButton label="Search room (left)" room/>
                    <ControlButton label="Search room (right)" room/>
                </div>

                <div className="control-buttons">
                    <ControlButton label="Begin Autonomous mode" procedure onClick={this.beginAutonomous}/>
                    {/* <ControlButton label="Calibrate line sensor" procedure/> */}
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
        marginBottom: 30
    },
    roomControl : {
        textAlign: "center",
        marginTop: 30,
        marginBottom: 30
    }
}

export default ManualControl