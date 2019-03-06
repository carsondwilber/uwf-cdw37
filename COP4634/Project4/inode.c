/**
 * Models an inode in a file system.
 *
 * @author Thomas Reichherzer
 */

#include "inode.h"

void initializeInode(inode_t* inode)
{
	inode->flag = FILE_UNUSED;
	inode->owner = 0;
	inode->filesize = 0;
	inode->filename[0] = '\0';

	int i;
    for (i=0; i < MAX_POINTERS; i++)
    {
        inode->pointer[i] = -1;
    }
}
