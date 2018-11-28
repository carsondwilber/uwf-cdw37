/**
  Student Name: Carson Wilber
  File Name: BigNumberTest.java
  Project 2

  Static BigNumber test using both custom and standard LinkedList implementations.
*/

import bignumber.BigNumber;
import MyImplementationLinkedList.BigNumberCustomLinkedList;
import JavaUtilLinkedList.BigNumberStandardLinkedList;

public class BigNumberTest
{
	public static void main(String[] args)
	{
		String value1 = "10087234";
		String value2 = "-2279823";
		String value3 = "-98723";
		
		BigNumber bn_c_1 = new BigNumberCustomLinkedList(value1);
		BigNumber bn_c_2 = new BigNumberCustomLinkedList(value2);
		BigNumber bn_c_3 = new BigNumberCustomLinkedList(value3);
		
		System.out.print("Custom #1: ");
		
		for (int i = 0; i < value1.length(); ++i)
		{
			System.out.printf("%d ", bn_c_1.getDigit(i));
		}
		
		System.out.println();
		
		System.out.print("Custom #2: ");
		
		for (int i = 0; i < value2.length(); ++i)
		{
			System.out.printf("%d ", bn_c_2.getDigit(i));
		}
		
		System.out.println();
		
		BigNumber bn_s_1 = new BigNumberStandardLinkedList(value1);
		BigNumber bn_s_2 = new BigNumberStandardLinkedList(value2);
		BigNumber bn_s_3 = new BigNumberStandardLinkedList(value3);
		
		System.out.print("Standard #1: ");
		
		for (int i = 0; i < value1.length(); ++i)
		{
			System.out.printf("%d ", bn_s_1.getDigit(i));
		}
		
		System.out.println();
		
		System.out.print("Standard #2: ");
		
		for (int i = 0; i < value2.length(); ++i)
		{
			System.out.printf("%d ", bn_s_2.getDigit(i));
		}
		
		System.out.println();
		
		bn_c_1.add(bn_c_1);
		
		System.out.printf("Custom #1 (+%s) + Custom #1 (+%s) = %s\n", value1, value1, bn_c_1.toString());

		bn_s_1.add(bn_s_3);
		
		System.out.printf("Standard #1 (+%s) + Standard #3 (%s) = %s\n", value1, value3, bn_s_1.toString());
		
		bn_c_1 = new BigNumberCustomLinkedList(value1);
		bn_c_1.subtract(bn_s_3);
		
		System.out.printf("Custom #1 (+%s) - Standard #3 (%s) = %s\n", value1, value3, bn_c_1.toString());

		bn_c_1 = new BigNumberCustomLinkedList(value1);
		bn_s_3.add(bn_c_1);
		
		System.out.printf("Standard #3 (%s) + Custom #1 (+%s) = %s\n", value3, value1, bn_s_3.toString());
		
		bn_s_2.add(bn_c_3);
		
		System.out.printf("Standard #2 (%s) + Custom #3 (%s) = %s\n", value2, value3, bn_s_2.toString());
		
		bn_c_3.subtract(bn_c_2);
		
		System.out.printf("Custom #3 (%s) - Custom #2 (%s) = %s\n", value3, value2, bn_c_3.toString());
	}
}
