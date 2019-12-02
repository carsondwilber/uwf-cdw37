let mongoose = require("mongoose");

// Define the crime schema
let postSchema = mongoose.Schema({
    username: String,
    message: String,
    postDate: { type : Date, default: Date.now }
});

// Register each model with Mongoose.
// There's no need to export here because mongoose is a singleton
mongoose.model('Post', postSchema);
