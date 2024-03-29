import React, {Component} from "react";


class ControlButton extends Component{
    render(){
        const {label, secondary, danger, success, control, procedure, room, onClick, style} = this.props;

        return (
            <a 
                className={`
                    control-button
                    ${room ? "is-room" : ""}
                    ${procedure ? "is-procedure" : ""}
                    ${success ? "is-success" : ""}
                    ${control ? "is-control" : ""}
                    ${danger ? "is-danger" : ""}
                `}
                onClick={onClick}
                style={{...style, ...styles.container}}>
                
                <div style={{...styles.button, ...(secondary ? styles.secondaryButton : {})}}>
                    {label}
                </div>
            </a>

        )
    }
}

const styles = {
    container : {
        display: "inline-block",
    },
    button : {
        padding: 10,
        margin: 5
    },
    secondaryButton : {
        fontSize: 12,
        width : 70
    }
}

export default ControlButton