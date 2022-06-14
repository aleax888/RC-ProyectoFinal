#include <sqlite3.h>
#include <iostream>
#include <string>

using namespace std;

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int createDB(int num){
    
    for(int i = 0; i < num; i++){
        sqlite3 *db;
        char *zErrMsg = 0;
        int rc;
        char *sql, *sql2;
        string name = "NA" + to_string(i+1) + ".db";
        const char* n = name.c_str();
        rc = sqlite3_open(n, &db);

        if( rc ) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            return(0);
        } else {
            fprintf(stderr, "Opened database successfully\n");
        }

        sql = "CREATE TABLE RELACIONES("  \
        "NODO1 TEXT NULL," \
        "RELACION TEXT NULL," \
        "NODO2 TEXT NULL);";

        sql2 = "CREATE TABLE ATRIBUTOS("  \
        "NODO TEXT NULL," \
        "ATRIBUTO TEXT NULL," \
        "VALOR TEXT NULL);";

        rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

        if( rc != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        } else {
            fprintf(stdout, "Table created successfully\n");
        }

        
        rc = sqlite3_exec(db, sql2, callback, 0, &zErrMsg);

        if( rc != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        } else {
            fprintf(stdout, "Table created successfully\n");
        }

        sqlite3_close(db);
    }
}

int main() {
    createDB(5);
}