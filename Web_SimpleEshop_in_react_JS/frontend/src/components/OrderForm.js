import React from "react";

class OrderForm extends React.Component {
    constructor(props){
        super(props);
        this.sendToDB = this.sendToDB.bind(this);
    }
    sendToDB(e){
        e.preventDefault();
        const requestOptions = {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({
                email: document.getElementById("email").value,
                name: document.getElementById("name").value,
                street: document.getElementById("street").value,
                houseNumber: document.getElementById("houseNumber").value,
                town: document.getElementById("town").value,
                postNumber: document.getElementById("postNumber").value,
                orderedProducts: this.props.productsInCart.join()
            })
        };
        fetch('http://localhost:3001/insert', requestOptions);
            
        this.props.onPageChange(3);
        this.props.flushCart();
    }
    render(){
        return (
            <div className="orderForm">
                <form onSubmit={this.sendToDB}>
                    <input id="email" required="required" pattern="[a-z0-9._%+-]+@[a-z0-9.-]+"></input><label> Email</label><br></br>
                    <input id="name" required="required" pattern="[A-Za-z ]+"></input><label> Meno</label><br></br>
                    <input id="street" required="required" pattern="[A-Za-z ]+"></input><label> Ulica</label><br></br>
                    <input id="houseNumber" required="required" pattern="[0-9]+"></input><label> Cislo bytu</label><br></br>
                    <input id="town" required="required" pattern="[A-Za-z ]+"></input><label> Mesto</label><br></br>
                    <input id="postNumber" required="required" pattern="[0-9]+"></input><label> PSC</label><br></br>
                    <button type="submit">Vytvorit objednavku</button>
                </form>
            </div>
        );
    };
}
  
export default OrderForm;