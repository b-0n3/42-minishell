// #include "array_list.h" 
// int  push_somthing_here(t_array_list *list, int argc , char **argv)
// {
//     int i;

//     i = 0;

//     while(i < argc)
//     {
//         list->push(list, argv[i], sizeof(char));
//         i++;
//     }
//     return 1;
// }

// void *print_item(void *item)
// {
//     printf("item : %s\n", (char*) item);
//     return (void *)"hello";
// }

// int main(int  argc , char **argv)
// {
//     t_array_list list;

//     new_array_list(&list , argc, sizeof(char ));
    
//     push_somthing_here(&list, argc , argv);

//     t_array_iterator *iterator = list.iterator(&list);
//     if (iterator == NULL)
//     {
//         list.free(&list, NULL);
//         exit(1);
//     }

//     void *first = iterator->next(iterator);
//     printf("first item : %s\n", (char *) first);
//     while (iterator->has_next(iterator))
//         iterator->do_on_next(iterator, &print_item);

//     iterator->reset(iterator);
//      while (iterator->has_next(iterator))
//         iterator->do_on_next(iterator, &print_item);
//     exit(1);
// }