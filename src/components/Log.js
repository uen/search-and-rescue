import React, {Component} from "react";


class Log extends Component{
    render(){
        const {style} = this.props;
        console.log(this.props)

        console.log(style)
        const log = ["Initialized", "Running automous search and destroy protocol", "Detected object in room 4","Running automous search and destroy protocol", "Detected object in room 4","Running automous search and destroy protocol", "Detected object in room 4","Running automous search and destroy protocol", "Detected object in room 4","Running automous search and destroy protocol", "Detected object in room 4"]

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