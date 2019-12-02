const Soda = require('../entities/soda');

// We start with pre-set inventory in the machine 
// (in later examples this will come from the database)
let inventory = [
  new Soda(1, 'Coke', 12, 1.50, true),
  new Soda(2, 'Dr. Pepper', 12, 1.50, true),
  new Soda(3, 'Sprite', 12, 1.50, false),
  new Soda(4, 'Iced Tea', 12, 2.00, true)
];

/**
 * Renders a page displaying the current sodas in the machine
 * @param {*} req 
 * @param {*} res 
 */
const sodaList = function (req, res) {
  res.render('soda-list', {
    title: 'Coke Machine',
    pageHeader: {
      title: 'Coke Machine Menu',
      strapline: 'Enjoy!'
    },
    sidebar: "Try a nice, refreshing coke!",
    
    // Inventory can change with each page load so it's not hard-coded
    inventory: inventory
  });
};

/** Vends a soda and displays a receipt to the user */
const vendSoda = function (req, res) {
  if (req.params.soda_id)
  {
    let find_id = parseInt(req.params.soda_id);

    if (Number.isInteger(find_id))
    {
      for (let i = 0; i < inventory.length; ++i)
      {
        if (inventory[i].id == find_id)
        {
          if (inventory[i].vend())
          {
            res.render('receipt', {
              "item": inventory[i].name,
              "price": inventory[i].cost.toFixed(2),
              "caffeinated": inventory[i].caffeinated
            });
          }
          else
          {
            res.render('error', {
              message: "Sorry, we are out of that soda."
            });
          }
          
          return;
        }
      }
    }
  }

  res.render('error', {
    message: "Sorry, we couldn't find a soda by that id."
  });
};

/** Stocks a soda by adding it to the inventory, then redirects home */
const stockSoda = function (req, res) {
  if (req.params.soda_id)
  {
    let find_id = parseInt(req.params.soda_id);

    if (Number.isInteger(find_id))
    {
      if (req.params.quantity)
      {
        let quantity = parseInt(req.params.quantity);

        if (Number.isInteger(quantity) && quantity > 0)
        {
          for (let i = 0; i < inventory.length; ++i)
          {
            if (inventory[i].id == find_id)
            {
              inventory[i].stock(quantity);

              res.redirect(302, '/');
      
              return;
            }
          }
        }
        else
        {
          res.render('error', {
            message: "Quantity of soda to stock must be number greater than 0."
          });

          return;
        }
      }
      else
      {
        res.render('error', {
          message: "Must provide a quantity to stock."
        });

        return;
      }
    }
  }

  res.render('error', {
    message: "Sorry, we couldn't find a soda by that id."
  });
};

/** Returns the inventory to the user as json */
const report = function (req, res) {
  res.json(inventory);
};

// Remember to uncomment these as well to make available to the app
module.exports = {
  sodaList: sodaList,
  vendSoda: vendSoda,
  stockSoda: stockSoda,
  report: report
};