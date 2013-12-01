#include <linux/fs.h>
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <linux/syscalls.h>
#include <linux/stat.h>
#include <linux/path.h>
#include <linux/fs_struct.h>
#include <linux/fdtable.h>
#include <linux/slab.h>

extern struct files_struct init_files;
extern unsigned int full_name_hash(const unsigned char *name, unsigned int len);

/*unsigned char * get_final_path(const char __user *path) 
{
	int path_len;
	unsigned char *final_path;
	unsigned char *tmp_path;
	struct dentry *dentry;
	if(path[0] == '/')
	{
		// absolute path
		final_path = (unsigned char *) path;
	}
	else
	{
		
		tmp_path = current_thread_info()->task->fs.pwd->dentry;
		path_len = strlen(tmp_path) + strlen(path);
		final_path = (unsigned char *) kmalloc((size_t) path_len,GFP_KERNEL);
		strcpy(final_path,tmp_path);
		strcat(final_path,path);
	}
	return final_path;
}
*/

asmlinkage int sys_ext4_cowcopy(const char __user *src, const char __user *dest)
{

	struct dentry *dentry; 
	struct dentry *parent;
	struct qstr *d_name; 
	
	char *tmp_src;
	char *tmp_dest;
	tmp_src = getname(src);
	printk("src is correct : name = %s\n", tmp_src);
	tmp_dest = getname(dest);
	printk("dest is correct: name = %s\n", tmp_dest);
	dentry = (struct dentry *) kmalloc(sizeof(struct dentry),GFP_KERNEL);
	printk("size: %d \n",sizeof(struct qstr));
	d_name = (struct qstr *) kmalloc(sizeof(struct qstr),GFP_KERNEL);
	printk("d_name: %p \n", d_name);
	
	
	if(src[0]=='/')
	{
		parent = current_thread_info()->task->fs->root.dentry;
	}
	else
	{
		printk("else entered \n");
		parent = current_thread_info()->task->fs->pwd.dentry;
		printk("else executed \n");
	}
	printk("parent: %s\n", parent->d_name.name);
	d_name->name = (const unsigned char *)tmp_src;
	d_name->len = (unsigned int)strlen(tmp_src);
	printk("hash before %d \n", d_name->hash);
	d_name->hash = full_name_hash(d_name->name, d_name->len);
	dentry = d_lookup(parent,d_name);

	printk("src name: %s \n",d_name->name);
	printk("hash after %u \n", d_name->hash);
	if(dentry != NULL)
	{
		printk("dentry name: %s \n", dentry->d_name.name);
	}
	else
	{
		printk("dentry: %p \n", dentry);
	
	/*
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
	*/
	// lazy copy - create a hard link
	return 0;
}