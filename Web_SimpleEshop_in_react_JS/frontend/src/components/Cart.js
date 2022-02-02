import React from "react";
import OrderForm from "./OrderForm";

class Cart extends React.Component {
    render(){
        var classes;
        if(this.props.pageState === 2)
            classes = "cart";
        else
            classes = "cart hidden";
        
        var toRender = [];
        //console.log(this.props.allProducts, this.props.productsInCart);
        for(var i = 0; i < this.props.productsInCart.length; i++){
            toRender.push(<p className="orderText">{this.props.allProducts[this.props.productsInCart[i] - 1].name} {this.props.allProducts[this.props.productsInCart[i] - 1].price}€</p>);
            toRender.push(<br></br>);
        }
        //form only when there is product
        if(this.props.productsInCart.length)
            toRender.push(<OrderForm onPageChange={this.props.onPageChange} productsInCart={this.props.productsInCart} flushCart={this.props.flushCart}/>);
        else
            toRender.push(<p>Nemas v kosiku ziaden produkt!</p>);
        return (
            <div className={classes}>
                <br></br>
                {toRender}
                <p>Zadavat bez diakritiky!</p>
            </div>
        );
    }
}
  
export default Cart;