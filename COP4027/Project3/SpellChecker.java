/**
 * Student Name: Carson Wilber
 * File Name: SpellChecker.java
 * Project 3
 *
 * SpellChecker is the main JavaFX application. It launches the window where a
 * file may be loaded into and provides all spell checking functionality.
 * When words are misspelled, their suggested spellings are opened in a separate
 * alert window.
 */

import java.io.File;
import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.control.Menu;
import javafx.scene.control.MenuBar;
import javafx.scene.control.MenuItem;
import javafx.scene.input.MouseEvent;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.control.Label;
import javafx.scene.layout.VBox;
import javafx.scene.text.Text;
import javafx.stage.FileChooser;
import javafx.stage.Stage;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Scanner;

public class SpellChecker extends Application
{
	private static MenuBar menuBar = new MenuBar();
	
	private static Text text = new Text();
	
	private static VBox vbox = new VBox();
	
	private static Scene scene = new Scene(vbox, 400, 400);
	
	private static String replaceWithEmpty = "!?.,()[] {}`<>#\"";
	
	private static String replaceWithSpace = "~-+/";
	
	public static void main(String[] args)
	{
		if (Dictionary.loadDictionary("dictionary.txt"))
		{
			launch(args);
		}
		else
		{
			System.err.println("ERROR: Could not load dictionary.txt.");
		}
	}

	@Override
	public void start(Stage primaryStage) throws Exception {
		primaryStage.setTitle("Spell Checker by Carson Wilber");
		
		vbox.getChildren().addAll(menuBar, text);
		
		primaryStage.setScene(scene);
		
		Menu fileMenu = new Menu("File");
		
		MenuItem openFileMenuItem = new MenuItem("Open");
		
		openFileMenuItem.setOnAction(new EventHandler<ActionEvent>() {
			@Override
			public void handle(ActionEvent event) {
				 FileChooser fileChooser = new FileChooser();
				 fileChooser.setTitle("Open File");
				 File selectedFile = fileChooser.showOpenDialog(primaryStage);
				 
				 if (selectedFile != null)
				 {
					 try
					 {
						 Scanner scnr = new Scanner(selectedFile);
						 
						 while (scnr.hasNextLine())
						 {
							 text.setText(text.getText() + scnr.nextLine() + "\n");
						 }
						 
						 scnr.close();
					 }
					 catch (FileNotFoundException e)
					 {
						 
					 }
				 }
			}
		});
		
		MenuItem saveFileMenuItem = new MenuItem("Save");
		
		saveFileMenuItem.setOnAction(new EventHandler<ActionEvent>() {
			@Override
			public void handle(ActionEvent event) {
				 FileChooser fileChooser = new FileChooser();
				 fileChooser.setTitle("Open File");
				 File selectedFile = fileChooser.showOpenDialog(primaryStage);
				 
				 try
				 {
					 PrintWriter prnt = new PrintWriter(selectedFile);
					 
					 prnt.write(text.getText());
					 
					 prnt.close();
				 }
				 catch (FileNotFoundException e)
				 {
					 
				 }
			}
		});
		
		MenuItem exitMenuItem = new MenuItem("Exit");
		
		exitMenuItem.setOnAction(new EventHandler<ActionEvent>() {
			@Override
			public void handle (ActionEvent event) {
				System.exit(0);
			}
		});
		
		fileMenu.getItems().addAll(openFileMenuItem, saveFileMenuItem, exitMenuItem);
		
		menuBar.getMenus().add(fileMenu);
		
		Menu spellCheckMenu = new Menu("Edit");
		
		MenuItem spellCheckMenuItem = new MenuItem("Spell Check");

		spellCheckMenuItem.setOnAction(new EventHandler<ActionEvent>() {
			@Override
			public void handle(ActionEvent event) {
				spellCheck();
			}
		});

		spellCheckMenu.getItems().add(spellCheckMenuItem);
		
		menuBar.getMenus().add(spellCheckMenu);
		
		primaryStage.setScene(scene);
		
		primaryStage.show();
	}
	
	private static ArrayList<String> phraseToWords(String word)
	{
		for (Character c : replaceWithEmpty.toCharArray())
		{
			word = word.replace("" + c, "");
		}
		
		for (Character c : replaceWithSpace.toCharArray())
		{
			word = word.replace(c, ' ');
		}
		
		String[] tokens = word.split(" ");
		
		ArrayList<String> words = new ArrayList<String>();
		
		for (String token : tokens)
		{
			if (token.length() > 0)
			{
				words.add(token);
			}
		}
		
		return words;
	}
	
	private static void spellCheck()
	{
		Scanner scnr = new Scanner(text.getText());
		
		while (scnr.hasNext())
		{
			String next = scnr.next();
			
			ArrayList<String> words = phraseToWords(next);
			
			for (String word : words)
			{
				if (!Dictionary.exists(word))
				{
					Alert spellCheckDialog = new Alert(AlertType.WARNING);
					
					spellCheckDialog.setTitle("Spell Check");
					
					spellCheckDialog.setHeaderText("Spelling suggestions for '" + word + "'");
					
					spellCheckDialog.setContentText(String.join("\n", Dictionary.suggest(word)));
					
					spellCheckDialog.showAndWait();
				}
			}
		}
		
		scnr.close();
	}
}
