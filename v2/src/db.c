#include<stdio.h>
#include<sqlite3.h>
#include"../headers/db.h"
#include"../headers/utils.h"

sqlite3* open_database(const char *db_name) {
    sqlite3 *db;
    int rc = sqlite3_open(db_name, &db);
    if (rc) {
        fprintf(stderr, "Error opening database (%s): %s\n", db_name, sqlite3_errmsg(db));
        return NULL;
    }
    return db;
}

void close_database(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

int execute_query(sqlite3 *db, const char *sql) {
    char *errmsg = NULL;
    int rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return !SQLITE_OK; // Failure
    }
    return SQLITE_OK; // Success
}

sqlite3 * init_empty_db(char *file_name){
    FILE * file = create_new_file(file_name);
    if(file == NULL) 
        return NULL;
    fclose(file);

    sqlite3 *db = open_database(file_name);
    if(db == NULL) 
        return NULL;

    char *sql = FILE_SCHEMA;
    
    if(execute_query(db,sql) == SQLITE_OK) {
        printf("Initialized sucessfully\n");
        return db;
    }

    // Base case
    printf("Could not create file\n");
    return NULL;
}