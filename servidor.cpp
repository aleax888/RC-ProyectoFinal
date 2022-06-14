#define _CRT_SECURE_NO_DEPRECATE
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>       
#include <thread> 
#include <string>
#include <fstream>
#include <filesystem>
#include <map>
#include <vector>

using namespace  std;
map<int,string> room;

vector<int> port_list = {45001,45002,45003,45004,45005};

int hash_nodo(string protocolo) {

	int tam = stoi(protocolo.substr(2, 3));
	string nodo = protocolo.substr(5, tam);

	int total = 0;
	for (int i = 0; i < nodo.size(); i++)
	{
		total += char(nodo[i]);
	}
	return total%5;
}

string reverse_protocolo(string protocolo01) {
	
	//C2005Julio008educador002RC
	string protocolo = protocolo01;
	
	string protocolo_reverse = protocolo.substr(0,2);				//C2
	protocolo.erase(0, 2);

	int nodo01_size = stoi(protocolo.substr(0, 3));					//5
	string nodo01 = protocolo.substr(0,nodo01_size+3);				//005Julio
	protocolo.erase(0, nodo01_size + 3);

	int relacion_size = stoi(protocolo.substr(0, 3));				//8
	string relacion = protocolo.substr(0, relacion_size+3);			//008educador
	protocolo.erase(0, relacion_size + 3);

	//protocolo queda 002RC

	protocolo_reverse += protocolo;							//C2002RC
	protocolo_reverse += relacion;							//C2002RC008educador
	protocolo_reverse += nodo01;							//C2002RC008educador005educador

	return protocolo_reverse;
}

vector<pair<int, string>> hash_proto(string protocolo) {

	map<string, int> mymap;
	mymap.insert(pair<string, int>("C1", 1));
	mymap.insert(pair<string, int>("C2", 2));
	mymap.insert(pair<string, int>("U1", 3));
	mymap.insert(pair<string, int>("U2", 4));
	mymap.insert(pair<string, int>("U3", 5));
	mymap.insert(pair<string, int>("D1", 6));
	mymap.insert(pair<string, int>("D2", 7));
	mymap.insert(pair<string, int>("D3", 8));
	mymap.insert(pair<string, int>("R1", 9));
	mymap.insert(pair<string, int>("R2", 10));
	mymap.insert(pair<string, int>("R3", 11));

	map<string, int>::iterator it;
	it = mymap.find(protocolo.substr(0,2));

	vector<pair<int, string>> hash_protocolo;

	int nodo01 = 0;
	int nodo02 = 0;
	int tam = 0;
	string protocolo_reverse = "";

	switch (it->second)
	{
	case 1:
		//C1005Julio
		nodo01 = hash_nodo(protocolo);
		hash_protocolo.push_back(make_pair(nodo01, protocolo));
		break;

	case 2:
		//C2005Julio008educador002RC
		nodo01 = hash_nodo(protocolo);
		hash_protocolo.push_back(make_pair(nodo01, protocolo));

		protocolo_reverse = reverse_protocolo(protocolo);
		nodo02 = hash_nodo(protocolo_reverse);
		hash_protocolo.push_back(make_pair(nodo02, protocolo_reverse));
		break;

	case 3:
		//U1005Julio005email0061se.pe
		nodo01 = hash_nodo(protocolo);
		hash_protocolo.push_back(make_pair(nodo01, protocolo));
		break;

	case 4:
		//U2005Julio008educador002RC
		nodo01 = hash_nodo(protocolo);
		hash_protocolo.push_back(make_pair(nodo01, protocolo));
		break;

	case 5:
		//U3005Julio006Julius
		nodo01 = hash_nodo(protocolo);
		hash_protocolo.push_back(make_pair(nodo01, protocolo));
		break;

	case 6:
		//D1002RC
		// nodo01 = hash_nodo(protocolo);
		// hash_protocolo.push_back(make_pair(nodo01, protocolo));

		for (int i = 0; i < 5; i++)
		{
			hash_protocolo.push_back(make_pair(i, protocolo));
		}
		
		break;

	case 7:
		//D2005Julio008educador002RC
		nodo01 = hash_nodo(protocolo);
		hash_protocolo.push_back(make_pair(nodo01, protocolo));

		protocolo_reverse = reverse_protocolo(protocolo);
		nodo02 = hash_nodo(protocolo_reverse);
		hash_protocolo.push_back(make_pair(nodo02, protocolo_reverse));
		break;

	case 8:
		//D3005Julio012Nacionalidad
		nodo01 = hash_nodo(protocolo);
		hash_protocolo.push_back(make_pair(nodo01, protocolo));
		break;

	case 9:
		//R1005Julio0015000
		nodo01 = hash_nodo(protocolo);
		hash_protocolo.push_back(make_pair(nodo01, protocolo));
		break;

	case 10:
		//R2005Julio001A
		nodo01 = hash_nodo(protocolo);
		hash_protocolo.push_back(make_pair(nodo01, protocolo));
		break;

	case 11:
		//R3005Julio001I
		nodo01 = hash_nodo(protocolo);
		hash_protocolo.push_back(make_pair(nodo01, protocolo));
		break;

	default:
		break;
	}

	return hash_protocolo;
}

