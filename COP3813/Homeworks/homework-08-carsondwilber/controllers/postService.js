const mongoose = require("mongoose");
const Post = mongoose.model("Post");

let postService = {
    loadPosts: () => {
        // Simply return promise for posts from database
        return Post.find().exec();
    },

    create: (user, content) => {
        // Check user provided, else log error and continue
        if (user)
        {
            // Check content provided, else log error and continue
            if (content)
            {
                // Create post if both are present
                Post.create({user: user, content: content});
            }
            else
            {
                console.error("Requires content to post!");
            }
        }
        else
        {
            console.error("Requires a user to post!");
        }
    }
};

module.exports = postService;