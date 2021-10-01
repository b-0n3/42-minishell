#include "array_list.h"

t_array_iterator *array_list_to_iterator(t_array_list *this)
{
    t_array_iterator *iterator;

    iterator = NULL;
    if (this != NULL)
    { 
        iterator = new_iterator(this);
    }
    return (iterator);
}