#include "minishell.h"

static	int		check_nl(void *item)
{
	if (strcmp((char*) item, "-n"))
		return (1);
	else
		return (0);
}

void	cmd_echo(t_node node, t_plug con)
{
	t_array_iterator *iterator;
	int	newline;

	newline = 0;
	iterator = node.args.iterator(&node.args);
	while (iterator->has_next(iterator) && newline == 0)
		newline += (int) iterator->do_on_next(iterator, &check_nl);
	write(con.out,&iterator,strlen((char *)iterator));
	if (newline)
		write(con.out,"\n",1);
}
