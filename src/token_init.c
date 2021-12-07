/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ham <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 04:18:41 by aait-ham          #+#    #+#             */
/*   Updated: 2021/12/07 04:18:43 by aait-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *new_token(t_string value, t_token_type type) {
    t_token *token;
    int i;

    i = 0;
    token = malloc(sizeof(t_token));
    if (token == NULL)
        return NULL;
    while (value[i] == ' ')
        i++;
    token->value = value;
    token->type = type;
    token->start_with = value[i];
    token->to_node = &token_to_node;
    token->free = &token_free;
    token->expand = &token_expand_env;
    token->to_file = &token_to_file;
    return (token);
}

void token_free(t_token *this) {
    if (this != NULL) {
        free(this->value);
        free(this);
    }
}