#ifndef UTILS_H
#define UTILS_H

int is_file_exists(char *);
FILE* create_new_file(char *filename);
int itrans(char *src,char *dest, char *flag);
#endif