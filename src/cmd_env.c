
#include "minishell.h"

void	cmd_env(t_shell this,int in, int out)
{
	size_t	i;
	t_array_list *m;
	t_key_map *sw;

	m = (t_array_list *)&this.env;
	i = 0;
	while (i < m->index)
	{
		if (m->arr[i] != NULL)
		{
			sw = (t_key_map *) m->arr[i];
			write(out,sw->key,sizeof(sw->key));
			write(out,"=",1);
			write(out,sw->value,sizeof(sw->value));
			write(out,"\n",1);
		}
		i++;
	}
}

void	cmd_unset(t_shell shell,t_string key)
{
	t_array_list  this;
	t_key_map *item;
	int i;

	this = (t_array_list ) shell.env;
	i = 0;
	if (this.is_map)
	{
		item =(t_key_map *) this.get(&this, i);
		while (item != NULL)
		{
			if (!strcmp(key, (const char *)item->key) )
			{
				this.pop_i(&this,i);
				return (1);
			}
			i++;
			item =(t_key_map *) this.get(&this, i);
		}

	}
	return (NULL);	
}