const postService = require('../services/postService');

const home = async function (req, res) {
  let allPosts = await postService.getAllPosts();
  res.render('home', {allPosts: allPosts});
}

const process = async function (req, res) {
  if(req.body.message && req.body.username) {
    await postService.addPost(req.body.username, req.body.message);
  }
  res.redirect('/');
}

module.exports = {
  home,
  process
};