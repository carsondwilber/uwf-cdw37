/**
 * Emulates a file system.
 *
 * @author Thomas Reichherzer
 */

#include "fileSystem.h"

void readSuperBlock( superBlock_t* superBlock );
void writeSuperBlock( superBlock_t * superBlock );
void readInode( inode_t* inode, int blockNumber );
void writeInode( inode_t* inode, int blockNumber );
void printInode( inode_t *dInode );
void printInodeAtBlock( int block );
void printInodes();
void printFreeBlocks();
int numUsedBlocksByInode( inode_t *dInode );
int numUsedBlocksByInodeAtBlock( int block );
int numUsedInodes();
int numFreeInodes();
int numUsedBlocks();
int numFreeBlocks();
int checkIntegrity( int diskSizeInKB );
void intToByteArray( int value, char* buffer, int pos );
int byteArrayToInt( char* buffer, int pos );
int isOpen( int inodeBlockNum );
fd_t * createfd( int inodeBlockNum );

fd_t * oft[NUM_INODES];
int open = 0;

/**
 * A test program for testing file system operations.
 *
 * @param args - a list of arguments
 */
int main(int argc, char* argv[])
{
	// TEST FORMAT DISK

	int error = format( 100, "./disk1.bin" );

	if (error != 0)
	{
		printf( "[ ER ] %s", errorDesc );
		return -1;
	}

	superBlock_t dSuperBlock;

	readSuperBlock( &dSuperBlock );

	printf( "[ IF ] First free block: %d\n", dSuperBlock.firstFreeBlock );

	// TEST INTEGRITY AND PRINT DEBUG INFORMATION
	{
		printInodes();

		printFreeBlocks();

		error = checkIntegrity( 100 );

		if (error != 0)
		{
			printf( "[ ER ] Integrity of disk is corrupt: \"%s\"\n", errorDesc );
			return -1;
		}

		printf( "[ OK ] Disk maintains integrity\n" );
	}

	// TEST OPEN FILE

	fd_t *newFile = openf( "test.dat" );

	if (newFile == NULL)
	{
		printf( "[ ER ] %s", errorDesc );
		return -1;
	}

	printf( "[ IF ] inodes after creating test.dat:\n" );

	// TEST INTEGRITY AND PRINT DEBUG INFORMATION
	{
		printInodes();

		printFreeBlocks();

		error = checkIntegrity( 100 );

		if (error != 0)
		{
			printf( "[ ER ] Integrity of disk is corrupt: \"%s\"\n", errorDesc );
			return -1;
		}

		printf( "[ OK ] Disk maintains integrity\n" );
	}

	// TEST OPEN ALREADY OPENED FILE
	{
		fd_t *reopenFile = openf( "test.dat" );

		if (reopenFile != NULL)
		{
			printf( "[ ER ] %s", errorDesc );
			return -1;
		}

		printf( "[ OK ] Failed to reopen already opened file\n" );
	}

	// TEST WRITE LESS THAN ONE FULL BLOCK TO FILE
	{
		char *writeData = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum. Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lo\0";

		error = writef( newFile, writeData, 1024 );

		if (error != 0)
		{
			printf( "[ ER ] Failed to write 1024 bytes of data to new file.\n" );
			return -1;
		}

		printf( "[ OK ] Write 1024 bytes of data to new file.\n" );

		printInodeAtBlock( newFile->inodeBlockNum );

		printFreeBlocks();
	}

	// TEST WRITE OVERAGE INTO FILE
	{
		char *writeData = "Continue writing, overflowing to new block\0";

		error = writef( newFile, writeData, 43 );

		if (error != 0)
		{
			printf( "[ ER ] Failed to write 43 more bytes of data to new file and into new block.\n" );
			return -1;
		}

		printf( "[ OK ] Wrote 43 more bytes of data to new file and into new block.\n" );

		printInodeAtBlock( newFile->inodeBlockNum );

		printFreeBlocks();
	}

	// TEST READ NO BYTES FROM FILE
	{
		error = readf( newFile, NULL, 0 );

		if (error == 0)
		{
			printf( "[ ER ] Was able to attempt read 0 bytes from file.\n" );
			return -1;
		}

		printf( "[ OK ] Was unable to attempt read 0 bytes from file.\n" );
	}

	// TEST READ BYTES FROM FILE INTO NULL BUFFER
	{
		error = readf( newFile, NULL, 1 );

		if (error == 0)
		{
			printf( "[ ER ] Was able to attempt read data from file into null buffer.\n" );
			return -1;
		}

		printf( "[ OK ] Was unable to attempt read data from file into null buffer.\n" );
	}

	// TEST CLOSE FILE
	{
		error = closef( newFile );

		if (error != 0)
		{
			printf( "[ ER ] Failed to close file: \"%s\".\n", errorDesc );
			return -1;
		}

		printf( "[ OK ] Successfully closed file.\n" );
	}

	// TEST READ FROM CLOSED FILE
	{
		error = readf( newFile, NULL, 0 );

		if (error == 0)
		{
			printf( "[ ER ] Was able to attempt read data from closed file.\n" );
			return -1;
		}

		printf( "[ OK ] Was unable to attempt read data from closed file.\n" );
	}

	// TEST CLOSE ALREADY CLOSED FILE
	{
		error = closef( newFile );

		if (error == 0)
		{
			printf( "[ ER ] Was able to close already closed file.\n" );
			return -1;
		}

		printf( "[ OK ] Was unable to close already closed file.\n" );
	}

	// TEST REOPEN FILE

	fd_t *file = openf( "test.dat" );

	if (file == NULL)
	{
		printf( "[ ER ] Was unable to reopen file.\n" );
		return -1;
	}

	printf( "[ OK ] Was able to reopen file.\n" );

	// TEST READ LESS THAN ONE BLOCK FROM FILE
	{
		char buffer[44];

		error = readf( file, buffer, 43 );

		if (error != 0)
		{
			printf( "[ ER ] Error reading 43 bytes data from file: \"%s\"\n", errorDesc );
			return -1;
		}

		buffer[43] = '\0';

		printf( "[ OK ] Read 43 bytes data from file: \"%s\"\n", buffer );
	}

	// TEST READ OVERAGE FROM FILE
	{
		char buffer[1024];

		error = readf( file, buffer, 1023 );

		if (error != 0)
		{
			printf( "[ ER ] Error reading 1023 bytes from file starting at byte 44: \"%s\"\n", errorDesc );
			return -1;
		}

		buffer[1023] = '\0';

		printf( "[ OK ] Read 1023 bytes data from file at byte 44: \"%s\"\n", buffer );
	}

	// TEST CLOSE REOPENED FILE
	{
		error = closef( file );

		if (error != 0)
		{
			printf( "[ ER ] Error closing file: \"%s\"\n", errorDesc );
			return -1;
		}

		printf( "[ OK ] Successfully closed file.\n" );
	}

	printf( "[ OK ] Successfully passed all tests. Exiting now.\n" );

	return 0;
}

