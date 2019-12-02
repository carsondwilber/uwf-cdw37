// This exercise will involve creating a separate node module for your middleware
// You will need to set up express just like you did in problem-a, then follow the instructions


// Require the express framework
const express = require("express");

// Create an instance of express in a variable called "app"
let app = express(); 

// Require the middleware.js file and store it in a constant called "middlware"
// See the middleware.js file for the implementations that are required there
let middleware = require("./middleware");

// Add an instance of the card number detection middleware here. It should respont to all request types.
// That way, no matter what route is visited we will look for card numbers.
app.use(middleware.checkCardNumbers);

// Create a route for the home page "/" that returns text "Register Now"
app.get("/", function (req, res) {
    res.send("Register Now")
});

// Create another route that works for "/register/<whatever>", where whatever is any string that you
// want to intsrt there.  This route should return the text "You are registered".
// We can test our credit card middleware here by calling "/register/seminar?cardNum=5500000000000004"
app.get("/register/*", (req, res) => {
    res.send("You are registered");
});


// Create a route at "/cause/error" that purposefully throws a new Error object 
// with the message "Crazy error message"
app.get("/cause/error", () => {
    throw new Error("Crazy error message");
});

// Insert the "handleErrors"error handling middleware object here. 
// It should respont to all request types.
app.use(middleware.handleErrors);

// Insert the "handleNotFound" 404 error handling middleware here.
app.use(middleware.handleNotFound);

// Export the app object using the module.exports variable
module.exports = app;