/*
 * Student Name: Carson Wilber
 * File Name: HorseRace.java
 * Project 4
 * 
 * HorseRace is the driving JavaFX application for Project 4. It launches
 * the main application window, initializes the individual threads, handles
 * user interaction, and serves as the container for global parameters
 * regarding the race (mutex lock for race conditions, graphics parameters,
 * graphics, horses, and threads.)
 */

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.locks.ReentrantLock;

import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Pos;
import javafx.stage.Stage;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.ButtonType;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.layout.HBox;

public class HorseRace extends Application
{
	/** This constant defines the number of horses in the race. */
	private static final int HORSES = 5;
	
	/** This constant defines the preferred height of each horse. */
	private static final int HORSESCALE = 100;
	
	/** Synchronous lock for the critical section. */
	static ReentrantLock raceLock = new ReentrantLock();
	
	/** Whether or not the race is currently running. */
	static boolean racing = false;
	
	/** Whether or not the race has been won. */
	static boolean raceWon = false;
	
	/** Whether or not the program is currently exiting. */
	static boolean quitting = false;
	
	/** ID number of the winning horse, once the race has been won. */
	static int winningHorse;
	
	/** Start time, in milliseconds, of the currently running race. */
	private static long start;
	
	/** The horses. */
	private static List<Horse> horses = new ArrayList<>();
	
	/** The images of the horses on display. */
	private static List<ImageView> images = new ArrayList<>();
	
	/** The threads handling each individual horse. */
	private static List<Thread> threads = new ArrayList<>();
	
	/**
	 * Updates the image for an individual horse of id {@code id}.
	 * 
	 * @param id ID number of the horse to update
	 */
	public static void updateHorse(int id)
	{
		/*
		 * Note: the assumption is made that the given ID is valid; there is
		 *       no error checking. However, given the logic of this program,
		 *       there should never be a case where the given ID is invalid.
		 */
		
		images.get(id).setLayoutX((horses.get(id).getProgress() / 10.0d) * HORSESCALE);
	}
	
	/**
	 * Creates the alert dialog indicating the winning horse and race time.
	 */
	public static void notifyWinner()
	{
		String message = "Horse " + winningHorse + " won at " + ((System.currentTimeMillis() - start) / 1000.0d) + " seconds.";
		Alert winner = new Alert(AlertType.INFORMATION, message, ButtonType.OK);
		winner.show();
	}
	
	/**
	 * Helper function to create the Start, Reset, and Quit buttons.
	 * 
	 * @return newly created buttons
	 */
	private static List<Button> createButtons()
	{
		// Create the list of buttons
		ArrayList<Button> buttons = new ArrayList<Button>();
		
		// Create the Start Race button
		Button startRace = new Button("Start Race");
		
		// Create the event handler for the Start Race button
		startRace.setOnAction(new EventHandler<ActionEvent>()
			{
				@Override
				public void handle(ActionEvent event) {
					try
					{
						/* 
						 * Because this handler manipulates the global racing parameter,
						 * is necessary to treat it as a critical section and implement
						 * race condition prevention with the mutex lock.
						 */
						
						raceLock.lock();

						/***** BEGIN CRITICAL SECTION *****/
						
						racing = true;
						
						start = System.currentTimeMillis();
						
						/***** END CRITICAL SECTION *****/
					}
					finally
					{
						raceLock.unlock();
					}
				}
			});
		
		// Add the Start Race button to the list of buttons
		buttons.add(startRace);
		
		// Create the Reset Race button
		Button resetRace = new Button("Reset Race");
		
		// Create the event handler for the Reset Race button
		resetRace.setOnAction(new EventHandler<ActionEvent>()
			{
				@Override
				public void handle(ActionEvent event)
				{
					try
					{
						/* 
						 * Because this handler manipulates the position of each
						 * horse as well as the global parameters for the race, it
						 * is necessary to treat it as a critical section and implement
						 * race condition prevention with the mutex lock.
						 */
						
						raceLock.lock();
						
						/***** BEGIN CRITICAL SECTION *****/
						
						racing = false;
						
						for (int i = 0; i < horses.size(); ++i)
						{
							horses.get(i).reset();
							updateHorse(i);
						}
						
						raceWon = false;
						
						/***** END CRITICAL SECTION *****/
					}
					finally
					{
						raceLock.unlock();
					}
				}
			});
		
		// Add the Reset Race button to the list of buttons
		buttons.add(resetRace);
		
		// Create the Quit button
		Button quit = new Button("Quit");
		
		// Create the event handler for the Quit button
		quit.setOnAction(new EventHandler<ActionEvent>()
			{
				@Override
				public void handle(ActionEvent event)
				{
					try
					{
						raceLock.lock();
						
						racing = false;
						raceWon = false;
						quitting = true;
					}
					finally
					{
						raceLock.unlock();
					}
					
					for (Thread thread : threads)
					{
						try
						{
							thread.join();
						}
						catch (InterruptedException e)
						{
							// do nothing
						}
						
						System.exit(0);
					}
				}
			});
		
		// Add the Quit button to the list of buttons
		buttons.add(quit);
		
		return buttons;
	}
	
	/**
	 * Helper function to create the button HBox with the Start, Reset, Quit buttons.
	 * 
	 * @return newly created button bar
	 */
	private static HBox createButtonBar()
	{
		HBox buttonBar = new HBox();
		
		buttonBar.getChildren().addAll(createButtons());
		
		buttonBar.setAlignment(Pos.BASELINE_CENTER);
		
		buttonBar.setPrefWidth(HORSESCALE * 11);
		
		buttonBar.setPrefHeight(50);
		
		return buttonBar;
	}
	
	/**
	 * Helper function to create a new horse and its associated graphics components.
	 * 
	 * @param id ID number of the horse to create
	 * @return newly created horse
	 */
	private static Horse createNewHorse(int id)
	{
		Horse horse = new Horse();
		
		horses.add(horse);
		
		Image image = new Image("horse.png");
		
		ImageView view = new ImageView(image);
		
		view.setLayoutX(0);
		
		view.setLayoutY(HORSESCALE * (id + 1));
		
		view.setFitHeight(HORSESCALE);
		
		view.setFitWidth(image.getWidth() * (HORSESCALE / image.getHeight()));
		
		images.add(view);
		
		return horse;
	}
	
	public static void main(String[] args)
	{
		launch(args);
	}

	@Override
	public void start(Stage primaryStage) throws Exception
	{
		// Create the root group and scene with dimensions perfect for fitting the horses exactly, plus extra height for buttons
		Group root = new Group();
		Scene raceScene = new Scene(root, HORSESCALE * 11, (HORSES + 1) * HORSESCALE);
		
		// Create the button bar
		HBox buttonBar = createButtonBar();
		
		// Add the button bar to the scene
		root.getChildren().add(buttonBar);
		
		// Create the racetrack to hold the horses
		Group racetrack = new Group();
		
		// Create the horses
		for (int i = 0; i < HORSES; ++i)
		{
			// Create the horse
			Horse horse = createNewHorse(i);
			
			// Create the horse process
			HorseProcess process = new HorseProcess(horse, i);
			
			// Create and start the horse thread
			Thread thread = new Thread(process);
			threads.add(thread);
			thread.start();
		}
		
		// Add the horses to the racetrack
		racetrack.getChildren().addAll(images);
		
		// Add the racetrack to the scene
		root.getChildren().add(racetrack);
		
		// Begin the race scene
		primaryStage.setScene(raceScene);
		primaryStage.show();
	}

}