int format( int diskSizeInKB, char* path )
{
	int error;

    // create a disk
    error = createDisk( diskSizeInKB, path );

    if (error != 0)
    {
    	return error;
    }

	superBlock_t dSuperBlock;

	// create inodes
	int i;
	for (i = 1; i < 1 + NUM_INODES; ++i)
	{
		inode_t dInode;

		initializeInode( &dInode );

		writeInode( &dInode, i );
	}

	dSuperBlock.numberOfInodes = NUM_INODES;

	// create free blocks
	for (i = 1 + NUM_INODES; i < diskSizeInKB - 1; ++i)
	{
		char byteArray[BLOCK_SIZE];

		intToByteArray( i + 1, byteArray, BLOCK_SIZE - 4 );

		writeBlock( i, byteArray, BLOCK_SIZE );
	}

	// create final free block with -1 for next free block
	{
		char byteArray[BLOCK_SIZE];

		intToByteArray( -1, byteArray, BLOCK_SIZE - 4 );

		writeBlock( diskSizeInKB - 1, byteArray, BLOCK_SIZE );
	}

	dSuperBlock.numberOfDiskBlocks = diskSizeInKB;

	dSuperBlock.firstFreeBlock = 1 + NUM_INODES;

	writeSuperBlock( &dSuperBlock );

	return 0;
}

fd_t * createfd( int inodeBlockNum )
{
	fd_t *dFd = (fd_t *)malloc(sizeof(fd_t));

	dFd->inodeBlockNum = inodeBlockNum;

	dFd->fileptr = 0;

	oft[open++] = dFd;

	return dFd;
}

