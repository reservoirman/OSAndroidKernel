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
	int readtestvalue = 0;

	error = user_path(src, &src_path);
	// error value indicates whether there is any error while opening the file. 0 --> no error. 
	if (error == 0)
	{
		src_inode = src_path.dentry->d_inode;
		printk("The dentry name is %s\n", src_path.dentry->d_name.name);

		// if src is a directory , return -EPERM
		if(S_ISDIR(src_inode->i_mode))
		{
			printk("src is a directory\n");
			return (-EPERM);
		}
		printk("src is not a directory\n");
		
		// if src is not a regular file return -EPERM
		if( ! S_ISREG(src_inode->i_mode) )
		{
			printk("src is not a regular file\n");
			return (-EPERM);
		}
		printk("src is a regular file\n");
		
		// if src is not in ext4 file system , return -EOPNOTSUPP
		if(strcmp(src_inode->i_sb->s_type->name,"ext4") != 0)
		{
			printk("src does not use ext4. It uses %s \n",src_inode->i_sb->s_type->name );
			return (-EOPNOTSUPP);
		}
		printk("src uses ext4\n");


		// if the file is open for writing, then return an EPERM
		printk("file not open for writing, hence can be copied\n");

		// if the  dest exists, then return EEXISTS
		error = user_path(dest, &dest_path);
		if (error == 0)
		{	
			printk("destination file exists\n");
			return (-EEXIST);
		}
		printk("destination file does not exist, so a COW file can be created\n");

		// if src and dest are not on same device then return  -EXDEV
		
		dest_dentry = user_path_create(-1, dest, &dest_path, 0);
		printk("dest_path: %s \n ", dest_path.dentry->d_name.name);
		printk("dest_dentry : %s \n", dest_dentry->d_name.name);
		if(src_path.mnt != dest_path.mnt)
		{
			printk("src device and dest not on same device\n");
			return (-EXDEV);
		}
		printk("src and dest on same device\n");
		

		// lazy copy - create a hard link
		result = vfs_link(src_path.dentry,dest_path.dentry->d_inode,dest_dentry);
		printk("hard link creation status : %d \n", result);

		dput(dest_dentry);
		mutex_unlock(&dest_path.dentry->d_inode->i_mutex);
		path_put(&dest_path);
		path_put(&src_path);
		
		if(result == 0 )
		{
			ret = ext4_xattr_set(src_inode, EXT4_XATTR_INDEX_USER, "COW", &testvalue, 4, XATTR_CREATE);
			if( ret != 0)
			{
				ret = ext4_xattr_set(src_inode, EXT4_XATTR_INDEX_USER, "COW", &testvalue, 4, XATTR_REPLACE);
			}
			printk("ext4_xattr_set return value: %d\n", ret);
			ext4_xattr_get(src_inode, EXT4_XATTR_INDEX_USER, "COW", &readtestvalue, 4);
			printk("This should be 1: %u\n", readtestvalue);
		}
		else
		{
			return result;
		}
	}
	else
	{
		printk("source file doesnt exist\n" );
		return -1;
	}
	return 0;
}