/**
  Student Name: Carson Wilber
  File Name: BigNumber.java
  Project 2

  Abstract class as an interface for BigNumber operations, independent of the underlying stack implementation.
*/

package bignumber;

public abstract class BigNumber
{
	protected boolean positive;
	
	public boolean isPositive()
	{
		return this.positive;
	}
	
	public void add(BigNumber number)
	{
		if (!number.isPositive()) // +-X + (-Y) == +-X - Y
		{
			this.subtract(number.negated());
		}
		else if (!this.isPositive()) // -X + Y == -(X - Y)
		{
			this.positive = true;
			this.subtract(number);
			this.positive = !this.positive;
		}
		else // X + Y
		{
			int order = (this.getOrder() >= number.getOrder() ? this.getOrder() : number.getOrder());
			
			this.pad(order + 1);
			number.pad(order + 1);
			
			boolean carry = false;
			int val;
			
			for (int i = 0; i < order + 1; ++i)
			{
				val = this.getDigit(i) + number.getDigit(i) + (carry ? 1 : 0);
				carry = val % 10 != val;
				this.setDigit(i, val % 10);
			}
			
			if (carry)
			{
				this.setDigit(order, 1);
			}
			
			this.trim();
			number.trim();
		}
	}
	
	public void subtract(BigNumber number)
	{
		if (!number.isPositive()) // +-X - (-Y) == +-X + Y
		{
			this.add(number.negated());
		}
		else if (!this.isPositive()) // -X - Y == -(X + Y)
		{
			this.positive = true;
			this.add(number);
			this.positive = !this.positive;
		}
		else // X - Y
		{
			if (number.getOrder() > this.getOrder())
			{
				int size = number.getOrder() + 1;
				
				this.pad(size);
				number.pad(size);
				
				// MOVE DIGITS TO TEMP
				// this:
				//    digits: empty
				//    temp: digits
				for (int x = 0; x < size; ++x)
				{
					this.tempPush(this.pop());
				}
				
				// MOVE NUMBER TO THIS
				// this:
				//    digits: number
				//    temp: digits
				for (int x = 0; x < size; ++x)
				{
					number.tempPush(number.pop());
				}
				
				for (int x = 0; x < size; ++x)
				{
					this.push(number.tempPop());
				}
				
				// MOVE TEMP TO NUMBER
				// this:
				//    digits: number
				//    temp: empty
				// number:
				//    digits: digits
				//    temp: empty
				for (int x = 0; x < size; ++x)
				{
					number.push(this.tempPop());
				}
				
				// COPY DIGITS TO TEMP
				// this:
				//    digits: number
				//    temp: number
				// number:
				//    digits: digits
				//    temp: empty
				for (int x = 0; x < size; ++x)
				{
					this.tempPush(this.getDigit(size - 1 - x));
				}
				
				this.subtract(number);
				this.positive = !this.positive;
				
				// EMPTY NUMBER
				// this:
				//    digits: number - digits
				//    temp: number
				// number:
				//    digits: empty
				//    temp: empty
				for (int x = 0; x < number.size(); ++x)
				{
					number.pop();
				}
				
				// MOVE TEMP TO NUMBER
				// this:
				//    digits: number - digits
				//    temp: empty
				// number:
				//    digits: number
				//    temp: empty
				for (int x = 0; x < this.tempSize(); ++x)
				{
					number.push(this.tempPop());
				}
				
				this.trim();
			}
			else if (number.getOrder() == this.getOrder())
			{
				for (int i  = this.getOrder(); i >= 0; --i)
				{
					if (this.getDigit(i) == number.getDigit(i))
					{
						continue;
					}
					else if (this.getDigit(i) < number.getDigit(i))
					{
						int size = this.getOrder() + 1;
						
						// MOVE DIGITS TO TEMP
						// this:
						//    digits: empty
						//    temp: digits
						for (int x = 0; x < size; ++x)
						{
							this.tempPush(this.pop());
						}
						
						// MOVE NUMBER TO THIS
						// this:
						//    digits: number
						//    temp: digits
						for (int x = 0; x < size; ++x)
						{
							number.tempPush(number.pop());
						}
						
						for (int x = 0; x < size; ++x)
						{
							this.push(number.tempPop());
						}
						
						// MOVE TEMP TO NUMBER
						// this:
						//    digits: number
						//    temp: empty
						// number:
						//    digits: digits
						//    temp: empty
						for (int x = 0; x < size; ++x)
						{
							number.push(this.tempPop());
						}
						
						// COPY DIGITS TO TEMP
						// this:
						//    digits: number
						//    temp: number
						// number:
						//    digits: digits
						//    temp: empty
						for (int x = 0; x < size; ++x)
						{
							this.tempPush(this.getDigit(size - 1 - x));
						}
						
						this.subtract(number);
						this.positive = !this.positive;
						
						// EMPTY NUMBER
						// this:
						//    digits: number - digits
						//    temp: number
						// number:
						//    digits: empty
						//    temp: empty
						for (int x = 0; x < size; ++x)
						{
							number.pop();
						}
						
						// MOVE TEMP TO NUMBER
						// this:
						//    digits: number - digits
						//    temp: empty
						// number:
						//    digits: number
						//    temp: empty
						for (int x = 0; x < size; ++x)
						{
							number.push(number.tempPop());
						}

						this.trim();
						
						return;
					}
				}
				
				while (!this.isEmpty())
				{
					this.pop();
				}
				
				this.push(0);
			}
			else // this.getOrder() > number.getOrder()
			{
				int order = this.getOrder();
				
				this.pad(order + 1);
				number.pad(order + 1);
				
				boolean borrow = false;
				int val;
				
				for (int i = 0; i < order; ++i)
				{
					val = this.getDigit(i) - number.getDigit(i) - (borrow ? 1 : 0);
					borrow = val < 0;
					this.setDigit(i, val < 0 ? val + 10 : val);
				}
				
				if (borrow)
				{
					this.positive = !this.positive;
				}

				if (this.tempSize() == 0)
				{
					this.trim();
				}
				
				number.trim();
			}
		}
	}
	