/**
 * Opens a binary file on the disk for storing data.
 */
fd_t* openf( char* name )
{
	int freeInode = -1;

	int i;
	for (i = 1; i < 1 + NUM_INODES; ++i)
	{
		inode_t dInode;

		readInode( &dInode, i );

		if (strcmp(name, dInode.filename) == 0)
		{
			if (dInode.flag == FILE_STORED)
			{
				if (isOpen( i ))
				{
					strncpy( errorDesc, "Cannot open file already opened\n", ERROR_MESSAGE_SIZE );
				}
				else
				{
					return createfd( i );
				}
			}
			else if (dInode.flag == FILE_UNUSED)
			{
				strncpy( errorDesc, "Critical failure: inode is in inconsistent state (filename match but flag marks unused)\n", ERROR_MESSAGE_SIZE );
			}
			else
			{
				strncpy( errorDesc, "Critical failure: inode is in inconsistent state (unknown flag)\n", ERROR_MESSAGE_SIZE );
			}

			return NULL;
		}
		else if (dInode.flag == FILE_UNUSED)
		{
			if (freeInode == -1)
			{
				freeInode = i;
			}
		}
	}

	if (freeInode > -1)
	{
		superBlock_t dSuperBlock;

		readSuperBlock( &dSuperBlock );

		if (dSuperBlock.firstFreeBlock == -1)
		{
			strncpy( errorDesc, "No free block available to open new file\n", ERROR_MESSAGE_SIZE );
		}
		else
		{
			char buffer[BLOCK_SIZE];

			inode_t dInode;

			initializeInode( &dInode );

			strncpy( dInode.filename, name, 11 );

			dInode.flag = FILE_STORED;

			dInode.pointer[0] = dSuperBlock.firstFreeBlock;

			// update super block with next free block

			readBlock( dSuperBlock.firstFreeBlock, buffer );

			dSuperBlock.firstFreeBlock = byteArrayToInt( buffer, BLOCK_SIZE - 4 );

			writeSuperBlock( &dSuperBlock );

			writeInode( &dInode, freeInode );

			return createfd( freeInode );
		}
	}
	else
	{
		strncpy( errorDesc, "No free inode available to open new file\n", ERROR_MESSAGE_SIZE );
	}

	return NULL;
}

int isOpen( int inodeBlockNum )
{
	int i;
	for (i = 0; i < open; ++i)
	{
		if (oft[i]->inodeBlockNum == inodeBlockNum)
		{
			return 1;
		}
	}

	return 0;
}

int closef( fd_t *stream )
{
	// null check
	if (stream != NULL)
	{
		if (isOpen( stream->inodeBlockNum ))
		{
			int i;
			for (i = 0; i < open; ++i)
			{
				if (oft[i] == stream)
				{
					break;
				}
			}

			if (i == open)
			{
				strncpy( errorDesc, "Could not find file descriptor in open file table\n", ERROR_MESSAGE_SIZE );
				return -1;
			}

			oft[i] = oft[--open];

			// free fd
			free(stream);

			return 0;
		}

		strncpy( errorDesc, "Cannot close an unopened file\n", ERROR_MESSAGE_SIZE );
		return -1;
	}

	strncpy( errorDesc, "Cannot close a null file descriptor\n", ERROR_MESSAGE_SIZE );
	return -1;
}

