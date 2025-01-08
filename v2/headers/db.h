#ifndef DB_H
#define DB_H
#include<sqlite3.h>

#define FILE_SCHEMA "BEGIN TRANSACTION;\
CREATE TABLE `FILE_CONTENT` (`line_no` INTEGER PRIMARY KEY, `TEXT` TEXT);\
CREATE TABLE `LINE_MAPPING` (`virtual_start` INTEGER NOT NULL , `virtual_end` INTEGER NOT NULL, `actual_start` INTEGER NOT NULL , `actual_end` INTEGER NOT NULL);\
COMMIT;"

sqlite3 * init_empty_db(char *file_name);
sqlite3* open_database(const char *db_name);
void close_database(sqlite3 *db);
int execute_query(sqlite3 *db, const char *sql);


#endif