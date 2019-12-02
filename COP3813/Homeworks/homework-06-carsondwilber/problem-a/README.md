# Fix the Coke Machine

## Task

This week's problem involves repairing this coke machine app so that it can be restocked and will vend sodas.  I've built a coke machine app for you, but it's not finished. You will have to do the following:

- Implement the `vendSoda()` and `stockSoda()` controller routes
- Add the routes to the *app.js* file along with the proper URLs
- Create the *receipt.pug* template that will show a receipt to the user

### vendSoda()
This is an express route in *controllers/machine.js* that will:

- Look for a URL parameter for the id, following this pattern: `/vend/<soda_id>`
- If we have a soda id, look for the soda in inventory that matches the id
- If there is no id, use the Error template to tell the user "Sorry, we couldn't find a soda by that id."
- Use the soda's `vend()` method to vend a soda
- Display a receipt using the *receipt.pug* template (you have to create it)

### stockSoda()
Similar to the `vendSoda()` method, you need to implement an express route in *controllers/machine.js* that will:

- Look for a URL parameter for the id, following this pattern: `/stock/<soda_id>/<quantity>`
- If we have a soda id, look for the soda in inventory that matches the id
- If there is no id, use the Error template to tell the user "Sorry, we couldn't find a soda by that id."
- If we have a valid quantity, use the soda's `stock()` method to add sodas to the machine. **Hint:** You will want to convert the parameter from a string to a number before inserting it
- Redirect the user to the home screen after the new soda has been inserted. **Hint:** look into `res.redirect()`

### receipt.pug
This is a simple template file that you will have to create in the *views* directory that will:

- Use an h1 tag to thank the user for buying a soda, for example: "Thank you for buying a Sprite!"
- Use a paragraph tag to say "Here's your receipt for $X.XX" (with the correct amount)
- Use a paragraph containing a strong tag *only if the drink is caffeinated* to warn the user "Be careful not to drink too much caffeine!"
- Offer the user a link to get back to the home page

### app.js changes
You will need to add two more routes to the *app.js* file (look for the TODO statement). These routes will have the correct URL and parameters for the vending and stocking actions. They will also connect to the corresponding controller methods.

