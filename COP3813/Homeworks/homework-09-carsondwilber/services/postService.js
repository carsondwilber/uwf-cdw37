const mongoose = require('mongoose');
const Post = mongoose.model('Post');

module.exports = {

	// Returns all posts in the db
	getAllPosts: function () {
		return Post.find().exec()
			.catch(err => {
				console.error(err);
				return [];
			})
			.then(result => {
				return result;
			});
	},

	// Adds a post to the db
	addPost: function (username, message) {
		return Post.create({
			username: username,
			message: message
		})
			.catch(err => {
				console.error(err);
				return [];
			})
			.then(result => {
				return result;
			}, result => { return null; });
	},

	// Deletes a post in the db
	deletePost: async function (id) {
		return Post.deleteOne({
			_id: id
		})
			.catch(err => {
				console.error(err);
				return null;
			})
			.then(result => { return true; }, result => { return false; });
	},

	// Edits a post in the db
	editPost: async function (id, message) {
		return Post.updateOne({
			_id: id
		}, {
			message: message
		})
			.catch(err => {
				console.error(err);
				return null;
			})
			.then(result => { return true; }, result => { return false; });
	}

};