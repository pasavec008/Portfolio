import React from "react";

class AdminAds extends React.Component {
    constructor(props){
        super(props);
        this.state = {render: 0};
        this.addAd = this.addAd.bind(this);
    }
    componentDidMount(){
        fetch('http://localhost:3001/admin_ads').then(response => response.text()).then(data => {this.dataFromServer = data;this.setState({render: 1})});
    }
    addAd(e){
        e.preventDefault();
        const requestOptions = {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({
                url: document.getElementById("url").value,
                picture: document.getElementById("picture").value
            })
        };
        fetch('http://localhost:3001/add_ad', requestOptions).then(response =>{if(response.ok)window.location.reload();});
    }
    render(){
        if(this.state.render){
            var parsedData = JSON.parse(this.dataFromServer);
            var productsToRender = [
                <tr>
                    <th>Id reklamy</th>
                    <th>Url reklamy</th>
                    <th>Obrazok reklamy</th>
                    <th>Pocet kliknuti na reklamu</th>
                </tr>
            ];

            for(var i = 0; i < parsedData.length; i++){
                productsToRender.push(
                    <tr>
                        <th>{parsedData[i].id}</th>
                        <th>{parsedData[i].url}</th>
                        <th>{parsedData[i].picture}</th>
                        <th>{parsedData[i].counter}</th>
                    </tr>
                );
            }

            productsToRender.push(
                <tr>
                    <th><p>AUTO</p></th>
                    <th><input className="adaptInput" id="url" required="required"></input></th>
                    <th><input className="adaptInput" id="picture" required="required"></input></th>
                    <th><p>AUTO</p></th>
                </tr>
            );

            

            return (
                <div>
                    <form onSubmit={this.addAd}>
                        <table className="orderTable">
                            <tbody>
                                {productsToRender}
                            </tbody>
                        </table>
                        <button type="submit">Pridat reklamu</button>
                    </form>
                </div>
            );
        }
        else
            return(null);
    }
}
  
export default AdminAds;