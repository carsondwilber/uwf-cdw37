/***************************************************************
  Student Name: Carson Wilber
  File Name: timer.h
  Project 3

  Declares timer utilities.
***************************************************************/

#include <sys/time.h>
#include <time.h>

#ifndef TIMER_H_
#define TIMER_H_

/**
 * Begins the timer with the current time.
 */
void startTimer();

/**
 * Retrieves the elapsed time without ending the timer.
 */
time_t getElapsedTime();

/**
 * Stops the timer and returns the final elapsed time.
 */
time_t endTimer();

#endif /* TIMER_H_ */
