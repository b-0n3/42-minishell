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

char	*ft_strndup(const char *s1, size_t n)
{
	int			i;
	size_t		len;
	char		*cmd;

	i = 0;
	if (s1 == NULL)
		return (NULL);
	len = ft_strlen(s1);
	if (len < n)
		return (ft_strdup(s1));
	cmd = calloc(n + 1, 1);
	ft_memmove(cmd, s1, n);
	return (cmd);
}
