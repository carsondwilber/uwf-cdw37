module.exports = {
    mongo: {
        development: {
            connectionString: '[REDACTED]'
        },
        production: {
            connectionString: '[REDACTED]'
        }
    },
    session: {
        secret: 'topkek inspector gadget',
        resave: false,
        saveUninitialized: true,
        cookie: { secure: false }
    }
};