'use strict';

/* Define a function `addFour()` that takes a single argument 
   and returns a value 4 greater than the input.*/
function addFour(val) {
  return val + 4;
}
   
/* Create and log a variable `twelve` that is the result of passing 8 to your
   addFour() function. */
let twelve = addFour(8);
console.log(twelve);
   
/* Create and log a variable `twelveString` that is the result of passing "8" 
   (a string) to your addFour() function. Consider what this tells you about how
  the function should be explained (e.g., in a comment). */
let twelveString = addFour("8");
console.log(twelveString);
// The function turns the numerical 4 and treats it as the character '4', then appending it to the string "8". The string type, or perhaps the leftmost type, takes precedence.

/* Define a function `compoundInterest()` that takes three parameters: 
     1. an initial bank balance (principle, in dollars)
     2. an annual interest rate (as a decimal, e.g., 0.01) 
     3. a number of years
   The function should calculate the continuous compound interest
     (https://en.wikipedia.org/wiki/Compound_interest#Continuous_compounding) 
   and *return* the resulting total balance after that many number of years

   You can call the method and log the result to check your work. Compare to
     http://www.mathwarehouse.com/calculators/continuous-compound-interest-calculator.php
*/
function compoundInterest(balance, rate, years) {
  return balance * Math.exp(rate * years);
}
console.log(compoundInterest(1000, 0.05, 5)); // should be 1284.03

/* Define a function `getLetterFrequencies()` that takes in a single string as 
   an argument. The function should *return* an Object whose keys are characters
   (letters) and whose values are the number of times that character appears in
   the argument. Your function should be case sensitive.
   _Hint:_ start with an empty Object, then loop through the letters one at a
   time (you can access them with bracket notation, like with an array). For 
   each letter, increase the value associated with that key by one. Watch out 
   for if the letter is not in the Object yet!
   You can test this method with a word like "Mississippi". */
function getLetterFrequencies(str) {
  let obj = {};

  for (let i = 0; i < str.length; i++) {
    if (obj[str[i]]) {
      obj[str[i]] = obj[str[i]] + 1;
    } else {
      obj[str[i]] = 1;
    }
  }

  return obj;
}
console.log(getLetterFrequencies("testy test boondoggle")); // should be t = 4, e = 3, s = 2, y = 1, b = 1, o = 3, n = 1, d = 1, g = 2, l = 1 (also space ' ' = 2)

/* Create a variable `deck` that represents a deck of modern playing cards
   (https://en.wikipedia.org/wiki/Playing_card). This variable should be an
   *array* of 52 elements, each of which is an Object with properties:
     - `suit`, with a string value that is either `"hearts"`, `"diamonds"`, 
       `"clubs"`, or `"spades"`
     - `rank`, with an integer value ranging from 2 to 14 inclusive (values 
        11-14 represent a Jack, Queen, King, or Ace respectively).
    Tip: use a pair of nested loops to add each combination of suit and rank to 
    the `deck` array! 
    
    You can log out the `deck` to check your work! */
let deck = [];
for (let i = 0; i < 4; ++i) {
  for (let x = 2; x <= 14; ++x) {
    let card = {'rank': x};

    if (i == 0) {
      card['suit'] = "hearts";
    } else if (i == 1) {
      card['suit'] = "diamonds";
    } else if (i == 2) {
      card['suit'] = "clubs";
    } else {
      card['suit'] = "spades";
    }

    deck.push(card);
  }
}
console.log(deck); 

//You can test the below functions by creating e.g., a `pokerHand` array that 
//contains five cards from the `deck`.
let pokerHand = [deck[10], deck[6], deck[2], deck[5], deck[11]]; // Q, 8, 4, 7, K of hearts

/* Define a function `containsQueenOfHearts()` that takes in an array of "card"
   objects (e.g., a Poker hand) and returns whether or not the Queen of Hearts
   is in that array.
   Hint: use a loop to check each card. */
