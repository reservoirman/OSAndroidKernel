#include <linux/fs.h>
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <linux/syscalls.h>
#include <linux/stat.h>
#include <linux/path.h>
#include <linux/fs_struct.h>
#include <linux/fdtable.h>
#include <linux/slab.h>
#include <linux/namei.h>

extern struct files_struct init_files;
extern unsigned int full_name_hash(const unsigned char *name, unsigned int len);

asmlinkage int sys_ext4_cowcopy(const char __user *src, const char __user *dest)
{
	struct path src_path,dest_path;
	struct inode *src_inode;
	int error;
	error = user_path(src, &src_path);
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
		
		if(!S_ISREG(src_inode->i_mode))
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


		// if src and dest are not on same device then return  -EXDEV
		/*
		if(src_inode->i_sb != src_inode->i_sb)
		{
			printk("src and dest not on same device\n");
			return (-EXDEV);
		}
		printk("src and dest on same device\n");
		*/
		


		// if the file is open for writing, then return an EPERM
		if(src_inode->i_writecount.counter != 0)
		{

			printk("file already open for writing\n");
			return (-EPERM);
		}
		printk("file not open for writing, hence can be copied\n");

		// if the  dest exists, then return EEXISTS
		error = user_path(dest, &dest_path);
		if (error == 0)
		{	
			
			printk("destination file exists\n");
			return (-EEXIST);
		
		}
		else
		{
			printk("error parsing\n");
		}
		// lazy copy - create a hard link

	}

	


	return 0;
}