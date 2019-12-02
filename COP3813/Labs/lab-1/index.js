let readlineSync = require('readline-sync');

let amount, time, rate;
const COMPOUNDED = 4;

while (!amount || amount < 0.01)
{
	amount = parseFloat(readlineSync.question("Please enter the amount borrowed in dollars: "));
}

// Ask the user for the time period (in years)
while (!time || time <= 0)
{
	time = parseInt(readlineSync.question("Please enter the loan period in years: "));
}

// Ask the user for the interest rate
while (!rate || rate <= 0 || rate > 33)
{
	rate = parseFloat(readlineSync.question("Please enter the interest rate in percent: "));
}

console.log("The principal plus interest is: $" + calculateInterest(amount, time, rate));

// Calculate loan interest given amount, period, and rate
function calculateInterest (amount, time, rate) {
	const pct = rate / 100;
	return (amount * Math.pow((1 + (pct / COMPOUNDED)), COMPOUNDED * time)).toFixed(2);
}