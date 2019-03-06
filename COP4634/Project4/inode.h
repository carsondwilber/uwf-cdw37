#ifndef INODE_H_
#define INODE_H_

/**
 * Models an inode in a file system.
 *
 * @author Thomas Reichherzer
 */

#define MAX_POINTERS 250

// flag values
#define FILE_UNUSED 0
#define FILE_STORED 1

typedef struct inode
{
	// 0 indicates that the Inode is unused
	// 1 indicates that the Inode is used
    int flag;
    // indicates the owner of the file (an Id)
    int owner;
    // indicates the size of the file
    int filesize;
	// specifies the filename (up to 8 characters plus . plus 3 letter extension is allowed)
	char filename[12];
    // a list of direct pointers to data blocks of the file
    int pointer[MAX_POINTERS];
} inode_t;

//Prototypes
void initializeInode(inode_t* inode);

#endif /*INODE_H_*/
