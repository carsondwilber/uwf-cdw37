const express = require('express');
const path = require('path');

var app = express();

// database setup
require("./models/db");

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'pug');

// Set port app will run on
app.set('port', process.env.PORT || 3000);

app.use(express.static(path.join(__dirname, 'public')));

// Handle form submissions
const bodyParser = require('body-parser');
app.use(bodyParser.urlencoded({ extended: true }));

// Import the controller
let main = require('./controllers/main');

// Import the post service
let postService = require('./services/postService.js');

// Routes
app.get('/', main.home);
app.post('/', main.process);

app.get('/api/all', (req, res) => {
	res.setHeader('Content-Type', 'application/json');

	postService.getAllPosts()
		.then(result => {
			if (result)
			{
				res.end(JSON.stringify(result));
			}

			res.end(JSON.stringify({message: "Internal error."}));
		}).catch(err => {
			console.error(err);
			res.end(JSON.stringify({message: "Internal error."}));
		});
});

app.post('/api/post', (req, res) => {
	res.setHeader('Content-Type', 'application/json');

	if (req.params)
	{
		if (req.body.username)
		{
			if (req.body.message)
			{
				postService.addPost(req.body.username, req.body.message)
					.then(result => {
						if (result)
						{
							res.end(JSON.stringify({message: "Successfully posted."}));
						}

						res.end(JSON.stringify({message: "Internal error."}));
					}).catch(err => {
						console.error(err);
						res.end(JSON.stringify({message: "Internal error."}));
					});
			}
			else
			{
				res.end(JSON.stringify({message: "Failed to create post. No message provided!"}));
			}
		}
		else
		{
			res.end(JSON.stringify({message: "Failed to create post. No username provided!"}));
		}
	}
	else
	{
		res.end(JSON.stringify({message: "Failed to create post. No parameters provided!"}));
	}
});

app.get('/api/delete/:id', (req, res) => {
	res.setHeader('Content-Type', 'application/json');

	if (req.params)
	{
		if (req.params.id)
		{
			postService.deletePost(req.params.id)
				.then(result => {
					if (result)
					{
						res.end(JSON.stringify({message: "Successfully deleted."}));
					}
					else
					{
						res.end(JSON.stringify({message: "Failed to delete post. Post not found or internal error occurred."}));
					}
				}).catch(err => {
					console.error(err);
					res.end(JSON.stringify({message: "Internal error."}));
				});
		}
		else
		{
			res.end(JSON.stringify({message: "Failed to delete post. No post ID provided!"}));
		}
	}
	else
	{
		res.end(JSON.stringify({message: "Failed to delete post. No parameters provided!"}));
	}
});

app.post('/api/update', (req, res) => {
	res.setHeader('Content-Type', 'application/json');

	if (req.body)
  {
    if (req.body.id)
    {
      if (req.body.message)
      {
		postService.editPost(req.body.id, req.body.message)
			.then(result => {
				if (result)
				{
					res.end(JSON.stringify({message: "Successfully updated post."}));
				}
				else
				{
					res.end(JSON.stringify({message: "Failed to update post. Post not found or internal error occurred."}));
				}
			}).catch(err => {
				console.error(err);
				res.end(JSON.stringify({message: "Internal error."}));
			});
      }
      else
      {
        res.end(JSON.stringify({message: "Failed to update post. No message provided!"}));
      }
    }
    else
    {
      res.end(JSON.stringify({message: "Failed to update post. No post ID provided!"}));
    }
  }
  else
  {
    res.end(JSON.stringify({message: "Failed to update post. No parameters provided!"}));
  }
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
