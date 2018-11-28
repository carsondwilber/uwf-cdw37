/*
 * Student Name: Carson Wilber
 * File Name: HorseProcess.java
 * Project 4
 * 
 * HorseProcess handles an individual Horse, utilizing global
 * parameters defined by the HorseRace. While the race is running,
 * the process uses a mutex lock to prevent a race condition, and
 * then advances the horse it handles. If the horse has won, it
 * notifies the HorseRace to complete the race and indicate the
 * winner; if the horse has not won, it simply moves the horse to
 * the appropriate position on the screen via a call to the JavaFX
 * thread and HorseRace::updateHorse(int).
 */

import javafx.application.Platform;

public class HorseProcess implements Runnable
{
	/** Horse managed by this process. */
	private Horse horse;
	
	/** Sequential ID number of this horse process. */
	private int id;

	public HorseProcess(Horse mHorse, int mId)
	{
		id = mId;
		horse = mHorse;
	}
	
	@Override
	public void run()
	{
		while (true)
		{
			try
			{
				HorseRace.raceLock.lock();
				
				/***** BEGIN CRITICAL SECTION *****/
				
				// If the race is still running...
				if (HorseRace.racing)
				{
					horse.trot();
					
					// Print out progress
					System.out.println("Horse " + id + " now at " + horse.getProgress() + ".");

					// Queue the horse position updating on the JavaFX thread
					Platform.runLater(new Runnable()
						{
							@Override
							public void run()
							{
								HorseRace.updateHorse(id);
							}
						});
					
					// If this horse has won...
					if (horse.getProgress() == 100)
					{
						// End the race with this horse winning
						HorseRace.raceWon = true;
						HorseRace.racing = false;
						HorseRace.winningHorse = id;
						
						// Queue the winning alert dialog on the JavaFX thread
						Platform.runLater(new Runnable()
							{
								@Override
								public void run()
								{
									HorseRace.notifyWinner();
								}
							});
					}
				}
				
				/***** END CRITICAL SECTION *****/
			}
			finally
			{
				HorseRace.raceLock.unlock();
			}
			
			// If the program is exiting, end this thread
			if (HorseRace.quitting)
			{
				return;
			}
			else
			{
				try
				{
					// Wait for 1 second to update again
					Thread.sleep(1000);
				}
				catch (InterruptedException e)
				{
					return;
				}
			}
		}
	}

}
