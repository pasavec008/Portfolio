const express = require('express');
const mysql = require('mysql2');
const cors = require('cors');

var db = mysql.createConnection({
    host: "mysql_db",
    user: "root",
    password: "root",
    database: "eshop"
});


const app = express();
app.use(cors());
app.use(express.json());
app.use(express.urlencoded({ extended: true }));


app.get('/data', (req, res) => {
    //get data about products from db
    var sql = "SELECT * FROM Products";
    var dataToSend;
    db.query(sql, (err, results)=>{
        dataToSend = results;
        res.set('Content-Type', 'application/json');
        res.status(200);
        var messageToSend = JSON.stringify(dataToSend);
        //console.log(messageToSend);
        res.send(messageToSend);
    });
});

// add a order to the database
app.post("/insert", (req, res) => {
    //console.log(req.body);
    //insert customer if not exists
    var customerQuery = `INSERT IGNORE INTO Customers (email, name, street, number, town, post_code) VALUES (` 
        + `"` + req.body.email + `", `
        + `"` + req.body.name + `", `
        + `"` + req.body.street + `", `
        + req.body.houseNumber + `, `
        + `"` + req.body.town + `", `
        + req.body.postNumber + 
    `)`;
    db.query(customerQuery, (err, results)=>{
        //now get ID of customer
        var getIdQuery = `SELECT id FROM Customers WHERE email = "` + req.body.email + `"`;
        db.query(getIdQuery, (err, results)=>{
            var customerID = results[0].id;
            //console.log(results);
            var orderQuery = `INSERT INTO Orders (customer_id, state, ordered_products) VALUES(`
                + customerID + `, `
                + -1 + `, `
                + `"` + req.body.orderedProducts + `"` +
            `)`;
            db.query(orderQuery, (err, results)=>{
                if(!err){
                    res.status(200);
                    res.send();
                }
            });
        })
    });
});

app.get('/ad', (req, res)=>{
    var adQuery = `SELECT * FROM Ads
        ORDER BY RAND()
        LIMIT 1;`
    db.query(adQuery, (err, results)=>{
        var messageToSend = JSON.stringify(results[0]);
        res.status(200);
        res.send(messageToSend);
    })
});

//increment ad click
app.post("/increment",(req, res)=>{
    var incrementQuery = `UPDATE Ads
    SET counter = counter + 1
    WHERE id = ` + req.body.id;
    db.query(incrementQuery, (err, results)=>{
        if(!err){
            res.status(200);
            res.send();
        }
    });
});

//data for admin page
app.get("/admin", (req, res) =>{
    var adminQuery = `SELECT * FROM Orders`;
    var dataToSend;
    db.query(adminQuery, (err, results)=>{
        dataToSend = results;
        res.set('Content-Type', 'application/json');
        res.status(200);
        var messageToSend = JSON.stringify(dataToSend);
        res.send(messageToSend);
    });
})

//change state of order
//increment ad click
app.post("/pay",(req, res)=>{
    var payQuery = `UPDATE Orders
    SET state = state * -1
    WHERE id = ` + req.body.id;
    db.query(payQuery, (err, results)=>{
        if(!err){
            res.status(200);
            res.send();
        }
    });
});

app.get("/admin_ads", (req, res) =>{
    var adminAdsQuery = `SELECT * FROM Ads`;
    var dataToSend;
    db.query(adminAdsQuery, (err, results)=>{
        dataToSend = results;
        res.set('Content-Type', 'application/json');
        res.status(200);
        var messageToSend = JSON.stringify(dataToSend);
        res.send(messageToSend);
    });
});

app.post("/add_ad",(req, res)=>{
    //console.log("adding");
    var addAdQuery = `INSERT INTO Ads (url, picture, counter) VALUES (` 
        + `"` + req.body.url + `", `
        + `"` + req.body.picture + `", 0)`;
    db.query(addAdQuery, (err, results)=>{
        if(!err){
            res.status(200);
            res.send();
        }
    });
});

app.listen('3001', () => {console.log("listening on port 3001")});