
#include "minishell.h"

void	cmd_env(t_shell this, t_plug con)
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
			write(con.out,sw->key,sizeof(sw->key));
			write(con.out,"=",1);
			write(con.out,sw->value,sizeof(sw->value));
			write(con.out,"\n",1);
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
			}
			i++;
			item =(t_key_map *) this.get(&this, i);
		}

	}
}

void	cmd_export(t_shell this, t_node node)
{
	t_array_iterator *iterator;
	t_key_map *map;

	iterator = (char *) node.args.iterator(&node.args);
    map = malloc(sizeof(t_key_map));
    if (map == NULL)
        return;
    map->key = split_env(iterator);
    map->value = strdup( iterator + strlen(map->key) + 1);
	if((char *) this.env.find_by_key(this.env, map->key) == NULL)
		this.env.push(&this, map, sizeof(t_key_map *));
	else
		this.env.rmplc_by_key(this.env, map->key, map->value , strlen(map->value));
}
