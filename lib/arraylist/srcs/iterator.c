#include "array_list.h"

t_array_iterator *new_iterator(t_array_list *list)
{
    t_array_iterator *it;

    if (list == NULL)
        return (NULL);
    it = (t_array_iterator *) malloc(sizeof(t_array_iterator));
    if (it == NULL)
        return (it);
    it->list = list;
    it->has_next = &iterator_has_next;
    it->do_on_next = &iterator_do_on_next;
    it->next = &iterator_next;
    it->do_on_next_p = &iterator_do_on_next_p;
    it->next_index = 0;
    it->reset = &iterator_reset;
    it->free = &iterator_free;

    return (it);
}

void iterator_reset(t_array_iterator *this)
{
    if (this != NULL)
        this->next_index = 0;
}

void *iterator_next(t_array_iterator *this)
{
    void *ptr;

    ptr = NULL;
    if (this != NULL)
    {
        ptr = this->list->get(this->list, this->next_index);
        this->next_index++;
    }
    return ptr;
}

void *iterator_do_on_next(t_array_iterator *this, void *(*f)(void *))
{
    void *ptr;

    ptr = NULL;
    if (this != NULL)
    {
        ptr = this->next(this);
        if (ptr == NULL)
            return NULL;
        ptr = f(ptr);
        this->list->update_at(this->list, ptr, this->next_index - 1);
    }
    return ptr;
}

void *iterator_do_on_next_p(t_array_iterator *this, void *(*f)(void *, void *), void *p_item)
{
    void *ptr;

    ptr = NULL;
    if (this != NULL)
    {
        ptr = this->next(this);
        if (ptr == NULL)
            return NULL;
        ptr = f(ptr, p_item);
        this->list->update_at(this->list, ptr, this->next_index - 1);
    }
    return ptr;
}

t_bool	iterator_has_next(t_array_iterator *this)
{
    if (this == NULL)
        return (FALSE);

    return this->next_index < this->list->index;
}

void iterator_free(t_array_iterator *this, void (*f)(void *))
{
    if (this != NULL)
    {
        if (f != NULL)
            this->list->free(this->list, f);
        free(this);
    }
}