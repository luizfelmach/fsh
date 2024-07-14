#ifndef TOKENS_H
#define TOKENS_H

typedef char **Tokens;

/*
 * Return an split array of strings that are between the delimiters.
 * Need to free up memory after use
 * The last string is a null pointer
 */
Tokens tokens_create(char *s, char *delimiter);

/*
 *
 *
 */
int tokens_size(Tokens t);

/*
 *
 *
 */
void tokens_destroy(Tokens t);

#endif  // !TOKENS_H