int readf( fd_t* stream, char* data, int size )
{
	if (stream == NULL)
	{
		strncpy( errorDesc, "Cannot read an unopened file\n", ERROR_MESSAGE_SIZE );
		return -1;
	}

	if (size <= 0)
	{
		strncpy( errorDesc, "Cannot read fewer than 1 bytes\n", ERROR_MESSAGE_SIZE );
		return -1;
	}

	if (data == NULL)
	{
		strncpy( errorDesc, "Cannot read data into null buffer\n", ERROR_MESSAGE_SIZE );
		return -1;
	}

	inode_t dInode;

	readInode( &dInode, stream->inodeBlockNum );

	if (stream->fileptr + size > dInode.filesize)
	{
		strncpy( errorDesc, "Cannot read more bytes than exist in file\n", ERROR_MESSAGE_SIZE );
		return -1;
	}

	int startpos = stream->fileptr;

	int block = stream->fileptr / BLOCK_SIZE;

	int error;

	while ( block < ((startpos + size) / BLOCK_SIZE) )
	{
		char buffer[BLOCK_SIZE];

		error = readBlock( dInode.pointer[block], buffer );

		if (error != 0)
		{
			strncpy( errorDesc, "Failed to read block\n", ERROR_MESSAGE_SIZE );
			return -1;
		}

		strncpy( &data[stream->fileptr - startpos], &buffer[stream->fileptr % BLOCK_SIZE], BLOCK_SIZE - (stream->fileptr % BLOCK_SIZE) );

		block++;

		stream->fileptr += (BLOCK_SIZE - (stream->fileptr % BLOCK_SIZE));
	}

	{
		char buffer[BLOCK_SIZE];

		error = readBlock( dInode.pointer[block], buffer );

		if (error != 0)
		{
			strncpy( errorDesc, "Failed to read block\n", ERROR_MESSAGE_SIZE );
			return -1;
		}

		strncpy( &data[stream->fileptr - startpos], buffer, size - (stream->fileptr - startpos) );

		stream->fileptr = startpos + size;
	}

	return error;
}

int writef( fd_t* stream, char* data, int size )
{
	int pos = 0;

	inode_t dInode;

	readInode( &dInode, stream->inodeBlockNum );

	if (stream->fileptr > 0 && stream->fileptr == dInode.filesize)
	{
		stream->fileptr--;
	}

	if (stream->fileptr == 0 || stream->fileptr % BLOCK_SIZE != 0)
	{
		int maxBytes = BLOCK_SIZE - (stream->fileptr % BLOCK_SIZE);

		int bytes = size < maxBytes ? size : maxBytes;

		char buffer[BLOCK_SIZE];

		readBlock( dInode.pointer[stream->fileptr / BLOCK_SIZE], buffer );

		snprintf( &buffer[stream->fileptr % BLOCK_SIZE], bytes, "%s", data );

		// Handle end-of-block case
		if (bytes < size) buffer[stream->fileptr + bytes - 1] = data[bytes - 1];

		writeBlock( dInode.pointer[stream->fileptr / BLOCK_SIZE], buffer, BLOCK_SIZE );

		pos = bytes;

		stream->fileptr += bytes;

		if (stream->fileptr > dInode.filesize)
		{
			dInode.filesize = stream->fileptr;

			writeInode( &dInode, stream->inodeBlockNum );
		}
	}

	if (pos < size)
	{
		superBlock_t dSuperBlock;

		readSuperBlock( &dSuperBlock );

		while (pos < size)
		{
			char buffer[BLOCK_SIZE];

			if (dInode.pointer[stream->fileptr / BLOCK_SIZE] == -1)
			{
				if (dSuperBlock.firstFreeBlock > -1)
				{
					readBlock( dSuperBlock.firstFreeBlock, buffer );

					int nextFreeBlock = byteArrayToInt( buffer, BLOCK_SIZE - 4 );

					dInode.pointer[stream->fileptr / BLOCK_SIZE] = dSuperBlock.firstFreeBlock;

					writeInode( &dInode, stream->inodeBlockNum );

					dSuperBlock.firstFreeBlock = nextFreeBlock;

					writeSuperBlock( &dSuperBlock );

					strncpy( &buffer[BLOCK_SIZE - 4], "\0\0\0\0", 4 );
				}
				else
				{
					strncpy( errorDesc, "Cannot write remaining bytes to file - out of blocks\n", ERROR_MESSAGE_SIZE );
					return -1;
				}
			}

			int bytes = BLOCK_SIZE > size - pos ? size - pos : BLOCK_SIZE;

			snprintf( buffer, bytes, "%s", &data[pos] );

			writeBlock( dInode.pointer[stream->fileptr / BLOCK_SIZE], buffer, BLOCK_SIZE );

			pos += bytes;

			stream->fileptr += bytes;

			if (stream->fileptr > dInode.filesize)
			{
				dInode.filesize = stream->fileptr;

				writeInode( &dInode, stream->inodeBlockNum );
			}
		}
	}

	return 0;
}

/**
    * Reads the super block from disk.
    *
    * @param superBlock - destination to store the disk's super block
    */
