/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ham <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 05:25:15 by aait-ham          #+#    #+#             */
/*   Updated: 2021/12/07 05:25:17 by aait-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*strndup(const char *s1, size_t n)
{
	int			i;
	size_t		len;
	t_string	cmd;

	i = 0;
	if (s1 == NULL)
		return (NULL);
	len = ft_strlen(s1);
	if (len < n)
		return (ft_strdup(s1));
	cmd = malloc(n + 1);
	ft_strlcpy(cmd, s1, n);
	return (cmd);
}
