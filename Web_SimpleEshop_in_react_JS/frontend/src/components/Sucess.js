import React from "react";
import Ad from "./Ad";

class Sucess extends React.Component {
    constructor(props){
        super(props);
        this.state = {render: 0};
    }
    componentDidMount(){
        fetch('http://localhost:3001/ad').then(response => response.text()).then(data => {this.dataFromServer = data;this.setState({render: 1})});
    }

    render(){
        if(this.state.render){
            //console.log(this.dataFromServer);
            var classes;
            if(this.props.pageState === 3)
                classes = "sucess";
            else
                classes = "sucess hidden";
            return (
                <div className={classes}>
                    <p>Vasa objednavka bola zaznamenana, dakujeme!</p>
                    <Ad data={this.dataFromServer} />
                </div>
            );
        }
        else
            return (null);
    }
}

export default Sucess;