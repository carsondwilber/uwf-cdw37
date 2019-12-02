const mongoose = require("mongoose");

let itemSchema = mongoose.Schema({
        "id": String,
        "title": String,
        "category": String,
        "description": String,
        "cost": Number,
        "commission": Number,
        "image_url": String
});

mongoose.model('Item', itemSchema);