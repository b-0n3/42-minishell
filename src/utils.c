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

int	ft_intlen(unsigned int nb)
{
	int	i;

	i = 0;
	while (nb > 0)
	{
		nb = nb / 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int c)
{
	unsigned int	nb;
	unsigned int	i;
	char			*str;

	nb = c;
	i = 0;
	if (c <= 0)
	{
		nb = -c;
		i = 1;
	}
	i += ft_intlen(nb);
	str = (char *)malloc((i + 1) * sizeof(char));
	if (!str)
		return (0);
	str[i] = '\0';
	while (i-- > 0)
	{
		str[i] = (nb % 10) + '0';
		nb = nb / 10;
	}
	if (c < 0)
		str[0] = '-';
	return (str);
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
