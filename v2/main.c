#include<stdio.h>
#include<stdlib.h>

// Macro Definations
#define DB_DIR_PATH "./db"
#define DUMP_FILE_PATH "./io/tmp"


// Function declrations
int set_file(char *);
int set_db(char *);
int set_lang_flag(char *); // either -d (devnagiri) or -e (unicode english)
int read_into_the_file(int,int); 

// Global Variables
char *DB_PATH;
char *FILE_NAME;
char *LANG_FLAG;

int main(){
    return 0;
}