void write_nodo_almacenamiento(pair<int, string> hash_protocolo){
    struct sockaddr_in stSockAddr;
    int Res;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    int n;
    char buffer[1000];
	unsigned int myPort;
    if (-1 == SocketFD)
    {
        perror("cannot create socket");
        exit(EXIT_FAILURE);
    }

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(port_list[hash_protocolo.first]);
    //Res = inet_pton(AF_INET, "5.253.235.219", &stSockAddr.sin_addr);
    Res = inet_pton(AF_INET, "127.0.0.1", &stSockAddr.sin_addr);

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

    // Cliente manda su conexion y si no hay problema todo okidoki
    if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
        perror("connect failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }
    string proto = hash_protocolo.second.c_str();
    strcpy(buffer, proto.c_str());
    buffer[proto.size()] = '\0';
    
	//Obtener Puerto y ipaddress del servidor
    char *myIP = inet_ntoa(stSockAddr.sin_addr);
	myPort = ntohs(stSockAddr.sin_port);

    n = write(SocketFD, buffer, proto.size() + 1 );

	// send packet
	cout << ">> " << myIP << " , " << myPort << " : " << "packet: " << proto << endl << endl;
	// wait protocol A

}

void read_thread(int socket_cliente, char* ipaddress, unsigned int port) {
  	char buffer[1025];
  	int tam = 1025;
  	int n;

  	cout << "Cliente Conectado" << endl;
  	string protocolo;
  	while(true){

		n = read(socket_cliente,buffer,tam);

		buffer[n] = '\0';
		
		protocolo = buffer;

		// send packet
        cout << "<< " << ipaddress << " , " << port << " : " << "packet: " << protocolo << endl << endl;
        // wait protocol A


		vector<pair<int, string>> hash_protocolo = hash_proto(protocolo);
		
		for (int i = 0; i < hash_protocolo.size(); i++)
		{
			write_nodo_almacenamiento(hash_protocolo[i]);
		}

		
  	}

  	shutdown(socket_cliente, SHUT_RDWR);
 	close(socket_cliente);

  	cout << "Read_thread termino.\n";
}

int main(void){

    struct sockaddr_in stSockAddr;
    int SocketServer = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    char buffer[256];
    int n;

	struct sockaddr_in ClientAddr;
	socklen_t SinSize = sizeof(sockaddr_in);
	unsigned int myPort;

    if(-1 == SocketServer)
    {
      perror("can not create socket");
      exit(EXIT_FAILURE);
    }
 
    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
 
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(45191);
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
 
    for(;;) // muchos  clientes      
    {
      	int newSocketCli = accept(SocketServer, (struct sockaddr *)&ClientAddr,(socklen_t*)&SinSize);
		

		//Obtener Puerto y ipaddress del cliente
		char *myIP = inet_ntoa(ClientAddr.sin_addr);
		myPort = ntohs(ClientAddr.sin_port);

      if(0 > newSocketCli)
      {
        perror("error accept failed");
        close(SocketServer);
        exit(EXIT_FAILURE);
      }
	
      std::thread (read_thread,newSocketCli,myIP,myPort).detach();
  //  std::thread (write_thread,newSocketCli).detach();
    }
 
    close(SocketServer);
    return 0;
}