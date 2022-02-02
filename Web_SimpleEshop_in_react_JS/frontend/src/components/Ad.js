import React from "react";

class Ad extends React.Component {
    constructor(props){
        super(props);
        this.increaseCounter = this.increaseCounter.bind(this);
    }
    increaseCounter(){
        const requestOptions = {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({
                id: this.data.id
            })
        };
        fetch('http://localhost:3001/increment', requestOptions).then(window.location = this.data.url);
    }
    render(){
        this.data = JSON.parse(this.props.data);
        return (
            <div className="ad">
                <img className="adPicture" src={this.data.picture} onClick={this.increaseCounter}></img>
            </div>
        );
    };
}
  
export default Ad;