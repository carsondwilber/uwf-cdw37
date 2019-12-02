let itemService = require("../lib/itemService");
const mongoose = require("mongoose");
const Sale = mongoose.model("Sale");

/** Displays the home page to the user */
exports.home = function(req, res) {
	res.render('home');
};

/** Displays a category page to the user */
exports.category = function(req, res) {
	let category = req.params.category;

	// Get the items
	let items = itemService.getItemsByCategory(category);

	// Render the template with the items we've found
	items.then((list) => 
		res.render('category', {
			cart: req.session.cart,
			items: list,
			category: itemService.titleCaseCategory(category)
		})
	);
};

/** Displays the admin interface to the user */
exports.admin = function(req, res) {
	res.render('admin');
};

/** Processes an admin form submission */
exports.process = function(req, res){
	// Create a new inventory item based on the values submitted
	let newItem = itemService.createInventoryItem(
		req.body.id,
		req.body.category,
		req.body.title,
		req.body.description,
		req.body.cost,
		req.body.image_url,
		req.body.commission	// If this is undefined we'll handle it elsewhere
	);

	// Add the item
	return itemService.addInventoryItem(newItem).then((result) => {
		if (!result.success)
		{
			req.session.flash = "Error adding new item: " + result.message;
		}
		else
		{
			req.session.flash = "Successfully added item '" + newItem.title + "' in category " + newItem.category + ".";
		}

		// Redirect the user to the correct category page
		return res.redirect(303, '/category/' + req.body.category);
	});
};

/* View all sales */
exports.sales = function(req, res){
	// Get all sales
	let sales = Sale.find().exec();

	// Render sales page
	sales.then((list) => {
		res.render('sales', { salesList: list.sort((a, b) => b.dateSold - a.dateSold) }) 
	});
};