const mongoose = require('mongoose');
var credentials = require('../credentials.js');

// Select database connection string for development or production
let dbURI = credentials.mongo.development.connectionString;
if (process.env.NODE_ENV === 'production') {
    dbURI = credentials.mongo.production.connectionString;
}

// Define options as given
let options = { useUnifiedTopology: true, useNewUrlParser: true };

// Connect to database given database connection string and options
mongoose.connect(dbURI, options);

// Log out messages when Mongoose connection opened, closed, errored
mongoose.connection.on('connected', () => {
    console.log(`Mongoose connected to ${dbURI}`);
});
mongoose.connection.on('error', err => {
    console.log('Mongoose connection error:', err);
});
mongoose.connection.on('disconnected', () => {
    console.log('Mongoose disconnected');
});

// Graceful shutdown to close Mongoose connection
const gracefulShutdown = (msg, callback) => {
    mongoose.connection.close( () => {
        console.log(`Mongoose disconnected through ${msg}`);
        callback();
    });
};

// Graceful shutdown for different interrupts
process.on('SIGUSR2', () => {
    gracefulShutdown('nodemon restart', () => {
        process.kill(process.pid, 'SIGUSR2')
    });
});
process.on('SIGINT', () => {
    gracefulShutdown('app termination', () => {
        process.exit(0);
    });
});
process.on('SIGTERM', () => {
    gracefulShutdown('Heroku app shutdown', () => {
        process.exit(0);
    });
});

// Require specific schemas
require('./item');
require('./sale');