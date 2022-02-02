import BuyButton from "./BuyButton";
import React from "react";

class Product extends React.Component {
  render(){
    return (
      <div className="product">
        <h2 className="productText">{this.props.name}</h2>
        <img className="productPicture" src={this.props.productImage}></img>
        <p className="productText">Cena {this.props.price}€</p>
        <BuyButton id={this.props.id} addToCart={this.props.addToCart} />
      </div>
    );
  };
}
  
export default Product;