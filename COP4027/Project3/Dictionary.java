/**
 * Student Name: Carson Wilber
 * File Name: Dictionary.java
 * Project 3
 *
 * Dictionary contains the main functionality of the hash set with a list of
 * valid words. The static Dictionary functions are used to load a file with
 * a newline-delimited list of valid words, add them to the 'words' HashSet,
 * and either check their existence or suggest spellings for a word that does
 * not exist.
 */

import java.util.HashSet;
import java.util.ArrayList;
import java.io.File;
import java.util.Scanner;
import java.io.FileNotFoundException;

public class Dictionary
{
	private static HashSet<String> words = new HashSet<>();
	
	public static boolean loadDictionary(String fileName)
	{
		words.clear();
		
		File inputFile = new File(fileName);
		
		try
		{
			Scanner scnr = new Scanner(inputFile);
			
			while (scnr.hasNextLine())
			{
				String word = scnr.nextLine();
				
				words.add(word.toLowerCase());
				
				if (!word.toLowerCase().equals(word))
				{
					words.add(word);
				}
			}
			
			scnr.close();
		}
		catch (FileNotFoundException fnfe)
		{
			return false;
		}
		
		return true;
	}
	
	public static boolean existsIgnoreCase(String word)
	{
		return words.contains(word.toLowerCase());
	}
	
	public static boolean exists(String word)
	{
		return words.contains(word);
	}
	
	public static ArrayList<String> suggest(String word)
	{
		System.out.println("Looking for suggestions for '" + word + "'");
		HashSet<String> suggestions = new HashSet<>();
		
		boolean capitalized = word.charAt(0) == Character.toUpperCase(word.charAt(0));
		
		boolean uppercase = word.toUpperCase().equals(word);
		
		String suggestion;
		
		// Suggest words missing one letter
		for (int p = 0; p < word.length() + 1; ++p)
		{
			for (int c = 0; c < 26; ++c)
			{
				suggestion = word.substring(0,  p) + ((char) ('a' + c)) + word.substring(p);
				
				if (capitalized)
				{
					if (uppercase)
					{
						suggestion = Character.toUpperCase(suggestion.charAt(0)) + suggestion.substring(1).toLowerCase();
					}
					else
					{
						suggestion = suggestion.toUpperCase();
					}
				}
				
				if (existsIgnoreCase(suggestion))
				{
					suggestions.add(suggestion);
				}
			}
		}
		
		// Suggest words with one less letter
		if (word.length() > 1)
		{
			for (int p = 0; p < word.length(); ++p)
			{
				suggestion = word.substring(0, p) + word.substring(p + 1);
				
				if (p == 0 && capitalized && !uppercase)
				{
					String temp = "" + Character.toUpperCase(suggestion.charAt(0));
					if (suggestion.length() > 1)
					{
						temp += Character.toLowerCase(suggestion.charAt(1));
						if (suggestion.length() > 2)
						{
							temp += suggestion.substring(2);
						}
					}

					suggestion = temp;
				}
				
				if (existsIgnoreCase(suggestion))
				{
					suggestions.add(suggestion);
				}
			}
		}
		
		// Suggest words with swapped letters
		for (int p = 0; p < word.length() - 1; ++p)
		{
			suggestion = word.substring(0, p) + word.charAt(p + 1) + word.charAt(p) + word.substring(p + 2);
			
			if (p == 0 && capitalized && !uppercase)
			{
				suggestion = Character.toUpperCase(suggestion.charAt(0)) + Character.toLowerCase(suggestion.charAt(1)) + suggestion.substring(2);
			}
			
			if (existsIgnoreCase(suggestion))
			{
				suggestions.add(suggestion);
			}
		}
		
		return new ArrayList<String>(suggestions);
	}
}
