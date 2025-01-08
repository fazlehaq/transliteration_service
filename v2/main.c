#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<linux/limits.h>
#include"headers/utils.h"
#include"headers/db.h"

// Macro Definations
#define DB_DIR_PATH "./db"
#define DUMP_FILE_DIR_PATH "./io"  


// Function declrations
int set_file(char *); // sets the global variable 
int set_dump_file(char *);   // sets the global variable
int set_lang_flag(char *); // either -d (devnagiri) or -e (unicode english)
int read_into_the_file(int,int); 
int insert_file(char *); // creates new file
int get_file_path(char *); // retrieves the name of the file
int get_db_path(char *); // retrieves the name of the db


// Global Variables
char LANG_FLAG[3];
char DB_PATH[PATH_MAX];
char DUMP_FILE_PATH[PATH_MAX];


int main(){
    set_lang_flag(" -h");
    set_lang_flag(" -e ");
    init_empty_db("./db/test.db");
    return 0;
}

int set_file(char * file_name){
    char file_path[PATH_MAX];
    snprintf(file_path,PATH_MAX,"%s/%s.db",DB_DIR_PATH,file_name);

    if (is_file_exists(file_path) == 0){
        fprintf(stderr,"No such file : %s\n",file_name);
        return 0;
    }

    strcpy(DB_PATH,file_path);
    printf("File selected : %s \n",DB_PATH);
    return 1;
}

int set_dump_file(char * file_name){
    char file_path[PATH_MAX];
    snprintf(file_path,PATH_MAX,"%s/%s",DUMP_FILE_DIR_PATH,file_name);

    if (is_file_exists(file_path) == 0){
        fprintf(stderr,"No such file : %s\n",file_name);
        return 0;
    }

    strcpy(DUMP_FILE_PATH,file_path);
    printf("File selected : %s \n",DUMP_FILE_PATH);
    return 1;
}

int set_lang_flag(char *flag){

    if (strcmp(flag,"-h")){
        strcpy(LANG_FLAG,"-h");
        return 1;
    }else if(strcmp(flag,"-d")){
        strcpy(LANG_FLAG,"-d");
        return 1;
    }

    return 0;
}