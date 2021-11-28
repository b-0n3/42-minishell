#include "minishell.h"

//void start_shell(t_shell *this)
//{
//    t_string line;
//
////    line = readline(" $>");
////    while (line != NULL && !strcmp(line,"exit"))
////    {
////        this->loop(this, line);
////
////        line = readline(" $>");
////    }
//}
void print_map(void *item) {
    t_key_map *m;

    if (item != NULL) {
        m = (t_key_map *) item;
		printf("{\n\"key\":\"%s\",\n \"value\":\"%s\"\n}", m->key, (char *) m->value);
	}
}
// catt hello | cd ..  | pwd > file
//     ^

int main(int argc, t_string *argv, t_string *env) {
    t_shell shell;
    create_shell(&shell, env);
    shell.env.is_map = TRUE;
//  shell.env.foreach(&shell.env, &print_map);
    //  printf("%s\n", (char *)shell.env.find_by_key(shell.env, "HOMEBREW_TEMP"));
    shell.loop(&shell);
    // parser.free(parser);
    system("leaks minishell");

}

