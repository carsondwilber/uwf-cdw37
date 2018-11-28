/**
  Student Name: Carson Wilber
  File Name: CustomLinkedList.java
  Project 2

  My own implementation of a LinkedList.
*/

package MyImplementationLinkedList;

import MyImplementationLinkedList.Node;

public class CustomLinkedList <T>
{
	private Node<T> head = null;
	
	public void push(Node<T> node)
	{
		node.setNext(this.head);
		this.head = node;
	}
	
	public void push(T value)
	{
		this.push(new Node<T>(value));
	}
	
	public T peek()
	{
		return this.head.getValue();
	}
	
	public Node<T> getHead()
	{
		return this.head;
	}
	
	public T pop()
	{
		Node<T> temp = this.head;
		if (head != null) this.head = this.head.getNext();
		return temp.getValue();
	}
	
	public int size()
	{
		int nodes = 0;
		
		Node<T> node = head;
		
		while (node != null)
		{
			nodes++;
			node = node.getNext();
		}
		
		return nodes;
	}
	
	public boolean isEmpty()
	{
		return this.head == null;
	}
	
	public void set(int index, T value) throws IndexOutOfBoundsException
	{
		Node<T> newNode = new Node<T>(value);
		
		if (index == 0)
		{
			if (this.head != null)
			{
				newNode.setNext(this.head.getNext());
				this.head.setNext(null);
			}
			
			this.head = newNode;
		}
		else
		{
			Node<T> node = this.head;
			
			int i = 0;
			
			while (node != null && i < index - 1)
			{
				node = node.getNext();
				i++;
			}
			
			if (node == null)
			{
				throw new IndexOutOfBoundsException();
			}
			
			if (node.getNext() == null)
			{
				node.setNext(newNode);
			}
			else
			{
				newNode.setNext(node.getNext().getNext());
				
				node.getNext().setNext(null);
				
				node.setNext(newNode);
			}
		}
	}
	
	public T get(int index)
	{
		Node<T> node = this.head;
		
		for (int i = 0; i < index; ++i)
		{
			if (node == null) break;
			node = node.getNext();
		}
		
		if (node != null)
		{
			return node.getValue();
		}
		
		return (T) null;
	}
}
