// Require the express framework
const express = require('express');
const path = require('path');
const creds = require('./credentials');

// Create an instance of express in a variable called "app"
let app = express();

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'pug');

// Set up sessions
const session = require('express-session');
app.use(session(creds.session));

// Parse any cookies
const cookieParser = require('cookie-parser');
app.use(cookieParser());

// Serve up static files
app.use(express.static(path.join(__dirname, 'public')));

// Require the controllers
const pets = require('./controllers/pets');
const cart = require('./controllers/cart');

/* *** Middleware *** */

app.use((req, res, next) => {
  // If there's no cart in this session, initialize it
  if (!req.session.cart)
  {
    req.session.cart = [];
  }

  res.locals.cart = req.session.cart;

  next();
});

app.use((req, res, next) => {
  // Copy flash message to local and delete from session scope
  res.locals.flash = req.session.flash ? {message: req.session.flash} : null;
  delete req.session.flash;
  next();
});

/* Pet voting */
app.get('/', pets.showPets);
app.get('/vote/:pet', pets.petVote);

/* Donations */
app.get('/donate', cart.addDonation);
app.get('/donate/process', cart.processDonations);

/* Admin section */
app.get('/admin/view', cart.viewDonations);

// catch 404 and forward to error handler
app.use(function (req, res, next) {
  const err = new Error('Not Found');
  err.status = 404;
  next(err);
});

// error handler
app.use(function (err, req, res) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('system-error');
});

module.exports = app;
