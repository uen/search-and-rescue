import React, {Component} from "react";
import {socket} from "../services/zumo"



class StateBar extends Component{
    constructor(props){
        super(props);
    

        this.reset = this.reset.bind()
    }

    componentDidMount(){
    
    }

    reset(){
        // console.log("zumo reset")
        socket.emit("zumo-reset")
    }


    render(){

        return (
            <div className="state-bar">
                <div className="name">Search and Rescue</div>
                <div>
                    <a className="button control-button is-control is-reset" onClick={this.reset}>
                        <div>Reset</div>
                    </a>
                </div>
            </div>
        )
    }
}


export default StateBar;