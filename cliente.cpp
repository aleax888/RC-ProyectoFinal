#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

void createProtocol(string comando, char* buffer, int& tamProtocolo);

int main(){
    
    char buffer[1000];
    
    struct sockaddr_in stSockAddr;
    int Res;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    int n;

    if (-1 == SocketFD)
    {
      perror("cannot create socket");
      exit(EXIT_FAILURE);
    }
 
    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
 
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(45081);
    Res = inet_pton(AF_INET, "5.253.235.219", &stSockAddr.sin_addr);
 
    if (0 > Res)
    {
      perror("error: first parameter is not a valid address family");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
    else if (0 == Res)
    {
      perror("char string (second parameter does not contain valid ipaddress");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }

    if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
      perror("connect failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }

    while (1)
    {

        string comando;
        int tamProtocolo = 0;
        cout << "> GDBD ";
        getline(cin, comando);

        cout << "Comnado Ingresado: " << comando <<endl;

        createProtocol(comando, buffer, tamProtocolo);
        cout << "Buffer: "<< buffer << endl;
        cout << "Tamaño: "<< tamProtocolo << endl;

        n = write(SocketFD, buffer, tamProtocolo);
        
        // n = read(SocketFD,buffer,1000);

        cout << buffer;

        memset(buffer, 0, 1000);
    }
    
}

void createProtocol(string comando, char* buffer, int& tamProtocolo){
    string protocol;
    char aux[1000];
    char type = comando[0];

    // Contar cantidad de espacios
    int cont = 0;
    for(size_t i = 0; i < comando.size(); i++){
        if(comando[i] == ' '){
            cont++;
        }
    }

    // Identificar tipo de comando
    if(type == 'C'){
        if(cont == 1){
            string type = "C1";
            size_t nodePos = comando.find(" ");
            size_t nodeLen = comando.size() - ((nodePos)+1);
            sprintf(aux, "%03d", nodeLen);
            aux[4] = '\0';
            string len = aux;
            string node = comando.substr( nodePos + 1, nodeLen);
            
            protocol = type + len + node;

        }
        if(cont == 3){
            comando.append(" ");
            string type = "C2";
            size_t nodePos = comando.find(" ");
            size_t auxPos;

            for (int i = 0; i < cont; i++){
                auxPos = comando.find(" ", nodePos+1, 1);
                size_t nodeLen = auxPos - (nodePos + 1);
                sprintf(aux, "%03d", nodeLen);
                aux[4] = '\0';
                string len = aux;
                type.append(len);
                type.append(comando.substr(nodePos + 1, nodeLen));

                nodePos = comando.find(" ", nodePos+1, 1);
            }
            protocol = type;
        }
    }

    for(size_t i = 0; i < protocol.size(); i++){
        *(buffer + i)= protocol[i];
    }
    buffer[protocol.size()] = '\0';

    tamProtocolo = protocol.size()+1;
}