import React, {Component, Fragment} from "react";
import Log from "../components/Log";
import ControlPanel from "../components/ControlPanel";


class Main extends Component{
    render(){
        return (
            <div className="main-container">
                <Log style={styles.logContainer} />
                <ControlPanel style={styles.controlPanelContainer} />
            </div>
        )
    }
}

const styles = {
    logContainer : {
        width : "40%",
        verticalAlign: "top",
        display : "inline-block"
    },
    controlPanelContainer : {
        width : "60%",
        verticalALign: "top",
        display : "inline-block"
    }
}

export default Main