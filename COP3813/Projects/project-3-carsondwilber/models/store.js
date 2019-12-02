const mongoose = require('mongoose');

let itemSchema = mongoose.Schema({
    name: String,
    description: String,
    quantity: Number,
    reportDate: { type: Date, default: Date.now }
})

// Define the store schema
let storeSchema = mongoose.Schema({
    name: String,
    address: String,
    zip: String,
    inventory: [itemSchema]
});

// Register each model with Mongoose.
mongoose.model('Store', storeSchema);