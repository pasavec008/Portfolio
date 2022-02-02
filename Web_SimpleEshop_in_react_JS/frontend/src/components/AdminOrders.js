import React from "react";

class AdminOrders extends React.Component {
    constructor(props){
        super(props);
        this.state = {render: 0};
        this.payOrder = this.payOrder.bind(this);
    }
    componentDidMount(){
        fetch('http://localhost:3001/admin').then(response => response.text()).then(data => {this.dataFromServer = data;this.setState({render: 1})});
    }

    payOrder(id){
        const requestOptions = {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({
                id: id
            })
        };
        fetch('http://localhost:3001/pay', requestOptions).then(response =>{if(response.ok)window.location.reload();});
    }

    render(){
        
        if(this.state.render){
            var parsedData = JSON.parse(this.dataFromServer);
            var productsToRender = [
                <tr>
                    <th>Id objednavky</th>
                    <th>Id zakaznika</th>
                    <th>Objednane produkty (ich Id)</th>
                    <th>Stav objednavky (-1 = nezaplatena, 1 = zaplatena)</th>
                    <th>Zmenit stav</th>
                </tr>
            ];

            //render all orders from DB
            //we need to dynamicly set i from loop to function calls from buttons
            for(var i = 0; i < parsedData.length; i++){
                productsToRender.push(
                    <tr>
                        <th>{parsedData[i].id}</th>
                        <th>{parsedData[i].customer_id}</th>
                        <th>{parsedData[i].ordered_products}</th>
                        <th>{parsedData[i].state}</th>
                        <th><button onClick={((dyn)=>{
                            return ()=>{this.payOrder(dyn + 1)}
                        })(i)}>Zmen stav</button></th>
                    </tr>
                );
            }
                
            return (
                <div>
                    <table className="orderTable">
                        <tbody>
                            {productsToRender}
                        </tbody>
                    </table>
                </div>
            );
        }
        else
            return (null);
    }
}
  
export default AdminOrders;