module.exports = {
    mongo: {
        development: {
            connectionString: 'mongodb+srv://cdw37:MDBpwd145!963@cop3813-evzlq.mongodb.net/test?retryWrites=true&w=majority'
        },
        production: {
            connectionString: 'mongodb+srv://cdw37:MDBpwd145!963@cop3813-evzlq.mongodb.net/test?retryWrites=true&w=majority'
        }
    },
    session: {
        secret: 'cocoa puffs and captain crunch',
        resave: false,
        saveUnitialized: true,
        cookie: { secure: false }
    }
}