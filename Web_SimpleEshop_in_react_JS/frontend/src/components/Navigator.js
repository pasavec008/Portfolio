import React from "react";
import ProductBox from './ProductBox';
import PageButton from "./PageButton";
import Cart from "./Cart";
import Sucess from "./Sucess";

class Navigator extends React.Component {
    constructor(props){
        super(props);
        this.changePage = this.changePage.bind(this);
        this.addToCart = this.addToCart.bind(this);
        this.flushCart = this.flushCart.bind(this);
        this.state = {display: 1, render: 0};
        this.productsInCart = [];
        this.allProducts = [];
    }
    componentDidMount(){
        fetch('http://localhost:3001/data').then(response => response.text()).then(data => {this.dataFromServer = data; this.setState({display: 1, render: 1});});
    }
    changePage(state){
        this.setState({display: state});
    }
    flushCart(){
        this.productsInCart = [];
        document.getElementById("kosik").textContent="Košík";
    }
    addToCart(id){
        this.productsInCart.push(id);
        document.getElementById("kosik").textContent="Košík (" + this.productsInCart.length + ")";
    }
    render(){
        if(this.state.render === 1){
            this.allProducts = JSON.parse(this.dataFromServer);
            //console.log(this.allProducts);
            return (
                <div>
                    <ProductBox pageState={this.state.display}  allProducts={this.allProducts} addToCart={this.addToCart} />
                    <Cart pageState={this.state.display} productsInCart={this.productsInCart} allProducts={this.allProducts} onPageChange={this.changePage} flushCart={this.flushCart}/>
                    <Sucess pageState={this.state.display}/>
                    <PageButton id="ponuka" onPageChange={this.changePage} setTo={1} textContent="Ponuka"/>
                    <PageButton id="kosik" onPageChange={this.changePage} setTo={2} textContent={"Košík (" + this.productsInCart.length + ")"}/>
                </div>
                );
        }
        else
            return (null);
    }
}
  
export default Navigator;