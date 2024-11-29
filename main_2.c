#include<stdio.h>
#include<sqlite3.h>
#include<string.h>

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
    const char *sql = "INSERT INTO LINES (file_name,line_no,content) values (?,?,?)";
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
        printf("******************************************\n");
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

    init_db(db);

    insert_lines(db);

    sqlite3_close(db);
    return 0;
}
