const mongoose = require('mongoose');
const Store = mongoose.model('Store');
const zipcodes = require('zipcodes');

function isString(v) { return Object.prototype.toString.call(v) === "[object String]"; }

// validStoreObject: Validates given values for a store and either returns success
// or returns an error message explaning the validation error.
function validStoreObject(name, address, zip, inventory = []) {
    // Store names must be string between 4 and 50 characters
    if (!isString(name) || name.length < 4 || name.length > 50)
    {
        return {
            success: false,
            message: 'Store names must be string between 4 and 50 characters.'
        };
    }

    // Address must be string between 6 and 50 characters
    if (!isString(address) || address.length < 6 || address.length > 50)
    {
        return {
            success: false,
            message: 'Address must be string between 6 and 50 characters.'
        };
    }

    // ZIP code must be string of 5 numeric characters
    if (!isString(zip) || !/[0-9]{5}/.test(zip))
    {
        return {
            success: false,
            message: 'ZIP code must be string of 5 numeric characters.'
        };
    }

    // Inventory must be an array
    if (!Array.isArray(inventory))
    {
        return {
            success: false,
            message: 'Inventory must be array.'
        };
    }

    // Check each inventory item because...
    for (let i = 0; i < inventory.length; i++)
    {
        // Item name must be string between 4 and 40 characters
        if (!inventory[i].name || !isString(inventory[i].name) || inventory[i].name.length < 4 || inventory[i].name.length > 40)
        {
            return {
                success: false,
                message: 'Inventory item must have a name string between 4 and 40 characters.'
            };
        }

        // Item description must be string between 4 and 60 characters
        if (!inventory[i].description || !isString(inventory[i].description) || inventory[i].description.length < 4 || inventory[i].description.length > 60)
        {
            return {
                success: false,
                message: 'Inventory item must have a description string between 4 and 60 characters.'
            };
        }

        // Item quantity must be integer greater than 0
        if (!inventory[i].quantity || !Number.isInteger(inventory[i].quantity) || inventory[i].quantity < 1)
        {
            return {
                success: false,
                message: 'Inventory item must have an integral quantity greater than 0.'
            };
        }

        // If report date provided,
        if (inventory[i].reportDate)
        {
            // Item report date must not be in the future
            if (Date.parse(inventory[i].reportDate) > Date.now)
            {
                return {
                    success: false,
                    message: 'Inventory item cannot be reported at a future date.'
                };
            }
        }
    }

    // If all above validation checks pass, return success
    return { success: true };
}

function findZIPs(zip, city, state, radius)
{
  // Initialize ZIP array with provided ZIP code, if available; else, null.
  let zips = zip ? [ zip ] : null;

  // Initialize radius with provided radius value, if available; else, default to 20 miles.
  if (!radius) radius = 20;
  
  // If no ZIP provided but city and state are provided,
  if (zips == null && city && state)
  {
    // Initialize ZIP array to an empty array,
    zips = [];

    // Find ZIP codes in the given city and state,
    let result = zipcodes.lookupByName(city, state);

    // For each ZIP code in the city and state,
    if (result.length > 0)
    {
      for (r of result)
      {
        // Add only the ZIP (not lat/long, other details) to the ZIP array.
        zips.push(r.zip);
      }
    }
  }

  // If ZIPs and a search radius exist between 0 and 500 miles,
  if (zips != null && radius > 0 && radius <= 500)
  {
    // Copy the ZIP array,
    let tmp_zips = zips.slice();

    // For each ZIP in the array,
    for (z of tmp_zips)
    {
      // Search for ZIP codes within the given search radius,
      let result = zipcodes.radius(z, radius);

      // Add each matching ZIP code to the ZIP array.
      for (zip of result)
      {
        zips.push(zip);
      }
    }
  }

  return zips;
}

