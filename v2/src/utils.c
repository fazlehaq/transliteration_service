#include<sys/stat.h>
#include<stdio.h>
#include"../headers/utils.h"

/// @brief Takes the path of the file and check if exists
/// @param path path of the file
/// @return 1 if exists else 0
int is_file_exists(char * path){
    struct stat buffer;
    return stat(path,&buffer) == 0;
}

FILE* create_new_file(char *filename) {
    if (is_file_exists(filename)) {
        fprintf(stderr,"File '%s' already exists.\n", filename);
        return NULL;
    }

    FILE *file = fopen(filename, "w"); // Create an empty file
    if (!file) {
        fprintf(stderr, "Failed to create file '%s'.\n", filename);
        return NULL; // Failure
    }

    return file; // Success
}
    