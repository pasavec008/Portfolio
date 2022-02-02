CREATE DATABASE IF NOT EXISTS eshop;
USE eshop;

/*ALTER USER root IDENTIFIED WITH mysql_native_password BY root;*/

CREATE TABLE IF NOT EXISTS Products (
  id INT NOT NULL,
  name VARCHAR(100) NOT NULL,
  picture VARCHAR(100) NOT NULL,
  price VARCHAR(100) NOT NULL,
  PRIMARY KEY (id)
);

CREATE TABLE IF NOT EXISTS Customers (
  id INT NOT NULL AUTO_INCREMENT,
  email VARCHAR(100) NOT NULL UNIQUE,
  name VARCHAR(100) NOT NULL,
  street VARCHAR(100) NOT NULL,
  number INT NOT NULL,
  town VARCHAR(100) NOT NULL,
  post_code INT NOT NULL,
  PRIMARY KEY (id)
);

CREATE TABLE IF NOT EXISTS Orders (
  id INT NOT NULL AUTO_INCREMENT,
  customer_id INT NOT NULL,
  state INT NOT NULL,
  ordered_products VARCHAR(100),
  PRIMARY KEY (id),
  FOREIGN KEY (customer_id) REFERENCES Customers(id)
);

CREATE TABLE IF NOT EXISTS Ads (
  id INT NOT NULL AUTO_INCREMENT,
  url VARCHAR(100) NOT NULL,
  picture VARCHAR(100) NOT NULL,
  counter INT NOT NULL,
  PRIMARY KEY (id)
);

/*Generate few products*/
INSERT IGNORE INTO Products
VALUES (1, "Motyka", "https://zap.sk/16491-large_default/motyka-fed-205-1200-srdcova-s-nasadou.jpg", 10);
INSERT IGNORE INTO Products
VALUES (2, "Kvetinac", "https://images.obi.sk/product/HU/1500x1500/170945_3.jpg", 5);
INSERT IGNORE INTO Products
VALUES (3, "Substrat", "https://images.obi.sk/product/CZ/1500x1500/961921_1.jpg", 15);

/*Generate ads (manual add of ads should be implemented too)*/
INSERT IGNORE INTO Ads
VALUES (1, "https://www.alza.sk/", "https://techpit.sk/wp-content/uploads/2019/07/alza-mimozemstan.jpg", 0);
INSERT IGNORE INTO Ads
VALUES (2, "https://www.ikea.com/sk/sk/", "https://i.ytimg.com/vi/mFajSqKcDYc/maxresdefault.jpg", 0);