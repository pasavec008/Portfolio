import Product from "./Product";
import React from "react";

class ProductBox extends React.Component {
        
    render(){
        var classes;
        if(this.props.pageState === 1)
            classes = "productBox";
        else
            classes = "productBox hidden";

        var productsToRender = [];

        //render all objects received from db
        for(var i = 0; i < this.props.allProducts.length; i++){
            productsToRender.push(<Product name={this.props.allProducts[i].name} productImage={this.props.allProducts[i].picture} price={this.props.allProducts[i].price} id={this.props.allProducts[i].id} addToCart={this.props.addToCart}/>);
        }

        return (
        <div className={classes}>
            {productsToRender}
        </div>
        );
        
    }
}
  
export default ProductBox;