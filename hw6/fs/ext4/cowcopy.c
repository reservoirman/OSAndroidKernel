#include <linux/fs.h>
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <linux/syscalls.h>
#include <linux/stat.h>
#include <linux/fdtable.h>

extern struct files_struct init_files;

asmlinkage int sys_ext4_cowcopy(const char __user *src, const char __user *dest)
{

	char *tmp_src;
	char *tmp_dest;
	int fd_src ;
	int fd_dest ;
	tmp_src = getname(src);
	printk("src is correct : name = %s\n", tmp_src);
	tmp_dest = getname(dest);
	printk("dest is correct: name = %s\n", tmp_dest);
	fd_src = PTR_ERR(tmp_src);
	printk("src file descriptor is correct. fd_src =  %x \n",fd_src);
	fd_dest = PTR_ERR(tmp_dest);
	printk("dest file descriptor is correct. fd_dest =  %x \n",fd_dest);
	
	// if src is a directory , return -EPERM
	if(S_ISDIR(init_files.fd_array[fd_src]->f_dentry->d_inode->i_mode))
	{
		printk("src is a directory\n");
		return (-EPERM);
	}
	printk("src is not a directory\n");
	if(!S_ISREG(init_files.fd_array[fd_src]->f_dentry->d_inode->i_mode))
	{
	printk("src is a regular file\n");

		return (-EPERM);
	}

	printk("src is not a regular file\n");
	// if src is not in ext4 file system , return -EOPNOTSUPP
	if(strcmp(init_files.fd_array[fd_src]->f_dentry->d_inode->i_sb->s_type->name,"ext4") != 0)
	{
		printk("src does not use ext4\n");
		return (-EOPNOTSUPP);
	}

	printk("src uses ext4\n");
	// if src and dest are not on same device then return  -EXDEV
	if(init_files.fd_array[fd_src]->f_dentry->d_inode->i_sb != init_files.fd_array[fd_dest]->f_dentry->d_inode->i_sb)
	{
		printk("src and dest not on same device\n");
		return (-EXDEV);
	}
	printk("src and dest on same device\n");
	// lazy copy - create a hard link
	return 0;
}