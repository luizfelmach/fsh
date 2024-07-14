#ifndef TOKENS_H
#define TOKENS_H

/*
 * Alias for char **.
 */
typedef char **Tokens;

/*
 * Return an split array of strings that are between the delimiters.
 * Need to clean up memory after use.
 * The last string is a null pointer.
 */
Tokens tokens_create(char *s, char *delimiter);

/*
 * Return the length of tokens.
 */
int tokens_size(Tokens t);

/*
 * Free a block alloated for tokens.
 */
void tokens_destroy(Tokens t);

#endif  // !TOKENS_H
