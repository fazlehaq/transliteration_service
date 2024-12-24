#include<stdio.h>
#include<stdlib.h>
#include<linux/limits.h>

// Macro Definations
#define DB_DIR_PATH "./db"
#define DUMP_FILE_PATH "./io/tmp"


// Function declrations
int set_file(char *); // sets the global variable 
int set_db(char *);   // sets the global variable
int set_lang_flag(char *); // either -d (devnagiri) or -e (unicode english)
int read_into_the_file(int,int); 
int insert_file(char *); // creates new file
int get_file(char *); // retrieves the name of the file
int get_db(char *); // retrieves the name of the db

// Global Variables
char DB_PATH[NAME_MAX];
char FILE_NAME[NAME_MAX];
char LANG_FLAG[2];

int main(){
    return 0;
}