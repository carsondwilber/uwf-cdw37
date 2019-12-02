// Declare an object that will contain multiple middlware functions
let middleware = {};

// Inside the middleware object, declare a property named "checkCardNumbers" that contains
// a middleware function. The function should look at all query parameters and test for the
// existance of a credit card number.  We should allow the request to continue
// if no credit card number is detected.  Otherwise, return the string "Credit card detected"
// and stop the request from going further.
middleware.checkCardNumbers = function(req, res, next) {
    for (const key of Object.keys(req.query))
    {
        if (checkCardNumber(req.query[key]))
        {
            res.send("Credit card detected");
        }
    }

    next();
};

// Hint #1: You can iterate over an object's own keys using for..of and Object.keys(), like this:
// for (const key of Object.keys(obj)) {
//    console.log(key, obj[key]);
// }
// https://stackoverflow.com/questions/684672/how-do-i-loop-through-or-enumerate-a-javascript-object

// Hint #2: you can use this implementation of the luhn method to test for a valid card number.
// I recommend that you put this in it's own function outside of this object.
// https://gist.github.com/DiegoSalazar/4075533
// You can check with some sample valid credit card numbers: https://www.easy400.net/js2/regexp/ccnums.html

// Create another property called "handleErrors" with middleware function that logs them out on the screen
// using console.error(), then returns status code 500 and the string "An error has occurred"
// Hint: see "Writing error handlers" here: https://expressjs.com/en/guide/error-handling.html
middleware.handleErrors = function (err, req, res, next) {
    console.error(err.message);
    res.status(500).send("An error has occurred");
};

// Finally, you should have a middleware function called handleNotFound that deals with
// not found 404 errors.  It should return a 404 status and the text "Page not found"
middleware.handleNotFound = function (req, res, next) {
    res.status(404).send("Page not found");
}

// Export the middleware object using the module.exports variable
module.exports = middleware;

// Put your card checking method down here...
// Takes a credit card string value and returns true on valid number
function checkCardNumber(str)
{
    if (str.length != 16 || /[^0-9-\s]+/.test(str)) { return false; }

    let nCheck = 0, bEven = false;
    let value = str.replace(/\D/g, "");

    for (let n = value.length - 1; n >= 0; n--) {
        let cDigit = value.charAt(n), nDigit = parseInt(cDigit, 10);
        if (bEven && (nDigit *= 2) > 9) nDigit -= 9;

        nCheck += nDigit;
        bEven = !bEven;
    }

    return (nCheck % 10) == 0;
}