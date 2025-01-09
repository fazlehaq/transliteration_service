#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<linux/limits.h>
#include"headers/utils.h"
#include"headers/db.h"

// Macro Definations
#define DB_DIR_PATH "./db"
#define DUMP_FILE_DIR_PATH "./io"  
#define ITRANS_IO_FILE_PATH "./io/itrans_io"

// Function declrations
int set_file(char *); // sets the global variable 
int set_dump_file(char *);   // sets the global variable
int set_lang_flag(char *); // either -d (devnagiri) or -e (unicode english)
int read_into_the_file(int,int); 
int insert_file(char *); // creates new file and stores lines from the dump file
int get_file_path(char *); // retrieves the name of the file
int get_db_path(char *); // retrieves the name of the db


// Global Variables
char LANG_FLAG[3];
char DB_PATH[PATH_MAX];
char DUMP_FILE_PATH[PATH_MAX];


int main(){
    set_dump_file("service_io");
    set_lang_flag("-e");
    insert_file("trans");
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

int insert_file(char *filename) {
    // building file path 
    char *filepath;
    filepath = (char *) malloc(strlen(DB_DIR_PATH) + strlen(filename) + 1 + 3 + 1);
    snprintf(filepath, strlen(DB_DIR_PATH) + strlen(filename) + 1 + 3 + 1, "%s/%s.db", DB_DIR_PATH, filename);
    
    // initializing empty sqlite file
    sqlite3 *db = init_empty_db(filepath);
    if (!db) {
        fprintf(stderr, "Failed to open database.\n");
        return 0;
    }

    // opening dump file to read from
    FILE *file = fopen(DUMP_FILE_PATH, "r");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", DUMP_FILE_PATH);
        close_database(db);
        return 0;
    }

    // Setting up query
    char *begin_transaction = "BEGIN TRANSACTION;";
    char *commit_transaction = "COMMIT;";
    char *rollback_transaction = "ROLLBACK;";

    // Starting the transaction
    int rc = execute_query(db, begin_transaction);

    // If transactions fails
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to begin transaction: %s\n", sqlite3_errmsg(db));
        fclose(file);
        close_database(db);
        return 0;
    }

    char *line = NULL;
    char *corresponding_line = NULL;
    size_t len = 0;
    size_t corresponding_len = 0;
    int line_no = 1;
    int inserted_lines = 0;

    // Preparing sqlite3 statement
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO FILE_CONTENT (line_no, TEXT_ENGLISH, TEXT_DEVNAGIRI) VALUES (?, ?, ?)";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        fclose(file);
        close_database(db);
        return 0;
    }

    // Doing transliteration
    itrans(DUMP_FILE_PATH, ITRANS_IO_FILE_PATH, strncmp(LANG_FLAG, "-e", 2) == 0 ? "-he" : "-eh");

    // transliterated file
    FILE *itrans_io_file = fopen(ITRANS_IO_FILE_PATH, "r");
    if (!itrans_io_file) {
        fprintf(stderr, "Failed to open file: %s\n", ITRANS_IO_FILE_PATH);
        sqlite3_finalize(stmt);
        fclose(file);
        close_database(db);
        return 0;
    }

    // reading line by line from both files and inserting
    while (getline(&line, &len, file) != -1) {
        getline(&corresponding_line, &corresponding_len, itrans_io_file);
        line[strcspn(line, "\n")] = '\0';
        corresponding_line[strcspn(corresponding_line, "\n")] = '\0';

        sqlite3_bind_int(stmt, 1, line_no);
        if (strcmp(LANG_FLAG, "-e") == 0) {
            sqlite3_bind_text(stmt, 2, line, -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, corresponding_line, -1, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_text(stmt, 2, corresponding_line, -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, line, -1, SQLITE_TRANSIENT);
        }

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            fprintf(stderr, "Failed to insert into FILE_CONTENT: %s\n", sqlite3_errmsg(db));
            execute_query(db, rollback_transaction);
            sqlite3_finalize(stmt);
            fclose(file);
            fclose(itrans_io_file);
            free(line);
            close_database(db);
            return 0;
        }

        sqlite3_reset(stmt); // Reset the statement for the next use
        line_no++;
        inserted_lines++;
    }

    sqlite3_finalize(stmt);
    free(line);
    fclose(file);
    fclose(itrans_io_file);

    sqlite3_stmt *mapping_stmt;
    const char *mapping_sql = "INSERT INTO LINE_MAPPING (virtual_start, virtual_end, actual_start, actual_end) VALUES (?, ?, ?, ?)";
    if (sqlite3_prepare_v2(db, mapping_sql, -1, &mapping_stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare LINE_MAPPING statement: %s\n", sqlite3_errmsg(db));
        execute_query(db, rollback_transaction);
        close_database(db);
        return 0;
    }

    sqlite3_bind_int(mapping_stmt, 1, 1);
    sqlite3_bind_int(mapping_stmt, 2, inserted_lines);
    sqlite3_bind_int(mapping_stmt, 3, 1);
    sqlite3_bind_int(mapping_stmt, 4, inserted_lines);

    if (sqlite3_step(mapping_stmt) != SQLITE_DONE) {
        fprintf(stderr, "Failed to insert into LINE_MAPPING: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(mapping_stmt);
        execute_query(db, rollback_transaction);
        close_database(db);
        return 0;
    }

    sqlite3_finalize(mapping_stmt);

    rc = execute_query(db, commit_transaction);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to commit transaction: %s\n", sqlite3_errmsg(db));
        close_database(db);
        return 0;
    }

    close_database(db);
    return 1;
}
