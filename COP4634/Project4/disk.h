#ifndef DISK_H_
#define DISK_H_

/**
 * Simulates a physical disk. The disk reads and writes blocks of data.
 *
 * @author Thomas Reichherzer
 */

//defines
/** The size of a block on disk in bytes */
#define BLOCK_SIZE 1024

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "inode.h"

//prototypes
int createDisk( int sizeInKB, const char* path );
int readBlock( int blockNumber, char* blockData );
int writeBlock( int blockNumber, const char* blockData, int numberOfBytes  );
int getSize();

#define ERROR_MESSAGE_SIZE 256

//globals
int   _sizeInKB;
int   _diskLength;
FILE* _physicalDisk;
char  errorDesc[ERROR_MESSAGE_SIZE];

#endif /*DISK_H_*/
