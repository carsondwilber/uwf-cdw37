// Require the express framework
const express = require('express');
const path = require('path');

// Create an instance of express in a variable called "app"
let app = express();

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'pug');

// Serve up static files
app.use(express.static(path.join(__dirname, 'public')));

// Require the machine controller
const ctrlMachine = require('./controllers/machine');

/* Machine pages */
// Home page - show all sodas
app.get('/', ctrlMachine.sodaList);

// Vend request - attempt to vend one of soda with id (soda_id)
app.get('/vend/:soda_id', ctrlMachine.vendSoda);

// Stock requet - attempt to stock (quantity) of soda with id (soda_id)
app.get('/stock/:soda_id/:quantity', ctrlMachine.stockSoda);

// Used for reporting and testing
app.get('/report', ctrlMachine.report);

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