void readSuperBlock( superBlock_t* superBlock )
{
    int error;
	char buffer[BLOCK_SIZE];

    // read block and convert it to the superblock
	error = readBlock( 0, buffer );

	if (error != 0)
	{
		printf( "[ ER ] Failed to read super block.\n" );
		return;
	}

	// read number of disk blocks from block
	superBlock->numberOfDiskBlocks = byteArrayToInt( buffer, 0 );

	// read number of inodes from block
	superBlock->numberOfInodes =  byteArrayToInt( buffer, 4 );

	// read first free block from block
	superBlock->firstFreeBlock = byteArrayToInt( buffer, 8 );
}

/**
 * Writes the super block to disk.
 *
 * @param superBlock - the disk's super block
 */
void writeSuperBlock( superBlock_t* superBlock )
{
	// setup buffer to be written to disk
	char buffer[BLOCK_SIZE];

	// write number of disk blocks into buffer
	intToByteArray( superBlock->numberOfDiskBlocks, buffer, 0 );

	// write number of inodes into buffer
	intToByteArray( superBlock->numberOfInodes, buffer, 4 );

    // write first free block number into buffer
	intToByteArray( superBlock->firstFreeBlock, buffer, 8 );

	// write buffer to disk at block 0
	int error;

	error = writeBlock( 0, buffer, BLOCK_SIZE );

	if (error != 0)
	{
		printf( "[ ER ] Failed to write super block.\n" );
		return;
	}
}

/**
 * Reads an Inode from disk.
 *
 * @param inode - the inode to read into
 * @param blockNumber - the block number that holds the inode to be read
 *
*/
void readInode(inode_t* inode, int blockNumber)
{
	int error;
	char block[BLOCK_SIZE];

	// read block containing the Inode
    error = readBlock( blockNumber, block );

    if (error != 0)
    {
    	printf( "[ ER ] %s\n", errorDesc );
    	return;
    }

    inode->flag = byteArrayToInt( block, 0 );

    inode->owner = byteArrayToInt( block, 4 );

    inode->filesize = byteArrayToInt( block, 8 );

    strncpy( inode->filename, &block[12], 11 );

    int i;
    for (i = 0; i < MAX_POINTERS; ++i)
    {
    	inode->pointer[i] = byteArrayToInt( block, 24 + (4 * i) );
    }
}

/**
 * Writes an Inode to disk.
 *
 * @param inode - the inode to be written to disk
 * @param blockNumber - the block number in which the inode will be written
 */
void writeInode(inode_t* inode, int blockNumber)
{
	char buffer[BLOCK_SIZE];

	intToByteArray( inode->flag, buffer, 0 );

	intToByteArray( inode->owner, buffer, 4 );

	intToByteArray( inode->filesize, buffer, 8 );

	strncpy( &buffer[12], inode->filename, 11 );

	buffer[23] = '\0';

	int i;
	for (i = 0; i < MAX_POINTERS; ++i)
	{
		intToByteArray( inode->pointer[i], buffer, 24 + (4 * i));
	}

	int error;

	error = writeBlock( blockNumber, buffer, BLOCK_SIZE );

	if (error != 0)
	{
		printf( "[ ER ] Failed to write inode to block %d.\n", blockNumber );
	}
}

/**
 * Prints the content of the given inode.
 *
 * Format: (<flag>) <owner> <filename> [<filesize>K] {<blocks>}
 */
void printInode( inode_t *dInode )
{
	printf( "[ IF ] (%d) %d %s [%.2fK] {", dInode->flag, dInode->owner, dInode->filename, dInode->filesize / 1024.0 );

	int x;
	for (x = 0; x < MAX_POINTERS && dInode->pointer[x + 1] != -1; ++x)
	{
		printf("%d, ", dInode->pointer[x]);
	}

	printf("%d}\n", dInode->pointer[x]);
}

/**
 * Prints the content of a single inode at the given block.
 */
void printInodeAtBlock( int block )
{
	if (block < 1 || block > 1 + NUM_INODES)
	{
		printf( "[ ER ] Cannot print block that is not inode at block %d\n", block );
		return;
	}

	inode_t dInode;

	readInode( &dInode, block );

	printInode( &dInode );
}

/**
 * Prints the content of the inodes of the virtual disk to the screen.
 */
