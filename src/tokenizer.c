#include "minishell.h"

t_token  *new_token(t_string value, t_token_type type)
{
    t_token  *token;

    token = malloc(sizeof(t_token));
    if(token == NULL)
        return  NULL;
    token->value = value;
    token->type =type;
    return (token);
}

t_token *check_pipe(t_shell *this)
{

    if (this->commmand[this->cursor] == pipe) {
        this->cursor++;
        while(this->commmand[this->cursor] == ' ')
            this->cursor++;
        this->l_cursor = this->cursor;
        return new_token(strdup("|"), op);
    }
        return  NULL;
}


t_token  *check_less(t_shell *this)
{
    t_token *token;

    token = NULL;

    if (this->commmand[this->cursor] == less)
    {
        this->cursor++;
        this->l_cursor= this->cursor;
        token = new_token(strdup("<"), op);
        if (this->commmand[this->cursor] == less) {
            free(token->value);
            token->value = strdup("<<");
            this->cursor++;
            this->l_cursor = this->cursor;
        }
        while(this->commmand[this->cursor] == ' ')
            this->cursor++;
        this->l_cursor = this->cursor;
    }
    return token;
}
t_token  *check_great(t_shell *this)
{
    t_token *token;

    token = NULL;

    if (this->commmand[this->cursor] ==  great)
    {
        this->cursor++;
        this->l_cursor= this->cursor;
        token = new_token(strdup(">"), op);
        if (this->commmand[this->cursor] == great) {
            free(token->value);
            token->value = strdup(">>");
            this->cursor++;

        }
        while(this->commmand[this->cursor] == ' ')
            this->cursor++;
        this->l_cursor = this->cursor;
    }
    return token;
}

t_token *get_word(t_shell *this)
{
    t_token  *token;
    if (this->quot || this->dqout)
        return NULL;
    if(this->cursor <= this->l_cursor) {
        return NULL;
    }

    token = new_token(strndup(this->commmand + this->l_cursor,
                             this->cursor - this->l_cursor ), word);
    while(this->commmand[this->cursor] == ' ')
        this->cursor++;
    this->l_cursor = this->cursor;
    return  (token);
}
t_token *get_op(t_shell *this)
{
    t_token *token;
    if (this->quot || this->dqout)
        return NULL;
    token = check_pipe(this);
    if (token == NULL)
    {
        token = check_great(this);
        if (token == NULL)
            token = check_less(this);
    }
    return token;
}

t_token  *cut_token(t_shell *this) {
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
////    ^

t_bool  check_op(char ch)
{
    return (ch == less || ch == great || ch == pipe);
}

t_bool check_unclosed(t_shell *this)
{
    if (!this->has_next_token(this))
        return (FALSE);
    if (this->commmand[this->cursor] == '\"' && !this->quot)
    {
        this->dqout= !this->dqout;
        this->cursor++;
        return (TRUE);
    }
    if (this->commmand[this->cursor] == '\'' && !this->dqout)
    {
        this->quot= !this->quot;
        this->cursor++;
        return (TRUE);
    }
    return  (FALSE);
}
// sadf'  ' asdf'
//             ^
/*
 * todo : fix space after quotes
    @return a token if found or null in case of error or find unclosed quot
 * */
t_token *shell_get_next_token(t_shell *this)
{
    t_token  *token;

    if (this->cursor <= this->l_cursor && this->cursor >= this->command_len)
        return  NULL;
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
    if (check_op(this->commmand[this->cursor]) && !this->unclosed(this) )
    {
        if (this->l_cursor < this->cursor)
            token = get_word(this);
        else
            token = get_op(this);

        return token;
    }
    if (this->commmand[this->cursor] == ' ' && !this->unclosed(this) )
        return get_word(this);
    this->cursor++;
    return  this->get_next_token(this);
}