import React, {Component} from "react";
import {socket} from "../services/zumo";

import ManualControl from "./ManualControl"
import StateBar from "./StateBar"


// move to aprameters
const speed = 250
const turningSpeed = 0.4

class ControlPanel extends Component{
    constructor(props){
        super(props);
        
        this.handleKeyDown = this.handleKeyDown.bind(this);
        this.handleKeyUp = this.handleKeyUp.bind(this);
        this.keys = {}

        this.state = {
            motors : {
                left : 0,
                right : 0
            }
        }
        
    }

    setMotorSpeeds(speeds){
        socket.emit("set-motors", {left : Math.floor(speeds.left * speed), right : Math.floor(speeds.right * speed)})
    }

    handleKeyDown(e){
        const {motors} = this.state;
        if(this.keys[e.key]) return;
        if(["w", "a", "s", "d"].indexOf(e.key) === -1) return;
        this.keys[e.key] = true

        switch(e.key){
            case "w":
                motors.left += 0.5;
                motors.right += 0.5;
                break;
            
            case "s":
                motors.left -= 0.5;
                motors.right -= 0.5;
                break;

            case "a":
                motors.right += turningSpeed;
                break;

            
            case "d":
                motors.left += turningSpeed;
                break;
        }
        
        this.setState({motors});
        this.setMotorSpeeds(motors);
    }

    handleKeyUp(e){
        const {motors} = this.state;
        if(["w", "a", "s", "d"].indexOf(e.key) === -1) return;
        delete this.keys[e.key];


        switch(e.key){
            case "w":
                motors.left -= 0.5;
                motors.right -= 0.5;
                break;

            case "a":
                motors.right -= turningSpeed;
                break;

            
            case "d":
                motors.left -= turningSpeed;
                break;

            case "s":
                motors.left += 0.5;
                motors.right += 0.5;
                break;
        }
        
        this.setState({motors});
        this.setMotorSpeeds(motors);
    }

    componentDidMount(){
        document.addEventListener('keydown', this.handleKeyDown)
        document.addEventListener('keyup', this.handleKeyUp)
    }

    componentWillUnmount(){
        document.removeEventListener('keydown', this.handleKeyDown);
        document.removeEventListener('keyup', this.handleKeyUp);
    }

    render(){
        const {style} = this.props;

        return (

            <div style={{...style, ...styles.container}}>
                <StateBar />
                <ManualControl/>
            </div>

        )
    }
}

const styles = {
    container : {

    }
}

export default ControlPanel