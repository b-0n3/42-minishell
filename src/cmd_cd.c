#include "minishell.h"

void	cmd_cd(t_shell this,t_node args, int in, int out)
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
        dest_path = strcat(tmp,dest_path);
        chdir(dest_path);
		this.env.rmplc_by_key(this.env,"PWD", dest_path, strlen(dest_path));
		this.env.rmplc_by_key(this.env,"OLDPWD", oldpath, strlen(oldpath));
	}
	else
	{
		chdir(dest_path);
		this.env.rmplc_by_key(this.env,"PWD", dest_path, strlen(dest_path));
		this.env.rmplc_by_key(this.env,"OLDPWD", oldpath, strlen(oldpath));
	}
}

void	cmd_pwd(t_shell this,int in, int out)
{
	char *tmp;

	tmp = (char *) this.env.find_by_key(this.env, "PWD");
	write(out,&tmp,strlen(tmp));
	write(out,"\n",1);
}

static	int		check_nl(void *item)
{
	if (strcmp((char*) item, "-n"))
		return (1);
	else
		return (0);
}

void	cmd_echo(t_node node, int in, int out)
{
	t_array_iterator *iterator;
	int	newline;

	newline = 0;
	iterator = node.args.iterator(&node.args);
	while (iterator->has_next(iterator) && newline == 0)
		newline += (int) iterator->do_on_next(iterator, &check_nl);
	write(out,&iterator,strlen((char *)iterator));
	if (newline)
		write(out,"\n",1);
}
