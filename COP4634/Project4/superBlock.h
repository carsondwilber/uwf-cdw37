#ifndef SUPERBLOCK_H_
#define SUPERBLOCK_H_

typedef struct superBlock
{
	// the number of blocks on disk
	int numberOfDiskBlocks;
	// the number of inodes on disk
	int numberOfInodes;
	// the first free block on disk, or -1 if none free
	int firstFreeBlock;
} superBlock_t;

#endif /* SUPERBLOCK_H_ */
