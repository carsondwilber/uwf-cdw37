const express = require('express');
const path = require('path');

var app = express();

var tvService = require('./tvService');

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'pug');

// Set port app will run on
app.set('port', process.env.PORT || 3000);

app.use(express.static(path.join(__dirname, 'public')));

app.get('/', function(req, res) {
	res.render('home');
});

app.get('/search/:terms', (req, res) => {
	tvService.searchTitle({ terms: req.params.terms, decade: req.query.decade })
		.then((result) => {
			res.render('searchResults', { items: result.data, terms: req.params.terms, filter: result.filter });
		});
});

app.get('/details/:showId', (req, res) => {
	tvService.fetchDetails({ showId: req.params.showId })
		.then((result) => {
			res.render('details', { details: result });
		});
});

app.get('/reset/:terms', (req, res) => {
	tvService.clearFilter();
	res.redirect('/search/' + req.params.terms);
});

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