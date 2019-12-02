let mongoose = require("mongoose");

let postSchema = mongoose.Schema({
    user: String,
    content: String,
    posted: { type: Date, default: Date.now }
});

mongoose.model('Post', postSchema);