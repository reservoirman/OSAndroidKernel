#include <linux/fs.h>
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <linux/syscalls.h>
#include <linux/stat.h>
#include <linux/path.h>
#include <linux/fs_struct.h>
#include <linux/fdtable.h>
#include <linux/mount.h>
#include <linux/slab.h>
#include <linux/xattr.h>
#include <linux/namei.h>

extern struct files_struct init_files;
extern unsigned int full_name_hash(const unsigned char *name, unsigned int len);

#define EXT4_XATTR_INDEX_USER			1
extern int ext4_xattr_set(struct inode *inode, int name_index, const char *name, const void *value, size_t value_len, int flags);
extern int ext4_xattr_get(struct inode *inode, int name_index, const char *name, void *buffer, size_t buffer_size);

asmlinkage int sys_ext4_cowcopy(const char __user *src, const char __user *dest)
{
	struct path src_path,dest_path;
	struct dentry *dest_dentry;
	struct inode *src_inode;
	int error;
	int ret;
	int result;
	int testvalue = 1;

	error = user_path(src, &src_path);

	// error value indicates whether there is any error while opening the file. 0 --> no error. 
	if (error == 0)
	{
		src_inode = src_path.dentry->d_inode;

		// if src is a directory , return -EPERM
		if(S_ISDIR(src_inode->i_mode))
		{
			return (-EPERM);
		}
		
		// if src is not a regular file return -EPERM
		if( ! S_ISREG(src_inode->i_mode) )
		{
			return (-EPERM);
		}
		
		// if src is not in ext4 file system , return -EOPNOTSUPP
		if(strcmp(src_inode->i_sb->s_type->name,"ext4") != 0)
		{
			return (-EOPNOTSUPP);
		}


		// if the file is open for writing, then return an EPERM
		if(src_inode->i_writecount.counter != 0)
 		{
 			return (-EPERM);
 		}

		// if the  dest exists, then return EEXISTS
		error = user_path(dest, &dest_path);
		if (error == 0)
		{	
			return (-EEXIST);
		}
		
		// if src and dest are not on same device then return  -EXDEV
		dest_dentry = user_path_create(-1, dest, &dest_path, 0);
		if(src_path.mnt != dest_path.mnt)
		{
			return (-EXDEV);
		}
	
		// lazy copy - create a hard link
		result = vfs_link(src_path.dentry,dest_path.dentry->d_inode,dest_dentry);
		printk("hard link creation status : %d \n", result);

		mutex_unlock(&dest_path.dentry->d_inode->i_mutex);
		
		if(result == 0 )
		{
			ret = ext4_xattr_set(src_inode, EXT4_XATTR_INDEX_USER, "COW", &testvalue, 4, XATTR_CREATE);
			if( ret != 0)
			{
				ret = ext4_xattr_set(src_inode, EXT4_XATTR_INDEX_USER, "COW", &testvalue, 4, XATTR_REPLACE);
			}
			printk("ext4_xattr_set return value: %d\n", ret);

		}
		else
		{
			return result;
		}
	}
	else
	{
		return -1;
	}
	return 0;
}