	public abstract BigNumber negated();
	
	public abstract int getDigit(int place);
	
	public abstract void setDigit(int place, int value);
	
	public int getOrder()
	{
		int nodes = this.size();
		
		while (this.getDigit(nodes - 1) == 0)
		{
			nodes--;
		}
		
		return nodes;
	}
	
	public void pad(int digits)
	{
		if (this.size() > digits)
		{
			this.trim();
		}
		
		if (this.size() < digits)
		{
			while (!this.isEmpty())
			{
				this.tempPush(this.pop());
			}
			
			for (int i = tempSize(); i < digits; ++i)
			{
				this.push(0);
			}
			
			while (!this.tempIsEmpty())
			{
				this.push(this.tempPop());
			}
		}
	}
	
	public void trim()
	{
		while (this.tempSize() > 0)
		{
			this.tempPop();
		}
		
		while (!this.isEmpty())
		{
			this.tempPush(this.pop());
		}
		
		while (this.tempPeek() == 0)
		{
			this.tempPop();
		}
		
		while (!this.tempIsEmpty())
		{
			this.push(this.tempPop());
		}
	}
	
	@Override
	public String toString()
	{
		String ret = "";
		
		for (int i = 0; i < this.size(); ++i)
		{
			ret = Integer.toString(this.getDigit(i)) + ret;
		}
		
		return (this.positive ? "" : "-") + ret;
	}
	
	public abstract int size();
	
	public abstract boolean isEmpty();
	
	public abstract Integer pop();
	
	public abstract Integer peek();
	
	public abstract void push(Integer i);
	
	public abstract int tempSize();
	
	public abstract boolean tempIsEmpty();
	
	public abstract Integer tempPop();
	
	public abstract Integer tempPeek();
	
	public abstract void tempPush(Integer i);
}
