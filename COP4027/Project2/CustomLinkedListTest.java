/**
  Student Name: Carson Wilber
  File Name: CustomLinkedList.java
  Project 2

  Small test class to demonstrate the proper functioning of the CustomLinkedList.
*/

import MyImplementationLinkedList.CustomLinkedList;

public class CustomLinkedListTest
{
	public static void main(String[] args)
	{
		CustomLinkedList<Integer> list = new CustomLinkedList<Integer>();
		
		list.push(1);
		list.push(2);
		list.push(3);
		
		System.out.println("Showing initial list of 1, 2, 3 pushed in order...");
		
		for (int i = 0; i < list.size(); ++i)
		{
			System.out.printf("%d ", list.get(i));
		}
		
		System.out.println();
		
		System.out.println("Demonstrating constant size after peek...");
		System.out.println("Size: " + list.size());
		System.out.println("Peek: " + list.peek());
		System.out.println("Size: " + list.size());
		
		System.out.println("Demonstrating same value after adjacent peek/pop...");
		System.out.println("Peek: " + list.peek());
		System.out.println("Pop: " + list.pop());
		
		System.out.println("Inserting 4, 5, 6...");
		
		list.push(4);
		list.push(5);
		list.push(6);
		
		for (int i = 0; i < list.size(); ++i) {
			System.out.printf("%d ", list.get(i));
		}
		
		System.out.println();
		
		System.out.println("Demonstrating change in size after pop...");
		System.out.println("Size: " + list.size());
		System.out.println("Pop: " + list.pop());
		System.out.println("Size: " + list.size());

		System.out.println("Instering 7 and showing final list...");
		
		list.push(7);
		
		for (int i = 0; i < list.size(); ++i)
		{
			System.out.printf("%d ", list.get(i));
		}
		
		System.out.println();

		System.out.println("Setting all to 9 - value and showing final list...");

		for (int i = 0; i < list.size(); ++i)
		{
			list.set(i, 9 - list.get(i));
			System.out.printf("%d ", list.get(i));
		}
		
		System.out.println();
	}
}
