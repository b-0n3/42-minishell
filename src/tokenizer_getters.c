/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_getters.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ham <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 04:23:51 by aait-ham          #+#    #+#             */
/*   Updated: 2021/12/07 04:23:53 by aait-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_string cut_w(t_env_ext *this)
{
    if (this->cursor - this->l_cursor == 1 && (this->cmd[this->l_cursor] == '\"' ||this->cmd[this->l_cursor] == '\'' ))
        return ft_strdup(" ");
    return ft_strndup(this->cmd + this->l_cursor, this->cursor - this->l_cursor);
}
t_token *get_word(t_shell *this) {
    t_token *token;
    if (this->quot || this->dqout)
        return NULL;
    if (this->cursor <= this->l_cursor) {
        return NULL;
    }

    token = new_token(strndup(this->commmand + this->l_cursor,
                              this->cursor - this->l_cursor), word);
    while (this->commmand[this->cursor] == ' ')
        this->cursor++;
    this->l_cursor = this->cursor;
    return (token);
}

t_token *get_op(t_shell *this) {
    t_token *token;
    if (this->quot || this->dqout)
        return NULL;
    token = check_pipe(this);
    if (token == NULL) {
        token = check_great(this);
        if (token == NULL)
            token = check_less(this);
    }
    return token;
}

t_token *cut_token(t_shell *this) {
    t_token *token;
    // this->cursor++;

    if (this->quot || this->dqout)
        return NULL;
    token = get_op(this);
    if (token == NULL)
        token = get_word(this);
    return token;
}

t_string cut_word(t_env_ext *this) {
    return ft_strndup(this->cmd + this->l_cursor,
                   this->cursor - this->l_cursor);
}
