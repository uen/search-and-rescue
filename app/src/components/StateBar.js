import React, {Component} from "react";
import {socket} from "../services/zumo"



class StateBar extends Component{
    constructor(props){
        super(props);
    
        this.state = {
            state: "Idle"
        }

        this.reset = this.reset.bind()
    }

    componentDidMount(){
        socket.on("state-update", (state) => {
            console.log("state has updated");
            this.setState({state})
        });
    }

    reset(){
        console.log("zumo reset")
        socket.emit("zumo-reset")
    }


    render(){
        const {state} = this.state;

        return (
            <div className="state-bar">
                <div className="name">Search and Rescue</div>
                <div>
                    <a className="button control-button is-control is-reset" onClick={this.reset}>
                        <div>Reset</div>
                    </a>
                    <div className={`state ${!state ? "is-offline" : ""}`}>State: {state ? state : "Offline"}</div>
                </div>
            </div>
        )
    }
}


export default StateBar;