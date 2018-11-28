/**
  Student Name: Carson Wilber
  File Name: Node.java
  Project 2

  Simple LinkedList generic node.
*/

package MyImplementationLinkedList;

public class Node <T>
{
	private T value;
	
	private Node<T> next = null;
	
	public Node(T value)
	{
		this.value = value;
	}
	
	public T getValue()
	{
		return this.value;
	}
	
	public Node<T> getNext()
	{
		return this.next;
	}
	
	public void setNext(Node<T> node)
	{
		this.next = node;
	}
}
