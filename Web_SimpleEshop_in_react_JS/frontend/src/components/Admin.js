import React from "react";
import AdminOrders from "./AdminOrders";
import AdminAds from "./AdminAds";

class Admin extends React.Component {
    render(){
        return (
            <div>
                <h1>Objednavky</h1>
                <AdminOrders />
                <br></br>
                <h1>Reklamy</h1>
                <AdminAds />
            </div>
        );
    }
}
  
export default Admin;