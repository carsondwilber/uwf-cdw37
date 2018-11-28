/**
  Student Name: Carson Wilber
  File Name: BigNumberStandardLinkedList.java
  Project 2

  Utilizes java.util.LinkedList with Integer generic type to store BigNumber data.
*/

package JavaUtilLinkedList;

import java.util.Arrays;
import java.util.LinkedList;

import bignumber.BigNumber;

public class BigNumberStandardLinkedList extends BigNumber
{
	private LinkedList<Integer> digits;
	
	private LinkedList<Integer> temp;
	
	public BigNumberStandardLinkedList(LinkedList<Integer> digits, boolean positive)
	{
		this.digits = digits;
		this.temp = new LinkedList<Integer>();
		this.positive = positive;
	}
	
	public BigNumberStandardLinkedList()
	{
		this(new LinkedList<Integer>(), true);
	}
	
	public BigNumberStandardLinkedList(Integer[] digits, boolean positive)
	{
		this();
		
		this.positive = positive;
		
		for (Integer i : digits)
		{
			this.digits.push(i);
		}
	}
	
	public BigNumberStandardLinkedList(String fromString)
	{
		this(Arrays.stream(fromString.charAt(0) == '-' ? fromString.substring(1).split("") : fromString.split("")).map((s) -> Integer.parseInt(s)).toArray(Integer[]::new), fromString.charAt(0) != '-');
	}

	@Override
	public BigNumber negated() {
		return new BigNumberStandardLinkedList(this.digits, !this.positive);
	}

	@Override
	public int getDigit(int place)
	{
		try
		{
			return this.digits.get(place);
		}
		catch (IndexOutOfBoundsException e)
		{
			return 0;
		}
	}

	@Override
	public void setDigit(int place, int value) {
		if (this.digits.size() <= place)
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
