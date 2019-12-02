/** Class representing inventory of a soda in a machine. */
class Soda {
  /**
  * @param {number} id - Id number of this soda
  * @param {string} name - The display name of the soda
  * @param {number} quantity - The number of sodas in stock
  * @param {number} cost - The cost of the soda
  * @param {boolean} caffeinated - Whether or not the soda has caffeine
  */
  constructor(id, name, quantity, cost, caffeinated){
    this.id = id;
    this.name = name;
    this.quantity = quantity;
    this.cost = cost;
    this.caffeinated = caffeinated;
    this.vended = 0;
  }

  /** 
   * Will vend a single soda if we have sufficient quantity
   * @return {boolean} Will be true if vend operation is a success
   */
  vend() {
    if(this.quantity > 0) {
      this.quantity--;
      this.vended++;
      return true;
    }
    return false;
  }

  /** 
   * Adds a certain amount to the stock
   * @param {number} toAdd - Amount to add to stock
   * @return {boolean} Will be true if stock operation is a success
   */
  stock(toAdd) {
    if(Number.isInteger(toAdd) && toAdd > 0) {
      this.quantity += toAdd;
      return true;
    }
    return false;
  }
}

module.exports = Soda;