const express = require('express');
const path = require('path');

var app = express();

var bodyParser = require('body-parser');
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

require("./models/db");
const postService = require('./controllers/postService');

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'pug');

// Set port app will run on
app.set('port', process.env.PORT || 3000);

app.use(express.static(path.join(__dirname, 'public')));

// When GETing home...
app.get('/', function(req, res) {
	// Load posts from database.
	let posts = postService.loadPosts();

	// Then render home page with posts.
	posts.then((list) => res.render("home", {posts: list}));
});

// When POSTing home...
app.post('/', function(req, res) {
	// Create post using parameters.
	postService.create(req.body.user, req.body.content);

	// Then redirect to home.
	res.redirect(303, "/");
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
