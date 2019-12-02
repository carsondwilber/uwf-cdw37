let itemService = require("../lib/itemService");
const mongoose = require("mongoose");
const Item = mongoose.model("Item");
const Sale = mongoose.model("Sale");

/** Displays the cart to the user */
exports.view = function(req, res){
  res.render('cart', { cartItems: req.session.cart });
}

/** Processes an add to cart request and then sends the user back to the cart */
exports.add = async function(req, res){
  // Get the item
  let itemId = req.params['id'];
  let item = Item.findOne({'id': itemId}).exec();

  item.then((result) => {
    // Add it to the cart
    req.session.cart.push(result);

    // Redirect user to the cart
    res.redirect("/cart/view");
  });
}

/** Processes a remove from cart request and then sends the user back to the cart */
exports.remove = function(req, res){
  // Loop through the cart and only keep items without that id
  req.session.cart = req.session.cart.filter((item) => item.id != req.params['id']);

  // Redirect user to the cart
  res.redirect("/cart/view");
};

/** Begins the checkout process, shows the checkout form */
exports.checkout = function(req, res){
  res.render('checkout');
};

const validEmailRegex = /(?:[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*|"(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21\x23-\x5b\x5d-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])*")@(?:(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?|\[(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?|[a-z0-9-]*[a-z0-9]:(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21-\x5a\x53-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])+)\])/;
const validNameRegex = /([a-z]|[A-Z]|\s)+/;

/** Processes a user's cherckout request */
exports.process = function(req, res){
  // Check for a valid email and name first
  // If invalid data was entered, redirect user and don't continue
  if (!req.body.email.match(validEmailRegex))
  {
    req.session.flash = "Email invalid.";
    return;
  }

  if (!req.body.name.match(validNameRegex))
  {
    req.session.flash = "Name invalid.";
    return;
  }

  let sale = new Sale;
  sale.name = req.body.name;
  sale.email = req.body.email;

  for (item in req.session.cart)
  {
    sale.itemsSold.push({ title: req.session.cart[item].title, cost: req.session.cart[item].cost });
  }

  console.log(sale);

  sale.save()
    .catch((reason) => {
      console.log("Error checking out: " + reason);
      req.session.flash = reason;
      res.redirect("/cart/view");
    })
    .then((result) => {
      // Clear out the contents of the cart
      delete req.session.cart;

      // Add a flash message
      req.session.flash = "Items purchased! Thank you for shopping with us.";

      // Redirect the user to the home page
      res.redirect("/");
    });
};
