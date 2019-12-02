const mongoose = require('mongoose');
const randToken = require('rand-token');

let userSchema = mongoose.Schema({
    name: {
        first: String,
        last: String
    },
    username: String,
    apikey: { type: String, default: function () { return randToken.generate(64); }},
    admin: { type: Boolean, default: false }
});

mongoose.model('User', userSchema);

{
    // If this is the first time the application has been run (determined
    // by whether or not any users already exist), a system administrator
    // is created and printed to the console. The sysadmin API key may be
    // used in order to perform administrative functions.
    const User = mongoose.model('User');
    User.find().exec().then(result => {
        if (result.length == 0)
        {
            User.create({ 'name.first': 'System', 'name.last': 'Administrator', 'username': 'sysadmin', 'admin': true })
                .then(result => console.log(result));
        }
    })
}