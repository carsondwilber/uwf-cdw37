let donations = []; // Will hold donations after processed from the cart

/** Adds the donation to the cart for a user, then returns them home */
const addDonation = function (req, res) {
  if (req.query.amount)
  {
    let amount = parseFloat(req.query.amount);

    if (amount > 0)
    {
      req.session.cart.push({amount: amount, message: req.query.message ? req.query.message : "N/A"});
    }
    else
    {
      req.session.flash = "Sorry, but donation amount must be positive! ;)";
    }
  }
  else
  {
    req.session.flash = "You didn't specify an amount!";
  }
  
  res.redirect("/");
};

/** Processes the checkout action, clearing the cart and returning the user home */
const processDonations = function (req, res) {
  // Set flash to user Thank You
  req.session.flash = "Thank you for your gift!";

  // Transfer all donations to server array of donations
  for (donation in req.session.cart)
  {
    donations.push(req.session.cart[donation]);
  }

  // Clear the session cart
  delete req.session.cart;
  
  // Redirect to home
  res.redirect("/");
};

/** Lets an admin view all donations made on the platform */
const viewDonations = function (req, res) {
  res.render('donations', {
    donations: donations
  });
};

module.exports = {
  addDonation,
  processDonations,
  viewDonations
};
