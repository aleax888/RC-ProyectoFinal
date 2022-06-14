// C1004Alex
// C2004Alex008choteado006Sharon

#define _CRT_SECURE_NO_DEPRECATE
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <thread> 
#include <iostream>
#include <sqlite3.h>
#include <string>

using namespace std;

// SQLLITE
sqlite3 *db;
char *zErrMsg = 0;
int rc;
char *sql;

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

int delete_(string protocolo, sqlite3*& db, char*& msgError){
    const char *sql;
    int rc;

    if(protocolo[1] == '1'){

        int tamRelacion = stoi(protocolo.substr(2,3));
        string relacion = protocolo.substr(5,tamRelacion);

        string sqlCommand = "DELETE FROM RELACIONES WHERE RELACION LIKE '%" + relacion + "%'";  
        sql = sqlCommand.c_str();

        rc = sqlite3_exec(db, sql, callback, 0, &msgError);
        
    }else if(protocolo[1] == '2'){
        // DELETE FROM RELACIONES WHERE NODO1 LIKE '%Alex%' AND RELACION LIKE '%Alumno%' AND NODO2 LIKE '%Julio%';
        int j = 2;
        string sqlCommand = "DELETE FROM RELACIONES WHERE NODO1 LIKE '%";
        
        int tamRelacion = stoi(protocolo.substr(j,3));
        sqlCommand += protocolo.substr(j+3,tamRelacion);
        sqlCommand += "%' AND RELACION LIKE '%";
        int tamRelacion2 = stoi(protocolo.substr(j + 3 + tamRelacion, 3));
        sqlCommand += protocolo.substr(j+6+tamRelacion,tamRelacion2);
        sqlCommand += "%' AND NODO2 LIKE '%";
        int tamRelacion3 = stoi(protocolo.substr(j + 6 + tamRelacion + tamRelacion2, 3));
        sqlCommand += protocolo.substr(j+9+tamRelacion+tamRelacion2,tamRelacion3);
        sqlCommand += "%'";

        sql = sqlCommand.c_str();
        rc = sqlite3_exec(db, sql, callback, 0, &msgError);
    
    }else if(protocolo[1] == '3'){
        
    }else{
        cout << "Protocolo inexistente";
        exit(1);
    }

    return rc;

}

void read_thread(int socket_cliente) {
    char buffer[1025];
    int tam = 1025;
    int n;
    string protocolo;

    while (true)
    {
        n = read(socket_cliente,buffer,tam);
        buffer[n] = '\0';

        protocolo = buffer;
        char tipo = protocolo[0];
        if(tipo == 'C'){
            rc = create(protocolo, db, zErrMsg);
        }
        if(tipo == 'D'){
            rc = delete_(protocolo, db, zErrMsg);
        }

        if( rc != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        } else {
            fprintf(stdout, "Records created successfully\n");
        }
    }
}

int main(){

// SQLLITE
    rc = sqlite3_open("NA5.db", &db);
    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }

// SERVIDOR
    struct sockaddr_in stSockAddr;
    int SocketServer = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    char buffer[256];
    int n;

     if(-1 == SocketServer)
    {
      perror("can not create socket");
      exit(EXIT_FAILURE);
    }
 
    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
 
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(45005);
    stSockAddr.sin_addr.s_addr = INADDR_ANY;
 
    if(-1 == bind(SocketServer,(const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
      perror("error bind failed");
      close(SocketServer);
      exit(EXIT_FAILURE);
    }
 
    if(-1 == listen(SocketServer, 10))
    {
      perror("error listen failed");
      close(SocketServer);
      exit(EXIT_FAILURE);
    }

    for(;;) {
      int newSocketCli = accept(SocketServer, NULL, NULL);
      
      if(0 > newSocketCli)
      {
            perror("error accept failed");
            close(SocketServer);
            exit(EXIT_FAILURE);
      }
 
   	    std::thread (read_thread,newSocketCli).detach();
    }
    
    sqlite3_close(db);
    
    return 0;
}