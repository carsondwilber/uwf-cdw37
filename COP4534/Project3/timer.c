/***************************************************************
  Student Name: Carson Wilber
  File Name: timer.c
  Project 3

  Defines timer utilities.
***************************************************************/

#include "timer.h"
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

struct timeval * t;
time_t startsec;

void startTimer()
{
	if (t)
	{
		printf("ERROR: startTimer(): Cannot start timer when another timer is already running.\n");
		return;
	}

	t = (struct timeval *)malloc(sizeof(struct timeval));

	gettimeofday(t, NULL);

	startsec = t->tv_sec;
}

time_t getElapsedTime()
{
	if (!t)
	{
		printf("ERROR: getElapsedTime(): Cannot get elapsed time of nonexistent timer.\n");
		return 0;
	}

	gettimeofday(t, NULL);

	return (t->tv_sec - startsec);
}

time_t endTimer()
{
	if (!t)
	{
		printf("ERROR: endTimer(): Cannot end nonexistent timer.\n");
		return 0;
	}

	time_t final = getElapsedTime();

	free(t);

	t = (struct timeval *)0;

	return final;
}
