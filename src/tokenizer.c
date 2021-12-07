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

t_token *check_pipe(t_shell *this) {

    if (this->commmand[this->cursor] == _pipe) {
        this->cursor++;
        while (this->commmand[this->cursor] == ' ')
            this->cursor++;
        this->l_cursor = this->cursor;
        return new_token(strdup("|"), op);
    }
    return NULL;
}


t_token *check_less(t_shell *this) {
    t_token *token;

    token = NULL;

    if (this->commmand[this->cursor] == less) {
        this->cursor++;
        this->l_cursor = this->cursor;
        token = new_token(strdup("<"), op);
        if (this->commmand[this->cursor] == less) {
            free(token->value);
            token->value = strdup("<<");
            this->cursor++;
            this->l_cursor = this->cursor;
        }
        while (this->commmand[this->cursor] == ' ')
            this->cursor++;
        this->l_cursor = this->cursor;
    }
    return token;
}

t_token *check_great(t_shell *this) {
    t_token *token;

    token = NULL;

    if (this->commmand[this->cursor] == great) {
        this->cursor++;
        this->l_cursor = this->cursor;
        token = new_token(strdup(">"), op);
        if (this->commmand[this->cursor] == great) {
            free(token->value);
            token->value = strdup(">>");
            this->cursor++;

        }
        while (this->commmand[this->cursor] == ' ')
            this->cursor++;
        this->l_cursor = this->cursor;
    }
    return token;
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
//// cat | hell
////           ^

t_bool check_op(char ch) {
    return (ch == less || ch == great || ch == _pipe);
}

t_bool check_unclosed(t_shell *this) {
    if (!this->has_next_token(this))
        return (FALSE);
    if (this->commmand[this->cursor] == '\"' && !this->quot) {
        this->dqout = !this->dqout;
        this->cursor++;
        return (TRUE);
    }
    if (this->commmand[this->cursor] == '\'' && !this->dqout) {
        this->quot = !this->quot;
        this->cursor++;
        return (TRUE);
    }
    return (FALSE);
}

void token_free(t_token *this) {
    if (this != NULL) {
        free(this->value);
        free(this);
    }
}

t_bool is_allowed_in_env(char ch) {
    return ((ch >= 'a' && ch <= 'z') ||
            (ch >= 'A' && ch <= 'Z') ||
            (ch >= '0' && ch <= '9') ||
            (ch == '_'));
}

t_string cut_word(t_env_ext *this) {
    return strndup(this->cmd + this->l_cursor,
                   this->cursor - this->l_cursor);
}

t_bool is_digit(char ch) {
    return (ch >= '0' && ch <= '9');
}

t_string ft_strjoin(t_string s1, t_string s2) {
    t_string result;
    if (s1 == NULL)
        return s2;
    if (s2 == NULL)
        return s1;
    result = calloc(strlen(s1) + strlen(s2) + 1, 1);
    memcpy(result, s1, strlen(s1));
    memcpy(result + strlen(s1), s2, strlen(s2));
    free(s1);
    return result;

}

t_bool env_ext_has_next(t_env_ext *this) {
    return (this->cursor < this->length);
}
t_string cut_w(t_env_ext *this)
{
    if (this->cursor - this->l_cursor == 1 && (this->cmd[this->l_cursor] == '\"' ||this->cmd[this->l_cursor] == '\'' ))
        return strdup(" ");
    return strndup(this->cmd + this->l_cursor, this->cursor - this->l_cursor);
}

// asdf"sdaf$HOME'sdf'_$USER_$HOME"
t_string env_ext_next(t_env_ext *this) {
    int i;
    char *env;
    char *value;

    i = 0;
    if (this->cursor == this->length && this->l_cursor < this->cursor)
        return cut_w(this);

    if (this->l_cursor > this->cursor || this->cursor >= this->length)
        return NULL;
    // ''
    //  ^
    if (this->cmd[this->cursor] == '\"' && !this->q) {
        this->dq = !this->dq;
        if (this->l_cursor < this->cursor) {
            value = strndup(this->cmd + this->l_cursor, this->cursor - this->l_cursor);
            this->cursor++;
            this->l_cursor = this->cursor;
            return value;
        }
        this->cursor++;
        this->l_cursor = this->cursor;
    }
    if (this->cmd[this->cursor] == '\'' && !this->dq) {
        this->q = !this->q;
        this->expand = !this->expand;
        if (this->l_cursor < this->cursor) {
            value = strndup(this->cmd + this->l_cursor, this->cursor - this->l_cursor);
            this->cursor++;
            this->l_cursor = this->cursor;
            return value;
        }
            this->cursor++;
        this->l_cursor = this->cursor;
    }
    if (this->cmd[this->cursor] == '$' && (this->cmd[this->cursor +1] == '?' || this->cmd[this->cursor + 1] == '$'))
    {
        this->cursor++;
        if(this->cmd[this->cursor] == '?')
        {
            this->cursor++;
            this->l_cursor = this->cursor;
            return ft_itoa(this->exit_code % 255);
        }
        else {
            this->cursor++;
            this->l_cursor = this->cursor;
            return strdup("pid not found");
        }
        }

    if (this->cmd[this->cursor] == '$' && this->expand && !is_digit(this->cmd[this->cursor + 1])) {
        i = this->cursor + 1;
        while (this->cmd[i] != '\0' && is_allowed_in_env(this->cmd[i]))
            i++;
        env = strndup(this->cmd + this->cursor + 1, i - (this->cursor + 1));
        value = this->env->find_by_key(*this->env, env);
        if (value == NULL)
            value = strdup("");
        free(env);
        env = ft_strjoin(strndup(this->cmd + this->l_cursor, this->cursor - this->l_cursor), value);
        this->cursor = i;
        this->l_cursor = this->cursor;
        return env;
    } else if (this->cmd[this->cursor] == '$') {
        value = strndup(this->cmd + this->l_cursor, this->cursor - this->l_cursor);
        this->cursor += 2;
        this->l_cursor = this->cursor;
        return value;

    }
    this->cursor++;
    return this->next(this);
}

void new_env_ext(t_env_ext *this, t_array_list *env, char *cmd, int exit_code ) {
    this->env = env;
    this->l_cursor = 0;
    this->cursor = 0;
    this->exit_code = exit_code;
    this->q = 0;
    this->length = strlen(cmd);
    this->dq = 0;
    this->expand = 1;
    this->cmd = cmd;
    this->has_next = &env_ext_has_next;
    this->next = &env_ext_next;
}

t_token *token_expand_env(t_token *this,int exit_code, t_array_list env) {
    t_env_ext env_ext;
    t_string t;
    t_string tmp;
    new_env_ext(&env_ext, &env, this->value, exit_code);
    t = strdup("");
    while (env_ext.has_next(&env_ext)) {
        tmp = env_ext_next(&env_ext);
        if (tmp != NULL) {
            t = ft_strjoin(t, tmp);
            free(tmp);
        }

    }
    free(this->value);
    this->value = t;
    return this;
}
// cat herll | dsf  > dsf  <<ff
//   ^         ^
/*
 * todo : fix space after quotes
   * @return a token if found or null in case of error or find unclosed quot
 * */
t_token *shell_get_next_token(t_shell *this) {
    t_token *token;

    if (this->cursor <= this->l_cursor && this->cursor >= this->command_len)
        return NULL;
    if (this->cursor > this->command_len && this->unclosed(this))
        return (NULL);
    if (this->cursor > this->command_len)
        return (NULL);
    if (this->cursor == this->command_len)
        return cut_token(this);
    if (check_unclosed(this))
        return this->get_next_token(this);
    if ((this->commmand[this->cursor] == '\'' && this->quot)
        || (this->commmand[this->cursor] == '\"' && this->dqout))
        return get_word(this);
    if (check_op(this->commmand[this->cursor]) && !this->unclosed(this)) {
        if (this->l_cursor < this->cursor)
            token = get_word(this);
        else
            token = get_op(this);

        return token;
    }
    if (this->commmand[this->cursor] == ' ' && !this->unclosed(this) && this->cursor > this->l_cursor)
        return get_word(this);
    this->cursor++;
    return this->get_next_token(this);
}