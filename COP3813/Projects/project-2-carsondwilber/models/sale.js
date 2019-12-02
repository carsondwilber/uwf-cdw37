const mongoose = require("mongoose");

let subItemSchema = mongoose.Schema({
    "title": String,
    "cost": Number
});

let saleSchema = mongoose.Schema({
    "email": String,
    "name": String,
    "dateSold": { type: Date, default: Date.now },
    "itemsSold": [subItemSchema]
});

mongoose.model('Sale', saleSchema);