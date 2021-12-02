#include "minishell.h"
#include <dirent.h>

int	checkdir(char *dirname, t_plug con)
{
	DIR	*dir;

	dir = opendir(dirname);
	if (dir)
	{
		closedir(dir);
		return (1);
	}
	else
	{
		write(stderr, 1, 1);
		write(con.out, "cd: no such file or directory: ", 32);
		write(con.out, dirname, sizeof(dirname));
		write(con.out, " \n", 3);
		exit(1);
	}
}

int	editenv(t_shell this, char *dest_path, char *oldpath, t_plug con)
{
	if (chdir(dest_path) < 0)
	{
		write(stderr, 1, 1);
		write(con.out, "cd: no such file or directory: ", 32);
		write(con.out, dest_path, sizeof(dest_path));
		write(con.out, " \n", 3);
		exit(1);
	}
	this.env.rmplc_by_key(this.env, "PWD", dest_path, strlen(dest_path));
	this.env.rmplc_by_key(this.env, "OLDPWD", oldpath, strlen(oldpath));
	return (1);
}

void	cmd_cd(t_shell this, t_node args, t_plug con)
{
	t_array_iterator *iterator;
	char *dest_path;
	char *oldpath;
	char tmp[MAXPATH];

	dest_path = (char *) iterator->next(iterator);//get the cd argument value

	oldpath = (char *) this.env.find_by_key(this.env, "PWD");
	if(dest_path[0] != '/')
	{
		getcwd(tmp,sizeof(tmp));
		strcat(tmp,"/");
		dest_path = strcat(tmp, dest_path);
		if (checkdir(dest_path, con))
			editenv(this, dest_path, oldpath, con);
	}
	else
		editenv(this, dest_path, oldpath, con);
}

void	cmd_pwd(t_shell this, t_plug con)
{
	char tmp[MAXPATH];

	getcwd(tmp,sizeof(tmp));
	write(con.out,&tmp,strlen(tmp));
	write(con.out,"\n",1);
}
