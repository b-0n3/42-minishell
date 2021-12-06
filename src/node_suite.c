/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_suite.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 06:24:08 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/06 06:24:36 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	node_need_a_file(t_node *this)
{
	if (this == NULL)
		return (FALSE);
	return (this->op_type != pipeline && this->op_type != o_t_none);
}

void	node_free(t_node *this)
{
	if (this == NULL)
		return ;
	node_free(this->left);
	node_free(this->right);
	free(this);
}

void	node_to_string(t_node *this)
{
	return ;
}