void printInodes()
{
	int used = 0;

	int i;
	for (i = 1; i < 1 + NUM_INODES; ++i)
	{
		inode_t dInode;

		readInode( &dInode, i );

		if (dInode.flag != FILE_UNUSED)
		{
			used++;

			printInode( &dInode );
		}
	}

	printf( "[ IF ] Total of %d used inodes\n", used );
}

/**
 * Prints the block numbers of disk blocks that are free.
 */
void printFreeBlocks()
{
	superBlock_t dSuperBlock;

	readSuperBlock( &dSuperBlock );

	int block = dSuperBlock.firstFreeBlock;

	int freeBlocks = 0;

	int error;

	printf( "[ IF ] Free Blocks: " );

	while (block != -1)
	{
		printf("%d ", block);

		char buffer[BLOCK_SIZE];

		error = readBlock( block, buffer );

		if (error != 0)
		{
			printf( "\n[ ER ] %s\n", errorDesc );
			return;
		}

		freeBlocks++;

		block = byteArrayToInt( buffer, BLOCK_SIZE - 4 );
	}

	printf( "\n" );
}

int numUsedBlocksByInode( inode_t *dInode )
{
	int i, used = 0;
	for (i = 0; dInode->pointer[i] != -1; ++i)
	{
		used++;
	}

	return used;
}

int numUsedBlocksByInodeAtBlock( int block )
{
	inode_t dInode;

	readInode( &dInode, block );

	if (dInode.flag == FILE_UNUSED)
	{
		return 0;
	}

	return numUsedBlocksByInode( &dInode );
}

int numUsedInodes()
{
	int i, used = 0;
	for (i = 1; i < 1 + NUM_INODES; ++i)
	{
		inode_t dInode;

		readInode( &dInode, i );

		if (dInode.flag != FILE_UNUSED)
		{
			used++;
		}
	}

	return used;
}

int numFreeInodes()
{
	int i, unused =0;
	for (i = 1; i < 1 + NUM_INODES; ++i)
	{
		inode_t dInode;

		readInode( &dInode, i );

		if (dInode.flag == FILE_UNUSED)
		{
			unused++;
		}
	}

	return unused;
}

int numUsedBlocks()
{
	int i, used = 0;
	for (i = 1; i < 1 + NUM_INODES; ++i)
	{
		used += numUsedBlocksByInodeAtBlock( i );
	}

	return 1 + NUM_INODES + used;
}

int numFreeBlocks()
{
	superBlock_t dSuperBlock;

	readSuperBlock( &dSuperBlock );

	int block = dSuperBlock.firstFreeBlock;

	int i, unused = 0;

	while (block > -1)
	{
		unused++;

		char buffer[BLOCK_SIZE];

		readBlock( block, buffer );

		block = byteArrayToInt( buffer, BLOCK_SIZE - 4 );
	}

	return unused;
}

int checkIntegrity( int diskSizeInKB )
{
	if (numFreeBlocks() + numUsedBlocks() != diskSizeInKB)
	{
		strncpy( errorDesc, "Number of free blocks plus number of used blocks does not equal total disk size\n", ERROR_MESSAGE_SIZE );
		return -1;
	}

	if (numFreeInodes() + numUsedInodes() != NUM_INODES)
	{
		strncpy( errorDesc, "Number of free inodes plus number of used inodes does not equal total inodes\n", ERROR_MESSAGE_SIZE );
		return -1;
	}

	return 0;
}

/**
 * Writes an integer to a byte array.
 *
 * @param value - the integer to be written to a byte array
 * @param buffer - the byte array in which the integer will be written
 * @param pos - the position in the byte array where an integer will be written
 */
void intToByteArray( int value, char* buffer, int pos )
{
	int i;

    for (i = 0; i < 4; i++)
		{
        int shift = (4 - 1 - i) * 8;
        buffer[i+pos] = (char) (( (unsigned int) value >> shift) & 0xFF);
    }
}

 /**
  * Reads an integer from a byte array at a specific location.
  *
  * @param buffer - the byte array from which an integer will be read
  * @param pos - the position in the array from where an integer will be read
  *
  * @return an integer read from the byte array
  */
int byteArrayToInt( char* buffer, int pos )
{
	int i, value = 0;

    for (i = 0; i < 4; i++)
	{
		int shift = (4 - 1 - i) * 8;
        value += (buffer[i + pos] & 0x000000FF) << shift;
    }
    return value;
}
