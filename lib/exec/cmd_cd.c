#include "minishell.h"


int	checkdir(char *dirname)
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
		write(STDERR_FILENO, 1, 1);
		write(STDOUT_FILENO, "cd: no such file or directory: ", 32);
		write(STDOUT_FILENO, dirname, sizeof(dirname));
		write(STDOUT_FILENO, " \n", 3);
		exit(1);
	}
}

int	edit_env(t_shell *this, char *dest_path, char *old_path)
{
	if (chdir(dest_path) < 0)
	{
		write(STDOUT_FILENO, "cd: no such file or directory: ", 32);
		write(STDOUT_FILENO, dest_path, sizeof(dest_path));
		write(STDOUT_FILENO, " \n", 3);
		exit(1);
	}
    if (dest_path == NULL)
        dest_path = strdup("");
    if (old_path == NULL)
        old_path = strdup("");
	this->env.replace_by_key(&this->env, "PWD", strdup(dest_path));
	this->env.replace_by_key(&this->env, "OLDPWD", strdup(old_path));
	return (1);
}

void	cmd_cd(t_shell *this, t_node *head)
{
	t_array_iterator	*iterator;
	t_string            dest_path;
	t_string            oldpath;
	t_string            tmp;

	dest_path = (char *) head->args.get(&head->args, 0);
	oldpath = (char *) this->env.find_by_key(this->env, "PWD");
	if (dest_path == NULL)
        dest_path = this->env.find_by_key(this->env , "HOME");
    if (dest_path == NULL)
        return;
    edit_env(this, dest_path, oldpath);
}

void	cmd_pwd(t_shell *this)
{
	char *tmp;

    tmp = this->env.find_by_key(this->env, "PWD");
    if (tmp == NULL)
        write(STDOUT_FILENO, "(null)", 6);
    else {
        write(STDOUT_FILENO, tmp, strlen(tmp));
        write(STDOUT_FILENO, "\n", 1);
    }
}
