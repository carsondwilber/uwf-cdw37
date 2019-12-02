const express = require('express');
let app = express();
let zipcodes = require('zipcodes');
let bodyParser = require('body-parser');
let credentials = require('./credentials');

// Import database
require('./models/db');

// Import services for authorization and store functionality
const authService = require('./services/authService');
const storeService = require('./services/storeService');

app.set('port', process.env.PORT || 3000);
app.set('view engine', 'pug');  

// Implement sessions (TODO: Use session variables for user sign-in & UX)
app.use(require('express-session')(credentials.session));

// Require all requests to (app)/api routes to use the header Content-Type: application/json
app.use('/api', function(req, res, next) {
  var contentType = req.headers['content-type'];

  // All requests must be of Content-Type: application/json
  if (!contentType || contentType !== "application/json")
  {
    return res.send(400);
  }

  next();
});

// Use the JSON body parser for all requests to (app)/api routes.
app.use('/api', bodyParser.json());

// Require a valid API key for any (app)/api routes.
app.use('/api', require('apikey')(authService.auth));

app.post('/api/user', function (req, res) {
  // Pass POST body to authService.createUer and print success or
  // error as JSON. Creates a user if username is not already taken
  // and user details pass validation. See authService.createUser
  // for details. Requires calling user (by API key) to be
  // administrator.
  if (req.user.admin)
  {
    authService.createUser(req.body.firstName, req.body.lastName, req.body.username, req.body.admin)
      .catch(error => res.json({ success: false, message: error }))
      .then(result => res.json(result));
  }
  else
  {
    res.status(401);
  }
});

app.delete('/api/user/:id', function (req, res) {
  // Pass POST parameters to authService.deleteUser and print success
  // or error as JSON. Deletes a user if username exists. Requires
  // calling user (by API key) to be administrator.
  if (req.user.admin)
  {
    authService.deleteUser(req.params.id)
      .catch(error => res.json({ success: false, message: error }))
      .then(result => res.json(result));
  }
  else
  {
    res.status(401);
  }
});

app.get('/api/user/:username', function (req, res) {
  // Pass GET parameters to authService.getUser and print success
  // and user details or error as JSON.
  if (req.user.admin)
  {
    authService.getUser(req.params.username)
    .then(result => res.json(result));
  }
  else
  {
    res.status(401);
  }
});

app.post('/api/store', function (req, res) {
  // Pass POST body to storeService.createStore and print success
  // or error as JSON.
  storeService.createStore(req.body.name, req.body.address, req.body.zip, req.body.inventory)
    .catch(error => res.json({ success: false, message: error }))
    .then(result => res.json(result));
});

app.get('/api/store/:id', function (req, res) {
  // Get a single store with matching id and print the result as
  // JSON for the path (app)/store/<id>.
  storeService.getStore(req.params.id)
    .catch(error => res.json({ success: false, message: error}))
    .then(result => {
      if (result)
      {
        res.json({ success: true, object: result });
      }
      else
      {
        res.json({ success: false, message: 'Store not found.' });
      }
    });
});

app.get('/api/store', function (req, res) {
  // Request all stores from storeService.getAllStores() and print
  // the result as JSON.
  storeService.getAllStores()
    .catch(error => res.json({ success: false, message: error }))
    .then(result => res.json({ success: true, object: result }));
});

app.put('/api/store/:id', function (req, res) {
  // Pass PUT parameters and body to storeService.updateStore for the
  // path (app)/store/<id> and print the result as JSON.
  storeService.updateStore(req.params.id, req.body.name, req.body.address, req.body.zip, req.body.inventory ? req.body.inventory : [])
    .catch(error => res.json({ success: false, message: error }))
    .then(result => res.json(result));
});

app.delete('/api/store/:id', function (req, res) {
  // Pass DELETE parameters to storeService.deleteStore for the path
  // (app)/store/<id> and print the result as JSON.
  storeService.deleteStore(req.params.id)
    .catch(error => res.json({ success: false, message: error }))
    .then(result => res.json(result));
});

app.get('/api/search/items', function (req, res) {
  // If no search terms provided, return 400 error with no success and an explanation.
  if (!req.query.q)
  {
    res.status(400);
    res.json({ success: false, message: 'Must provide search terms (?q=...)' });
    return;
  }

  // Use the storeService to search for matching items. See storeService.searchItems
  // for more information.
  storeService.searchItems(
    req.query.q,
    { 
      zip: req.query.zip, 
      city: req.query.city, 
      state: req.query.state, 
      radius: req.query.radius 
    })
    .catch(error => res.json({ success: false, message: error }))
    .then(result => res.json(result));
});

app.get('/search/items', function (req, res) {
  if (req.query)
  {
    if (req.query.q)
    {
      storeService.searchItems(
        req.query.q, 
        { 
          zip: req.query.zip, 
          city: req.query.city, 
          state: req.query.state, 
          radius: req.query.radius 
        })
        .catch(error => res.render('search', { message: 'Error when searching: ' + error }))
        .then(result => {
          if (result.success)
          {
            res.render('search', { results: result.object });
          }
          else
          {
            if (result.message)
            {
              res.render('search', { message: result.message });
            }
            else
            {
              res.render('search', { message: 'Internal error.' });
            }
          }
        });
    }
    else
    {
      res.render('search', { message: 'Must provide search term.' });
    }
  }
  else
  {
    res.render('search');
  }
});

// 404 catch-all handler (middleware)
app.use(function(req, res, next){
  res.status(404);
  res.json({
      "status" : "error",
      "message" : "This page can not be found."
  });
});

// 500 error handler (middleware)
app.use(function(error, req, res, next){
  console.error(error.stack);
  res.status(500);
  res.json({
      "status" : "error",
      "message" : "An internal error has occurred."
  });
});

app.listen(app.get('port'), function(){
  console.log( 'Express started on http://localhost:' +
    app.get('port') + '; press Ctrl-C to terminate.' );
});