module.exports = {
    // createStore: Takes string name, string address, string zip, and optional array inventory
    createStore: async function (name, address, zip, inventory = []) {
        // Validates that values create a valid store object (see validStoreObject for details)
        let validated = validStoreObject(name, address, zip, inventory);

        // If not validated, return error message from validStoreObject;
        // otherwise, create a store with the input values, and either
        // return internal error or success
        return (!validated.success)
            ? validated
            : Store.create({ name: name, address: address, zip: zip, inventory: inventory })
                .then(result => {
                    return { success: true };
                })
                .catch(reason => {
                    return { success: false, message: 'Internal error.' };
                });
    },

    // getStore: Takes string id
    getStore: async function (id) {
        // Find and return single store, or empty object if not found
        return Store.findOne({
            _id: id
        }).exec();
    },

    // getAllStores: Takes no arguments
    getAllStores: async function () {
        // Return all stores, or empty object if none exist
        return Store.find(null, { _id: false, 'inventory._id': false }).exec();
    },

    // updateStore: Takes string id, string name, string address, string zip, and optional array inventory
    updateStore: async function (id, name, address, zip, inventory = [])
    {
        // Validates that values create a valid store object (see validStoreObject for details)
        let validated = validStoreObject(name, address, zip, inventory);

        // If not validated, return error message from validStoreObject;
        // otherwise, create a store with the input values, and either
        // return internal error or success
        return (!validated.success)
            ? validated
            : Store.findByIdAndUpdate(id, { name: name, address: address, zip: zip, inventory: inventory })
                .then(result => {
                    return { success: true };
                })
                .catch(err => {
                    return { success: false, message: 'Internal error.' };
                });
    },

    // deleteStore: Takes string id
    deleteStore: async function (id)
    {
        // Deletes store with given id and returns sucess or returns internal error
        return Store.findByIdAndDelete(id)
            .then(result => {
                return { success: true };
            })
            .catch(err => {
                console.error(err);
                return { success: false, message: 'Internal error.' }
            });
    },

    // searchItems: Takes string searchTerm and optional array zips
    searchItems: async function (searchTerm, searchParams)
    {
        let zips = findZIPs(searchParams.zip, searchParams.city, searchParams.state, searchParams.radius);

        // If ZIP codes used for search,
        if (zips)
        {
            // Find all inventory items with name matching the searchTerm and
            // which are contained within stores matching one of the provided
            // ZIP codes. Only return matching inventory items and the details
            // of the store in which they are contained.

            // If internal error occurs, return internal error, else return
            // success and object with all matching items.
            return Store.find({
                $or: [
                    { 'inventory.name': { $regex: searchTerm, $options: 'i' } },
                    { 'inventory.description': { $regex: searchTerm, $options: 'i' } }
                ],
                'zip': { $in: zips }
            }, {
                "_id": false,
                "name": true,
                "address": true,
                "zip": true,
                inventory: { 
                    $elemMatch: { 
                        $or: [ 
                            { 'name': { $regex: searchTerm, $options: 'i' } },
                            { 'description': { $regex: searchTerm, $options: 'i' } } 
                        ]
                    }
                }
            }).exec()
            .then(result => {
                return { success: true, object: result };
            })
            .catch(error => {
                return { success: false, message: 'Internal error.' };
            });
        }

        // If no ZIPs are provided, match globally any items which match
        // the given searchTerm. If internal error occurs, return internal
        // error, else return success and object with all matching items.
        return Store.find({
            $or: [
                { 'inventory.name': { $regex: searchTerm, $options: 'i' } },
                { 'inventory.description': { $regex: searchTerm, $options: 'i' } }
            ]
        }, {
            "_id": true,
            "name": true,
            "address": true,
            "zip": true,
            inventory: {
                $elemMatch: { 
                    $or: [
                        { 'name': { $regex: searchTerm, $options: 'i' } },
                        { 'description': { $regex: searchTerm, $options: 'i' } }
                    ]
                }
            }
        }).exec()
        .then(result => {
            return { success: true, object: result };
        })
        .catch(error => {
            return { success: false, message: 'Internal error.' };
        });
    }
};