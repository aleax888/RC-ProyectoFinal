#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream> // std::cout
#include <thread>
#include <string>
#include <sys/stat.h> //tam
#include <vector>

using namespace std;


string number_format(int number)
{
    string ans = to_string(number);
    int aux = ans.size();
    if (aux < 3)
        ans.insert(ans.begin(), 3 - aux, '0');
    return ans;
}

string make_protocol(string protocol_command, vector<string> elements)
{
    string ans = protocol_command;
    
    for (int i = 1, s = elements.size(); i < s; ++i)
        ans += number_format(elements[i].size()) + elements[i];
    
    return ans;
}

int interpret_protocol_A(string protocolA)
{
    cout << protocolA.substr(5, protocolA.size() - 1);
    return protocolA[1];
}

void space_format(string &str)
{
    str.push_back(' ');
    for (int i = str.find(' '), s = str.size(); i < s and i > -1; i = str.find(' ', i))
    {
        int space_begin = i, count = 0;
        for (; i < s and i > -1 and str[i] == ' '; count++, i++);
        
        if (count > 0) 
            str.replace(space_begin, count, " ");
        i = space_begin + 1;
    }
}

int count_spaces(string str)
{
    int ans = 0;

    for (int i = str.find(' '), s = str.size(); i < s and i > -1; i = str.find(' ', i + 1), ans++);
    
    return ans;
}

void split_query(vector<string>&ans, string query)
{
    while (!query.empty())
    {
        ans.push_back(query.substr(0, query.find(' ')));
        query.replace(0, query.find(' ') + 1, "");
    }
}

vector<string> interpret_query(string query)
{
    vector<string> query_parts;
    query_parts.push_back(query.substr(5, 1));
    split_query(query_parts, query.substr(7, query.size() - 1));

    return query_parts;
}

void write_thread(int socket_cliente, char* ipaddress, unsigned int port)
{
    int n;
    string aux = "GDBD ";
    string packet;
    char buffer[1025];

    char myIP[16];
    unsigned int myPort;
    struct sockaddr_in my_addr;

    bzero(&my_addr, sizeof(my_addr));
    socklen_t len = sizeof(my_addr);


    while (true)
    {
        cout << aux;
        string query; getline(cin, query);
        query = aux + query + ' ';
        space_format(query);
        vector<string> query_parts = interpret_query(query);

        if (query_parts[0] == "C")
        {
            int count = count_spaces(query);
            if (count == 3)
                packet = make_protocol("C1", query_parts);
            else if (count == 5)
                packet = make_protocol("C2", query_parts);
        }

        else if (query_parts[0] == "R")
        {
            int count = count_spaces(query);
            if (count == 5)
            {
                query_parts.erase(query_parts.begin() + 2);
                packet = make_protocol("R1", query_parts) + "000";
            }
            else if (count == 4 and query_parts[query_parts.size() - 1] == "A")
                packet = make_protocol("R2", query_parts);
            else if (count == 4 and query_parts[query_parts.size() - 1] == "I")
                packet = make_protocol("R3", query_parts);
        }

        else if (query_parts[0] == "U")
        {
            int count = count_spaces(query);
            if (count == 6)
            {
                query_parts.erase(query_parts.begin() + 2);
                packet = make_protocol("U1", query_parts);
            }
            else if (count == 8)
                packet = make_protocol("U2", query_parts);
            else if (count == 4)
                packet = make_protocol("U3", query_parts);
        }

        else if (query_parts[0] == "D")
        {
            int count = count_spaces(query);
            if (count == 3)
                packet = make_protocol("D1", query_parts);
            else if (count == 5)
                packet = make_protocol("D2", query_parts);
            else if (count == 4)
                packet = make_protocol("D3", query_parts);
        }

        else
        {
            cout << "error" << endl;
        }

        // send packet
        cout << ">> " << ipaddress << " , " << port << " : " << "packet: " << packet << endl << endl;
        // wait protocol A

        strcpy(buffer, packet.c_str());
        n = write(socket_cliente, buffer, packet.size());

    }

    // El cliente cierra su conexion
    shutdown(socket_cliente, SHUT_RDWR);
    close(socket_cliente);
}

int main(void)
{
    struct sockaddr_in stSockAddr;
    int Res;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    int n;
    char buffer[1000];

    struct sockaddr_in server_addr, my_addr;

    unsigned int myPort;

    if (-1 == SocketFD)
    {
        perror("cannot create socket");
        exit(EXIT_FAILURE);
    }

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(45191);
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

    //Obtener Puerto y ipaddress del servidor
    char *myIP = inet_ntoa(stSockAddr.sin_addr);
	myPort = ntohs(stSockAddr.sin_port);


    // std::thread(read_thread, SocketFD).detach();
    std::thread(write_thread, SocketFD,myIP,myPort).detach();
    
    while (true)
    {
    }
    return 0;
}