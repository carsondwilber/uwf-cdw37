const mongoose = require('mongoose');
const User = mongoose.model('User');
const crypto = require('crypto');

function isString(v) { return Object.prototype.toString.call(v) === "[object String]"; }

function getUserByKey(key)
{
    return User.findOne({ apikey: key }).exec();
}

module.exports = {
    // auth: Takes string key and function callback
    // (Used by apikey module for access control.)
    auth: function (key, callback) {
        getUserByKey(key).then(result => {
            if (result)
            {
                callback(null, { name: result.name.first + ' ' + result.name.last, username: result.username, admin: result.admin });
            }
            else
            {
                callback(null, null);
            }
        });
    },

    // getUser: Takes string username
    getUser: async function (username) {
        // Find and return user with username matching input username.
        return User.findOne({ username: username }).exec();
    },

    createUser: async function (firstName, lastName, username, admin = false) {
        // User first name must be a string between 2 and 25 characters containing only ASCII alphabetical characters.
        if (!firstName || !isString(firstName) || !/([a-z]|[A-Z]){2,25}/.test(firstName))
        {
          return { success: false, message: 'User requires first name string between 2 and 25 characters, ASCII alphabetical only.' };
        }
        else
        {
          // User last name must be a string between 2 and 25 characters containing only ASCII alphabetical characters.
          if (!lastName || !isString(lastName) || !/([a-z]|[A-Z]){2,25}/.test(lastName))
          {
            return { success: false, message: 'User requires last name string between 2 and 25 characters, ASCII alphabetical only.' };
          }
          else
          {
            // Username must be a string between 5 and 25 characters containing only ASCII alphabetical characters, numerals and underscore.
            if (!username || !isString(username) || !/([a-z]|[A-Z]|[0-9]|_){5,25}/.test(username))
            {
              return { success: false, message: 'User requires username string between 5 and 25 characters, alphanumeric and _.' };
            }
          }
        }
        
        // Check first if user exists with same username,
        return this.getUser(username)
            .then(result => {
                if (result)
                {
                    // If so, return error that user with input username already exists.
                    return { success: false, message: 'Username already exists!' };
                }
                else
                {
                    // Else, create the user and return either success or internal error on failure.
                    return User.create({ name: { first: firstName, last: lastName }, username: username, admin: admin })
                        .then(innerResult => {
                            return { success: true, user: innerResult };
                        })
                        .catch(error => {
                            return { success: false, message: 'Internal error.' };
                        });
                }
            })
            .catch(error => {
                return { success: false, message: 'Internal error.' }
            });
    },

    // deleteUser: Takes string username
    deleteUser: async function (id) {
        // Deletes user with given username and returns sucess or returns internal error
        return User.findByIdAndDelete(id)
            .then(result => {
                return { success: true };
            })
            .catch(err => {
                return { success: false, message: 'Internal error.' }
            });
    }
};