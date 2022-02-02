import React from "react";

class BuyButton extends React.Component {
    render(){
        return (
            <button className="addToCart productComponent" onClick={()=>{this.props.addToCart(this.props.id);}}>Pridať do košíka</button>
          );
    }
}
  
export default BuyButton;