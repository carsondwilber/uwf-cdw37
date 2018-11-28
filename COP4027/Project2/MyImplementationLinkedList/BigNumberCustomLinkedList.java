/**
  Student Name: Carson Wilber
  File Name: BigNumberCustomLinkedList.java
  Project 2

  Utilizes CustomLinkedList with Integer generic type to store BigNumber data.
*/

package MyImplementationLinkedList;

import java.util.Arrays;

import bignumber.BigNumber;

public class BigNumberCustomLinkedList extends BigNumber
{
	private CustomLinkedList<Integer> digits;
	
	private CustomLinkedList<Integer> temp;
	
	public BigNumberCustomLinkedList(CustomLinkedList<Integer> digits, boolean positive)
	{
		this.digits = digits;
		this.temp = new CustomLinkedList<Integer>();
		this.positive = positive;
	}
	
	public BigNumberCustomLinkedList()
	{
		this(new CustomLinkedList<Integer>(), true);
	}
	
	public BigNumberCustomLinkedList(Integer[] digits, boolean positive)
	{
		this();
		
		this.positive = positive;
		
		for (Integer i : digits)
		{
			this.digits.push(i);
		}
	}
	
	public BigNumberCustomLinkedList(String fromString)
	{
		this(Arrays.stream(fromString.charAt(0) == '-' ? fromString.substring(1).split("") : fromString.split("")).map((s) -> Integer.parseInt(s)).toArray(Integer[]::new), fromString.charAt(0) != '-');
	}

	@Override
	public BigNumber negated() {
		return new BigNumberCustomLinkedList(this.digits, !this.positive);
	}

	@Override
	public int getDigit(int place)
	{
		Integer val = this.digits.get(place);
		
		if (val == null)
		{
			return 0;
		}
		
		return val;
	}

	@Override
	public void setDigit(int place, int value)
	{
		if (this.size() <= place)
		{
			this.pad(place + 1);
		}
		
		this.digits.set(place, value);
	}

	@Override
	public int size() {
		return this.digits.size();
	}

	@Override
	public boolean isEmpty() {
		return this.digits.isEmpty();
	}

	@Override
	public Integer pop() {
		return this.digits.pop();
	}

	@Override
	public Integer peek() {
		return this.digits.peek();
	}

	@Override
	public void push(Integer i) {
		this.digits.push(i);
	}

	@Override
	public int tempSize() {
		return this.temp.size();
	}

	@Override
	public boolean tempIsEmpty() {
		return this.temp.isEmpty();
	}

	@Override
	public Integer tempPop() {
		return this.temp.pop();
	}

	@Override
	public Integer tempPeek() {
		return this.temp.peek();
	}

	@Override
	public void tempPush(Integer i) {
		this.temp.push(i);
	}
}
