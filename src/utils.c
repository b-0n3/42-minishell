/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ham <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 02:57:48 by aait-ham          #+#    #+#             */
/*   Updated: 2021/12/07 02:57:59 by aait-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
static	int		taille(unsigned int nb)
{
    unsigned int	size;

    size = 0;
    while (nb >= 10)
    {
        nb /= 10;
        ++size;
    }
    return (int)(size + 1);
}

char			*ft_itoa(int n)
{
    char			*tab;
    unsigned int	nb;
    unsigned int	i;
    unsigned int	size;

    if (n < 0)
        nb = n * -1;
    else
        nb = n;
    size = taille(nb);
    i = 0;
    tab = malloc(size + 1 + (n < 0 ? 1 : 0));
    if (tab == 0)
        return (0);
    if (n < 0 && (tab[i] = '-'))
        size++;
    i = size - 1;
    while (nb >= 10)
    {
        tab[i--] = (nb % 10 + 48);
        nb /= 10;
    }
    tab[i] = (nb % 10 + 48);
    tab[size] = '\0';
    return (tab);
}

void my_free(void *item)
{
    if (item != NULL)
        free(item);
}

void split_it(t_array_list *this, t_string cmd, char ch)
{
    int dq;
    int q;
    int cursor;
    int l_cursor;

    cursor = 0;
    l_cursor = 0;
    q = 0;
    dq = 0;
    while(cmd[cursor] != '\0')
    {
        if (cmd[cursor] == ch)
        {
            this->push(this, strndup(cmd + l_cursor, cursor - l_cursor ), sizeof(char *));
            while (cmd[cursor] == ch)
                cursor++;
            l_cursor = cursor;
        }
        cursor++;
    }
    if (l_cursor < cursor)
        this->push(this, strndup(cmd + l_cursor, cursor - l_cursor ), sizeof(char *));
}

t_array_iterator *split(t_string cmd, char ch)
{
    t_array_list  *list;

    if (cmd == NULL)
        return (NULL);
    list = malloc(sizeof(t_array_list));
    new_array_list(list, 2, sizeof(char *));
    if (list == NULL)
      return (NULL);
    split_it(list, cmd , ch);
    return list->iterator(list);
}
