import React from "react";

class PageButton extends React.Component {
    render(){
        return (
            <button id={this.props.id} className="addToCart productComponent" onClick={()=>{this.props.onPageChange(this.props.setTo);}}>{this.props.textContent}</button>
        );
    }
}
  
export default PageButton;