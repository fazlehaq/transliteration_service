#include<stdio.h>
#include<sqlite3.h>
#include<string.h>
#include"queries.h"
#include<limits.h>


// Helper functions
int is_file_exists(sqlite3 *db,char *filename){
    const char *sql_query = FILE_EXISTS_QUERY;
    sqlite3_stmt *sql_stmt;
    int rc;

    rc = sqlite3_prepare_v2(db,sql_query,-1,&sql_stmt,NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1; // Error in preparing statement
    }
    
    sqlite3_bind_text(sql_stmt,1,filename,-1,SQLITE_STATIC);

    rc = sqlite3_step(sql_stmt);
    int exists;
    if (rc == SQLITE_ROW) {
        exists = sqlite3_column_int(sql_stmt, 0);
    } else {
        fprintf(stderr, "Error executing query: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(sql_stmt);

    return exists;
}

int get_last_line_number(sqlite3 *db,char * filename){
    const char *sql_query = LAST_LINE_NUMBER_QUERY;
    if (filename == NULL){
        printf("Enter the filename\n");
        scanf("%s",filename);
    }

    sqlite3_stmt *sql_stmt;
    int rc = sqlite3_prepare_v2(db,sql_query,-1,&sql_stmt,NULL);

    if (rc != SQLITE_OK){
        fprintf(stderr,"Could not prepare the statement : %s" ,sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_text(sql_stmt,1,filename,-1,SQLITE_STATIC);

    if( sqlite3_step(sql_stmt) == SQLITE_ROW){
        return sqlite3_column_int(sql_stmt,0);
    }

    return -1;

}


void init_db(sqlite3 *db){
    const char *sql1 = "CREATE TABLE IF NOT EXISTS LINES (file_name TEXT, line_no INT, content TEXT)";
    char *err_msg = 0;
    int rc = sqlite3_exec(db,sql1,0,0,&err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }
}

int insert_line(sqlite3 *db,char *filename,int line_no,char *content){
    const char *sql = INSERT_LINE_QUERY;
    printf("%s %d %s \n",filename,line_no,content);
    sqlite3_stmt *sql_stmt;
    sqlite3_prepare_v2(db,sql,-1,&sql_stmt,NULL);
    sqlite3_bind_text(sql_stmt,1,filename,-1,SQLITE_STATIC);
    sqlite3_bind_int(sql_stmt,2,line_no);
    sqlite3_bind_text(sql_stmt,3,content,-1,SQLITE_STATIC);

    int rc = sqlite3_step(sql_stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr,"Failed to insert the line %s\n",sqlite3_errmsg(db));
    }   

    sqlite3_finalize(sql_stmt);
}

void insert_lines(sqlite3 *db) {
    char filename[FILENAME_MAX];
    char line_content[1024 * 10];
    int line_no = 1;

    printf("Enter filename : \n");
    scanf("%s", filename);
    printf("%s\n", filename);

    // Clear buffer after reading filename
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    while (1) {
        int choice;
        printf("******************************************\n");
        printf("1) Add line \n");
        printf("2) Stop \n");
        printf("Enter your choice : ");
        scanf("%d", &choice);

        // Clear buffer after reading choice
        while ((c = getchar()) != '\n' && c != EOF);

        switch (choice) {
            case 1:
                printf("Enter the line content: ");
                fgets(line_content, sizeof(line_content), stdin);

                // Remove trailing newline
                line_content[strcspn(line_content, "\n")] = '\0'; 
                
                insert_line(db, filename, line_no, line_content);
                line_no++;
                break;
            case 2:
                return;
            default:
                printf("Invalid choice. Try again.\n");
                break;
        }
    }
}

void import_file(sqlite3 *db){
    char filename[FILENAME_MAX];
    printf("Enter the name of the file : ");
    scanf("%s",filename);

    FILE *fp = fopen(filename,"rb");
    if (fp == NULL){
        fprintf(stderr,"Could not open the file %s\n",filename);
        return ;
    }
    int line_no = 1;
    char BUFFER[1024*10];
    while(fgets(BUFFER,sizeof(BUFFER),fp) != NULL){
        BUFFER[strcspn(BUFFER, "\n")] = '\0'; 
        insert_line(db,filename,line_no,BUFFER);
        line_no ++;
    }
    printf("File saved ! \n\n");
}

void list_lines(sqlite3 *db){
    char *sql_query = GET_LINES_IN_RANGE_QUERY;
    int start,end;
    char filename[FILENAME_MAX];
    printf("Enter the name of the file\n");
    scanf("%s",filename);
    printf("Enter start & end : ");
    scanf("%d",&start);
    scanf("%d",&end);

    printf("%d %d\n",start,end);

    int rc;

    sqlite3_stmt *sql_stmt;
    rc = sqlite3_prepare_v2(db,sql_query,-1,&sql_stmt,NULL);

    if (rc != SQLITE_OK){
        fprintf(stderr,"Could not prepare the statement : %s" ,sqlite3_errmsg(db));
        return;
    }
    
    sqlite3_bind_text(sql_stmt,1,filename,-1,NULL);
    sqlite3_bind_int(sql_stmt,2,start);
    sqlite3_bind_int(sql_stmt,3,end);

    while(sqlite3_step(sql_stmt) == SQLITE_ROW){
        int line_no = sqlite3_column_int(sql_stmt,1);
        const char * line_content = sqlite3_column_text(sql_stmt,2);
        printf("%d  %s\n",line_no,line_content);
    }

}


int main(int argc,char *argv[]){
    char *db_uri = "./db.db";
    if (argc > 1){
        db_uri = argv[1];
    }

    sqlite3 *db;
    int rc = sqlite3_open(db_uri,&db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    } else {
        fprintf(stdout, "Opened database successfully\n");
    }

    // initialize db 
    init_db(db);

    while(1){
        int choice;
        printf("************************\n");
        printf("1) Add new file\n");
        printf("2) Append lines to file\n");
        printf("3) Import file\n");
        printf("4) Get file\n");
        printf("5) Get lines in range\n");
        printf("5) Exit\n");
        printf("Your choice : ");
        scanf("%d",&choice);

        switch (choice){
            case 1:
                insert_lines(db);
                break;
            case 3:
                import_file(db);
                break;
            case 5 :
                list_lines(db);
                break;

            case 2:
            case 4:
                printf("Not implemented Yet");
                break;
                

            default:
                printf("Incorrect choice\n");
                break;
        }

        if (choice == 6) break;
        printf("************************\n");

    }
    
    sqlite3_close(db);
    return 0;
}
