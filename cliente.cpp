#include <iostream>
#include <string>
#include <vector>
using namespace std;

/*
Input: un entero
Output: el entero normalizado a formato de 3 digitos
Descripcion: Esta funcion sirve para dar un formato
de 3bytes a un numero, ya que devuelve un string el
cual contiene la entrada.

Ejemplos:
- 34 -> 043
- 5 -> 005
- 100 -> 100
*/

string number_format(int number)
{
    string ans = to_string(number);
    int aux = ans.size();
    if (aux < 3)
        ans.insert(ans.begin(), 3 - aux, '0');
    return ans;
}

/*
Input: un string con el codigo del protocolo y un vector que contenga la
informacion que sera enviada a traves del protocolo
Output: un string, que es el protocolo
Descripcion: concatena un numero normalizado que especifica que tan largo
en la informacion que viene en seguida, esto lo hace con cada elemento del vector
para poder formar el protocolo

Ejemplos:
- C Julio -> C1005Julio
- C Julio ensena Redes -> C2005Julio006ensena005Redes
*/

string make_protocol(string protocol_command, vector<string> elements)
{
    string ans = protocol_command;
    
    for (int i = 1, s = elements.size(); i < s; ++i)
        ans += number_format(elements[i].size()) + elements[i];
    
    return ans;
}

/*
Input: un string que contiene el protocolo A
Output: el tipo de error, que se obtiene del protocolo A
Descripcion: imprime el mensaje recibido por el protocolo
y retorna el tipo de error

Ejemplos:
- ProtocoloA -> 1 or 2 or 3
*/

int interpret_protocol_A(string protocolA)
{
    cout << protocolA.substr(5, protocolA.size() - 1);
    return protocolA[1];
}

/*
Input: un string
Output: no retorna nada, porque trabaja directamente sobre la entrada
Descripcion: reemplaza cadenas de espacios de tamaño a n a un solo espacio
y si es que la cadena no tenia un espacio al final le añade uno

Ejemplos:
- "aaa    aaaa" -> "aaa aaaa"
- "aaa    aaaaaaa    " -> "aaa aaaaaaa "
- "aaa " -> "aaa "
- "aaa" -> "aaa "
*/

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

/*
Input: un string
Output: el numero de espacios que tiene el string
Descripcion: esta funcion depende de space_format, ya que
dara por hecho que el string es un output de esta funcion, entonces
lo que hace es contar ingenuamente cada espacio   
                                                  
Ejemplos:                                         
- "aaa aaa aaa " -> 3                                                 
*/                                                

int count_spaces(string str)
{
    int ans = 0;

    for (int i = str.find(' '), s = str.size(); i < s and i > -1; i = str.find(' ', i + 1), ans++);
    
    return ans;
}

/*
Input: un vector de string y la consulta
Output: se trabaja directamente sobre el vector de la entrada
Descripcion: el vector es rellenado con cada palabra que tenga la consulta
ya que esta esta en formalizada segunda la funcion format_space, entonces 
en el vector se guardara cada elemento de la consulta

Ejemplos:
- C Julio ensena Redes -> {C, Julio, ensena, Redes}
- C Julio -> {C, Julio}
*/

void split_query(vector<string>&ans, string query)
{
    while (!query.empty())
    {
        ans.push_back(query.substr(0, query.find(' ')));
        query.replace(0, query.find(' ') + 1, "");
    }
}

/*
Input: la consulta
Output: los elementos de la consulta
Descripcion: primero elimina el codigo GDBD de la query para poder empezar
a conseguir la informacion de esta con la funcion split_query

Ejemplos: 
- GDBD C Julio -> {C, Julio}

*/

vector<string> interpret_query(string query)
{
    vector<string> query_parts;
    query_parts.push_back(query.substr(5, 1));
    split_query(query_parts, query.substr(7, query.size() - 1));

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
        cout << "packet: " << packet << endl << endl;
        // wait protocol A
    }
}

int main()
{ 
    menu();
}
