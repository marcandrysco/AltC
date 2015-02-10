#ifndef ARGV_H
#define ARGV_H

/*
 * argument vector function declarations
 */

void argv_parse(const char *str, char ***argv, unsigned int *argc);
void argv_delete(char **argv);

#endif
