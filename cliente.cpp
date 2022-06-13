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
