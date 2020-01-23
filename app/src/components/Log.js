import React, {Component} from "react";
import {socket} from "../services/zumo";


const defaultLog = "Search and Rescue initialized."
class Log extends Component{
    constructor(props){
        super(props);

        this.state = {
            log : [defaultLog] 
        }
    }

    componentDidMount(){
        const {log} = this.state;
        socket.on("zumo-log", (message) => {
            console.log("socket log here", message)
            log.unshift(message);
            this.setState({log})
        });

        socket.on("reset-log", (message) => {
            this.setState({log: [defaultLog + "alwdaoa"]});
        })
    }

    render(){
        const {style} = this.props;
        const {log} = this.state;

        return (
            <div style={{...styles.container, ...style}}>
                <div style={styles.log}>
                    {log.map(item => (
                        <div style={styles.logItem}>
                            <span style={styles.zumoText}>Zumo: </span>
                            <span style={styles.logText}>{item}</span>
                        </div>
                    ))}
                </div>
            </div>
        )
    }
}

const styles = {
    container : {
        backgroundColor: "#00144e",
        minHeight: "100vh",
        maxHeight: "100vh",
        overflowY: "auto",
        textAlign: "left",
    },
    log : {
        padding: 10
    },

    logItem : {
        marginBottom: 7,
        fontSize: 12,
        lineHeight: 1.5,
        fontFamily: '"Courier New", Courier, monospace'
    },
    zumoText : {
        color: "#fcff37",
    },
    logText : {
        color: "#ffffff"
    },

}

export default Log