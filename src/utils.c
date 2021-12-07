/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 03:56:03 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/07 04:07:28 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	taille(unsigned int nb)
{
	unsigned int	size;

	size = 0;
	while (nb >= 10)
	{
		nb /= 10;
		++size;
	}
	return ((int)(size + 1));
}

char	*taballoc(int n, int size, int i)
{
	char	*tab;
	int		nb;

	if (n < 0)
		nb = (size + 2);
	else
		nb = (size + 1);
	tab = malloc(nb);
	if (tab == 0)
		return (NULL);
	tab[i] = '-';
	return (tab);
}

char	*ft_itoa(int n)
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
	if (taballoc(n, size, i))
		return (0);
	if (n < 0)
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

void	my_free(void *item)
{
	if (item != NULL)
		free(item);
}

void	split_it(t_array_list *this, t_string cmd, char ch)
{
	int		dq;
	int		q;
	int		cursor;
	int		l_cursor;

	cursor = 0;
	l_cursor = 0;
	q = 0;
	dq = 0;
	while (cmd[cursor] != '\0')
	{
		if (cmd[cursor] == ch)
		{
			this->push(this, strndup(cmd + l_cursor,
					cursor - l_cursor), sizeof(char *));
			while (cmd[cursor] == ch)
				cursor++;
			l_cursor = cursor;
		}
		cursor++;
	}
	if (l_cursor < cursor)
		this->push(this, strndup(cmd + l_cursor,
				cursor - l_cursor), sizeof(char *));
}
