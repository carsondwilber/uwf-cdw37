/**
 * Simulates a physical disk. The disk reads and writes blocks of data.
 *
 * @author Thomas Reichherzer
 */

#include "disk.h"

#define NUM_BLOCKS 1

/**
 * Constructs a disk simulated by a file.
 *
 * @param sizeInKB - the size of the disk in kilo bytes
 * @param path - the path of the file simulating the disk
 *
 * @return -1 if error occurs creating the disk or 0 if the disk was created successfully
*/
int createDisk( int sizeInKB, const char* path )
{
	if (sizeInKB <= 0)
	{
		strncpy( errorDesc, "Invalid disk size\n", ERROR_MESSAGE_SIZE );
		return -1;
	}
	_sizeInKB = sizeInKB;
	_physicalDisk = fopen( path, "w+" );
	if( _physicalDisk == NULL )
	{
		strncpy( errorDesc, "Failed to create file\n", ERROR_MESSAGE_SIZE );
		return -1;
	}
	_diskLength = sizeInKB*BLOCK_SIZE;
	return 0;
}

/**
* Reads a data block from the disk.
*
* @param blockNumber - specifies the block to be read
* @param blockData - a block of data the size of BLOCK_SIZE
*
* @return -1 if an error occurrs reading a block of data, else returns number of bytes read
*/
int readBlock( int blockNumber, char* blockData )
{
	int error, count;
	if ((blockNumber >= _sizeInKB) || (blockNumber < 0))
	{
		strncpy( errorDesc, "Invalid block number\n", ERROR_MESSAGE_SIZE );
		return -1;
	}
	//offset specified number of bytes from the beginning of the disk
	error = fseek(_physicalDisk, blockNumber*BLOCK_SIZE, 0);
	if( error )
	{
		strncpy( errorDesc, "Couldn't seek to specified block\n", ERROR_MESSAGE_SIZE );
		return -1;
	}
	//read NUM_BLOCKS unit of 1024 bytes from file _physicalDisk to array blockData
	count = fread( blockData, BLOCK_SIZE, 1, _physicalDisk );
	if( count != NUM_BLOCKS  )
	{
		strncpy( errorDesc, "Couldn't read block\n", ERROR_MESSAGE_SIZE );
		return -1;
	}

	return error;
}

/**
 * Writes a data block to the disk.
 *
 * @param blockNumber - specifies the location where the block of data be written
 * @param blockData - the block of data to be written
 * @param numberOfBytes - the number of bytes to write into the block
 *
 * @return -1 if an error occurrs writing a block of data, else number of elements written
*/
int writeBlock( int blockNumber, const char* blockData, int numberOfBytes  )
{
	int error, count;
	if ((blockNumber >= _sizeInKB) || (blockNumber < 0))
	{
		strncpy( errorDesc, "Invalid block number\n", ERROR_MESSAGE_SIZE );
		return -1;
	}
	//offset specified number of bytes from the beginning of the disk
	error = fseek(_physicalDisk, blockNumber*BLOCK_SIZE, 0);
	if( error )
	{
		strncpy( errorDesc, "Couldn't seek to specified block\n", ERROR_MESSAGE_SIZE );
		return -1;
	}
	//if number of bytes to write is bigger than a block, write maximum of a block,
	//else write the number of bytes
	int len = (numberOfBytes > BLOCK_SIZE ? BLOCK_SIZE : numberOfBytes);
	//read NUM_BLOCKS unit of 1024 bytes from file _physicalDisk to array blockData
	count = fwrite( blockData, len, NUM_BLOCKS, _physicalDisk );
	if( count != NUM_BLOCKS )
	{
		strncpy( errorDesc, "Couldn't write block\n", ERROR_MESSAGE_SIZE );
		return -1;
	}

	return error;
}

/**
 * Returns the size of the disk.
 *
 * @return the size of the disk in kilo bytes
*/
int getSize()
{
	return _sizeInKB;
}