function containsQueenOfHearts(deckToCheck) {
  for (let i = 0; i < deckToCheck.length; ++i) {
    if (deckToCheck[i].suit == "hearts" && deckToCheck[i].rank == 12) {
      return true;
    }
  }

  return false;
}
console.log(containsQueenOfHearts(pokerHand)); // should be true
console.log(containsQueenOfHearts(pokerHand.slice(1))); // should be false

/* Define a function `getHighCard()` that takes in an array of "card" objects
  and returns the card object with the highest value. The "high card" is the one
  with the highest rank. Cards of different suits but the same rank are 
  considered to have the same value, and either is a valid result */
function getHighCard(deckToGet) {
  let highestRankCard = 0;

  for (let i = 1; i < deckToGet.length; ++i) {
    if (deckToGet[i].rank > deckToGet[highestRankCard].rank) {
      highestRankCard = i;
    }
  }

  return deckToGet[highestRankCard];
}
console.log(getHighCard(pokerHand)); // should be King of Hearts

/* Define a function `isFlush()` that takes in an array of "card" objects and
   returns whether or not the cards all have the same _suit_. */
function isFlush(deckToCheck) {
  let suitToMatch = deckToCheck[0].suit;

  for (let i = 1; i < deckToCheck.length; ++i) {
    if (deckToCheck[i].suit != suitToMatch) {
      return false;
    }
  }

  return true;
}
console.log(isFlush(pokerHand)); // should be true
console.log(isFlush(pokerHand.slice(1).concat([deck[22]]))); // should be false

/* Extra challenge: define a function `hasPair()` that takes in an array of 
   "card" objects and returns whether or not there is at least one _pair_ (two 
   cards with the same _rank_) in the array.
   Double challenge: return the rank of the pair of cards with the highest rank 
   (e.g., if the hand contains more than one pair!) */
function hasPair(deckToCheck) {
  let pairRanks = [];

  // move through the deck beginning to end
  for (let i = 0; i < deckToCheck.length - 1; ++i) {
    // move through each card in the deck after the first
    for (let x = i + 1; x < deckToCheck.length; ++x) {
      // if rank is equal, add the rank to the list of pair ranks
      if (deckToCheck[i].rank == deckToCheck[x].rank) {
        pairRanks.push(deckToCheck[i].rank);
      }
    }
  }

  // return false if no pairs
  if (pairRanks.length == 0) {
    return false;
  }
  
  // return rank if more than one pair
  if (pairRanks.length > 1) {
    pairRanks.sort();
    return pairRanks[pairRanks.length - 1];
  }

  // return true if only one pair
  return true;
}
console.log(hasPair(pokerHand)); // should be false
console.log(hasPair(pokerHand.concat([deck[23]]))); // should be true
console.log(hasPair(pokerHand.concat([deck[23], deck[24]]))); // should be 13

//Make functions and variables available to tester. DO NOT MODIFY THIS.
if(typeof module !== 'undefined' && module.exports){
  /* eslint-disable */
  if(typeof addFour !== 'undefined') 
    module.exports.addFour = addFour;
  if(typeof twelveString !== 'undefined') 
    module.exports.twelveString = twelveString;
  if(typeof compoundInterest !== 'undefined') 
    module.exports.compoundInterest = compoundInterest;
  if(typeof fizzBuzz !== 'undefined') 
    module.exports.fizzBuzz = fizzBuzz;
  if(typeof getLetterFrequencies !== 'undefined')
    module.exports.getLetterFrequencies = getLetterFrequencies;
  if(typeof deck !== 'undefined')
    module.exports.deck = deck;
  if(typeof containsQueenOfHearts !== 'undefined')
    module.exports.containsQueenOfHearts = containsQueenOfHearts;
  if(typeof getHighCard !== 'undefined')
    module.exports.getHighCard = getHighCard;
  if(typeof isFlush !== 'undefined')
    module.exports.isFlush = isFlush;
}