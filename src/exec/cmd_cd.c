#include "minishell.h"
extern int mood;

//int	checkdir(char *dirname)
//{
//	DIR	*dir;
//
//	dir = opendir(dirname);
//	if (dir)
//	{
//		closedir(dir);
//		return (1);
//	}
//	else
//	{
//		write(STDOUT_FILENO, "cd: no such file or directory: ", 32);
//		write(STDOUT_FILENO, dirname, sizeof(dirname));
//		write(STDOUT_FILENO, " \n", 3);
//		exit(1);
//	}
//}

int	edit_env(t_shell *this, char *dest_path)
{
    char tmp[1024];

    if (chdir(dest_path) < 0)
	{
		write(STDOUT_FILENO, "cd: no such file or directory: ", 32);
		write(STDOUT_FILENO, dest_path, strlen(dest_path));
		write(STDOUT_FILENO, "\n", 1);

		return (1);
	}
    if (dest_path == NULL)
        dest_path = strdup("");
	this->env.replace_by_key(&this->env, "PWD", strdup(getcwd(tmp, 1024)), &free);
	return (0);
}

void	cmd_cd(t_shell *this, t_node *head)
{
	t_string            dest_path;

    if (head->args.index > 0)
	dest_path = (char *)((t_token*) head->args.get(&head->args, 0))->value;
    else
       dest_path = NULL;
    if (dest_path == NULL)
        dest_path = this->env.find_by_key(this->env , "HOME");
    if (dest_path == NULL)
        return;
    this->exit_code = edit_env(this, dest_path);
    if (mood == 1)
        exit(this->exit_code);
}

void	cmd_pwd(t_shell *this)
{
	char *tmp;

    tmp =(char *) this->env.find_by_key(this->env, "PWD");
    if (tmp == NULL)
        write(STDOUT_FILENO, "(null)", 6);
    else {
        write(STDOUT_FILENO, tmp, strlen(tmp));
        write(STDOUT_FILENO, "\n", 1);
    }
    if (mood == 1)
        exit(0);
}
