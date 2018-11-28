/**
  Student Name: Carson Wilber
  File Name: JavaUtilLinkedList.java
  Project 2

  Utilizes the LinkedList based on the standard Java implementation to perform BigNumber operations.
*/

package JavaUtilLinkedList;

import java.io.File;
import java.io.IOException;
import java.util.Scanner;

public class JavaUtilLinkedList
{
	public static void main(String[] args)
	{
		BigNumberStandardLinkedList number1, number2;
		String op;
		
		File file = new File("addsAndSubtracts.txt");
		
		Scanner scnr = null;
		
		try
		{
			scnr = new Scanner(file);
		}
		catch (IOException e)
		{
			e.printStackTrace();
		}
		
		if (scnr != null)
		{
			while (scnr.hasNextLine())
			{
				number1 = new BigNumberStandardLinkedList(scnr.next());
				op = scnr.next();
				number2 = new BigNumberStandardLinkedList(scnr.next());
				
				if (op.equals("+"))
				{
					System.out.print(number1.toString());
					System.out.print(" + ");
					System.out.print(number2.toString());
					System.out.print(" = ");
					
					number1.add(number2);
					
					System.out.println(number1.toString());
				}
				else if (op.equals("-"))
				{
					System.out.print(number1.toString());
					System.out.print(" - ");
					System.out.print(number2.toString());
					System.out.print(" = ");
					
					number1.subtract(number2);
					
					System.out.println(number1.toString());
				}
				else
				{
					System.out.printf("Unsupported operation: '%s'\n", op);
				}
			}
			
			scnr.close();
		}
	}
}
