#include<sys/stat.h>
#include"../headers/utils.h"

/// @brief Takes the path of the file and check if exists
/// @param path path of the file
/// @return 1 if exists else 0
int is_file_exists(char * path){
    struct stat buffer;
    return stat(path,&buffer) == 0;
}