#include "minishell.h"

//void start_shell(t_shell *this)
//{
//	t_string line;
//
////	line = readline(" $>");
////	while (line != NULL && !strcmp(line,"exit"))
////	{
////		this->loop(this, line);
////
////		line = readline(" $>");
////	}
//}
int		g_mood	;

void	print_map(void *item)
{
	t_key_map	*m;

	if (item != NULL)
	{
		m = (t_key_map *) item;
		printf("{\n\"key\":\"%s\",\n \"value\":\"%s\"\n}", m->key, (char *) m->value);
	}
}

void	quit_handler(int num)
{
	if (g_mood == 0)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	main(int argc, t_string *argv, t_string *env)
{
	t_shell	shell;

	argc = 1;
	argv = NULL;
	g_mood = 0;
	signal(SIGQUIT, &quit_handler);
	signal(SIGINT, &quit_handler);
	create_shell(&shell, env);
	shell.env.is_map = TRUE;
	shell.loop(&shell);
	system("leaks minishell");
}
