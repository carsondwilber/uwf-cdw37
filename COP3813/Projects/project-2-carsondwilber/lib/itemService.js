const mongoose = require("mongoose");
const Item = mongoose.model("Item");

module.exports = {

  // Returns the inventory items that are of the desired type
  getItemsByCategory: function(category) {
    // Finds and returns all items in category
    return Item.find({'category': category}).exec();
  },

  // Returns the inventory item with the matching ID
  getItemById: function(id) {
    // Finds and returns the first item with matching id
    return Item.findOne({'id': id}).exec();
  },

  // Adds a new item to the inventory
  addInventoryItem: async function(newItem) {
    return Item.findOne({ $or: [ { 'id': newItem.id }, { 'name': newItem.name } ] })
      .then((found) => {
        if (found)
        {
          if (found.id == newItem.id)
          {
            return { success: false, message: "Item already exists with id '" + found.id + "'." };
          }
          else
          {
            return { success: false, message: "Item already exists with name '" + found.name + "'."};
          }
        }
        else
        {
          return Item.create(newItem)
            .catch((reason) => {
              return { success: false, message: reason };
            })
            .then((result) => {
              return { success: true };
            });
        }
      });
  },

  // Creates a new inventory item based on the values provided.
  // If no commission is specified, a value of .50 is set.
  createInventoryItem: function(id, category, title, description, cost,
                                  image_url, commission) {
      // Build a new inventory item from the values provided
      let newItem = {
        id: id,
        category: category,
        title: title,
        description: description,
        cost: cost,
        image_url: image_url,
      };

      // If commission is defined, use that value, otherwise use .50
      newItem.commission = commission != '' ? commission : .50;
      return newItem;
  },

  // Returns the appropriate title cased category
  titleCaseCategory: function(category) {
    if (category == "babygear")
    {
      category = "baby gear";
    }

    let categoryWords = category.split(" ");
    let capitalizedWords = [];
    categoryWords.forEach((word) => capitalizedWords.push(word.charAt(0).toUpperCase() + word.slice(1)));
    return capitalizedWords.join(" ");
  },

  getSales: function(){
    return Sale.find().exec();
  }
};
