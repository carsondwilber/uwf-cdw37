#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "parse.h"

// Status identifiers for internal state
#define RUNNING 0
#define WAITING 1
#define EXITING 2

// Maximum input buffer
#define MAXINPUT 1024

int main(int argc, char **argv)
{
	// initialize status and buffer
	char buffer[MAXINPUT];
	int shellStatus = RUNNING;
	
	// command loop
	while (shellStatus != EXITING)
	{
		// print shell prompt and read input to buffer
		printf("$$$ ");
		fgets(buffer, MAXINPUT, stdin);
		
		// remove newline
		buffer[strlen(buffer) - 1] = '\0';
		
		// exit command
		if (strcmp(buffer, "exit") == 0)
		{
			shellStatus = EXITING;
		}
		// run command (./)
		else if (buffer[0] == '.' && buffer[1] == '/')
		{
			// parse buffer into PARAM struct
			Param_t *params = parseArguments(buffer);

			// store stdin/stdout filenos for redirection
			int i = dup(fileno(stdin)), o = dup(fileno(stdout));
			
			// perform input redirect, if needed
			if (params->inputRedirect != NULL)
			{
				freopen(params->inputRedirect, "r", stdin);
			}
			
			// perform output redirect, if needed
			if (params->outputRedirect != NULL)
			{
				freopen(params->outputRedirect, "w+", stdout);
			}
			
			// enforce numChildren argument, plus any other arguments
			if (params->argumentCount >= 2)
			{
				// parse second argument as numChildren; initialize fork identifiers
				long int maxForks = strtol(params->argumentVector[1], NULL, 10);
				int nForks = 0;
				pid_t currentFork;
				
				// spawn child processes; print error, if needed
				while ((currentFork = fork()) != 0 && nForks < maxForks - 1)
				{
					if (currentFork < 0)
					{
						printf("ERROR: Could not produce one or more forks.\n");
						perror("REASON: ");
						break;
					}
					else
					{
						// increment nForks for loop and as unique identifier for each child process
						nForks++;
					}
				}
				
				// child process
				if (currentFork == 0)
				{
					// copy parameters
					Param_t * childParams = (Param_t *)malloc(sizeof(Param_t));
					childParams->argumentCount = params->argumentCount;
					
					// copy program name without run command (./) into first param
					childParams->argumentVector[0] = (char *)malloc(sizeof(char)*(strlen(params->argumentVector[0])-2));
					strcpy(childParams->argumentVector[0], params->argumentVector[0]+2);
					
					// copy unique process identifier (nForks) into third param
					childParams->argumentVector[2] = (char *)malloc(sizeof(char));
					sprintf(childParams->argumentVector[2], "%d", nForks);
					
					// copy every other parameter over
					int i;
					for (i = 1; i < childParams->argumentCount; i += (i == 1 ? 2 : 1))
					{
						childParams->argumentVector[i] = (char *)malloc(sizeof(char)*strlen(params->argumentVector[i]));
						strcpy(childParams->argumentVector[i], params->argumentVector[i]);
					}
					
					// replace current program with run program
					execv(childParams->argumentVector[0], childParams->argumentVector);
					
					// print error message and exit if failed to exec
					perror("ERROR: ");
					return 0;
				}
				// parent process
				else
				{
					int waitStatus;
					
					// change internal status to WAITING
					shellStatus = WAITING;
					
					// wait for -1 status, meaning all children have exited
					do
					{
						waitStatus = wait(NULL);
					} while (waitStatus != -1);

					// return internal status to RUNNING
					shellStatus = RUNNING;
				}
				
				// restore stdin
				dup2(i, fileno(stdin));
				close(i);
				
				// restore stdout
				dup2(o, fileno(stdout));
				close(o);
			}
		}
		// any other command
		else
		{
			// child process
			if (fork() == 0)
			{
				// parse arguments into PARAM struct
				Param_t * params = parseArguments(buffer);
				
				// exec command in child process
				execvp(params->argumentVector[0], params->argumentVector);
				
				// print error message and exit if failed to exec
				perror("ERROR: ");
				return 0;
			}
			// parent process
			else
			{
				int waitStatus;
				
				// change internal status to WAITING
				shellStatus = WAITING;
				
				// wait for -1 status, meaning child process has exited
				do
				{
					waitStatus = wait(NULL);
				} while (waitStatus != -1);
				
				// return internal status to RUNNING
				shellStatus = RUNNING;
			}
		} 
	}

	return EXIT_SUCCESS;
}


    

