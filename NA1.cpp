// C1004Alex
// C2004Alex008choteado006Sharon

#include <iostream>
#include <sqlite3.h>
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

int create(string protocolo, sqlite3*& db, char*& msgError){
    const char *sql;
    int rc;

    if(protocolo[1] == '1'){

        int tamNodo = stoi(protocolo.substr(2,3));
        string nodo = protocolo.substr(5,tamNodo);
        string sqlCommand = "INSERT INTO RELACIONES ( NODO1,RELACION,NODO2) VALUES('" + nodo + "', NULL, NULL)";  
        sql = sqlCommand.c_str();

        rc = sqlite3_exec(db, sql, callback, 0, &msgError);
        
    }else if(protocolo[1] == '2'){
        
        string sqlCommand = "INSERT INTO RELACIONES ( NODO1,RELACION,NODO2) VALUES(";

        for(int i = 0, j = 2; i < 3; i++){
            int tamNodo = stoi(protocolo.substr(j,3));
            string nodo = protocolo.substr(j+3,tamNodo);
            sqlCommand += '\'' + nodo + '\'';
            if(i != 2)
                sqlCommand += ',';
            j += (3 + tamNodo);
        }

        sqlCommand += ")";

        //cout << sqlCommand << endl;
        
        sql = sqlCommand.c_str();
        rc = sqlite3_exec(db, sql, callback, 0, &msgError);
    
    }else{
        cout << "Protocolo inexistente";
        exit(1);
    }

    return rc;
}


int main(){
// SQLLITE
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;

    rc = sqlite3_open("NA1.db", &db);
    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }

// PROTOCOLO
    string protocolo;
    getline(cin, protocolo);
    char tipo = protocolo[0];

    if(tipo == 'C'){
        rc = create(protocolo, db, zErrMsg);
    }

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Records created successfully\n");
    }

    sqlite3_close(db);
    
    return 0;
}