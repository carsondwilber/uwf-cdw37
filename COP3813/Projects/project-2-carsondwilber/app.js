const express = require('express');
const path = require('path');
const creds = require("./credentials");

let app = express();

// Set up database connection
require("./models/db");

// Set up sessions
const session = require('express-session');
app.use(session(creds.session));

// Import our modules
const main = require('./controllers/main.js');
const cart = require('./controllers/cart.js');

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'pug');

app.set('port', process.env.PORT || 3000);

app.use(express.static(__dirname + '/public'));

// Handle form submissions
const bodyParser = require('body-parser');
app.use(bodyParser.urlencoded({ extended: true }));

/*************** Middleware *******************/

// Shopping cart middleware
app.use((req, res, next) => {
	// Copy flash message to local and delete from session scope
	if (!req.session.cart)
	{
		req.session.cart = [];
	}

	res.locals.cart = req.session.cart;

	next();
  });

// Flash message middleware
app.use((req, res, next) => {
	// Copy flash message to local and delete from session scope
	res.locals.flash = req.session.flash ? {message: req.session.flash} : null;
	delete req.session.flash;
	next();
  });


/*************** Routes *******************/
// Note: the route handlers have been separated into their own files
// (See the handlers directory)
app.get('/', main.home);

// Displays the desired category to the user
app.get('/category/:category', main.category);

// admin form to add items to the database/list
app.get('/admin', main.admin);

//route for POST to handle admin form submission
app.post('/admin/process', main.process);

//route for view sales list
app.get('/admin/sales', main.sales);

// Shopping cart routes
app.get('/cart/view', cart.view);
app.get('/cart/add/:id', cart.add);
app.get('/cart/remove/:id', cart.remove);
app.get('/cart/checkout', cart.checkout);

//route for POST to handle CART CHECKOUT ACTIONS
app.post('/cart/process', cart.process);

/*************** Error Handlers *******************/

// 404 catch-all handler (middleware)
app.use(function(req, res, next){
	res.status(404);
	res.render('404');
});

// 500 error handler (middleware)
app.use(function(err, req, res, next){
	console.error(err.stack);
	res.status(500);
	res.render('500');
});

app.listen(app.get('port'), function(){
  console.log( 'Express started on http://localhost:' +
    app.get('port') + '; press Ctrl-C to terminate.' );
});
