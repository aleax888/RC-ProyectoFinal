#include <iostream>
#include <string>
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

string make_protocol_C1(string nombre_del_nodo)
{
    string ans = "C1" + number_format(nombre_del_nodo.size()) + nombre_del_nodo;
    return ans;
}

string make_protocol_C2(string nombre_del_nodo1, string nombre_de_la_relacion, string nombre_del_nodo2)
{
    string ans = "C2" + 
        number_format(nombre_del_nodo1.size()) + nombre_del_nodo1 +
        number_format(nombre_de_la_relacion.size()) + nombre_de_la_relacion +
        number_format(nombre_del_nodo2.size()) + nombre_del_nodo2;
    return ans;
}

string make_protocol_D1(string nombre_de_la_relacion)
{
    string ans = "D1" + number_format(nombre_de_la_relacion.size()) + nombre_de_la_relacion;
    return ans;
}

string make_protocol_D2(string nombre_del_nodo1, string nombre_de_la_relacion, string nombre_del_nodo2)
{
    string ans = "D2" +
        number_format(nombre_del_nodo1.size()) + nombre_del_nodo1 +
        number_format(nombre_de_la_relacion.size()) + nombre_de_la_relacion +
        number_format(nombre_del_nodo2.size()) + nombre_del_nodo2;
    return ans;
}

string make_protocol_D3(string nombre_del_nodo, string nombre_del_atributo)
{
    string ans = "D3" + 
        number_format(nombre_del_nodo.size()) + nombre_del_nodo +
        number_format(nombre_del_atributo.size()) + nombre_del_atributo;
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

    /*cout << "parsing: " << endl;
    for (int i = 0, s = query_parts.size(); i < s; ++i)
    {
        cout << query_parts[i] << endl;
    }*/

    return query_parts;
}



void menu()
{
    string aux = "GDBD ";
    string packet;
    
    while (true)
    {
        cout << aux;
        string query; getline(cin, query);
        query = aux + query + ' ';
        space_format(query);
        vector<string> query_parts = interpret_query(query);

        /*cout << "parsing: " << endl;
        for (int i = 0, s = query_parts.size(); i < s; ++i)
        {
            cout << query_parts[i] << endl;
        }*/

        if (query_parts[0] == "C")
        {
            int count = count_spaces(query);
            if (count == 3)
                packet = make_protocol_C1(query_parts[1]);
            else if (count == 5)
                packet = make_protocol_C2(query_parts[1], query_parts[2], query_parts[3]);

            // wait protocol A
        }

        else if (query_parts[0] == "R")
        {
            // working progress
        }

        else if (query_parts[0] == "U")
        {
            // working progress
        }

        else if (query_parts[0] == "D")
        {
            int count = count_spaces(query);
            if (count == 3)
                packet = make_protocol_D1(query_parts[1]);
            else if (count == 5)
                packet = make_protocol_D2(query_parts[1], query_parts[2], query_parts[3]);
            else if (count == 4)
                packet = make_protocol_D3(query_parts[1], query_parts[2]);

            // wait protocol A
        }

        else
        {
            cout << "error" << endl;
        }

        cout << "packet: " << packet << endl << endl;
    }
}

int main()
{ 
    menu();
}
