/*
 * Student Name: Carson Wilber
 * File Name: Horse.java
 * Project 4
 * 
 * The Horse class contains a minimal set of instance variables
 * for each individual horse. It contains no interaction with the
 * HorseRace parameters and does not itself provide any race
 * condition prevention. None of its instance fields can be
 * directly manipulated.
 */

import java.util.Random;

public class Horse
{
	/** Current progress of this horse. */
	private int progress = 0;
	
	/** Speed of this horse. */
	private int speed;
	
	public Horse()
	{
		// Initialize speed to random number 1..10
		speed = new Random(System.currentTimeMillis()).nextInt(9) + 1;
	}
	
	/**
	 * Reset the horse with no progress.
	 */
	public void reset()
	{
		progress = 0;
	}
	
	/**
	 * Move this horse's progress forward one step at the current speed.
	 */
	public void trot()
	{
		progress += speed;
		
		if (progress > 100) progress = 100;
	}
	
	public int getProgress()
	{
		return progress;
	}
}
