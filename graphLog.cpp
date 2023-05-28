#include <iostream>
#include "graphLog.hpp"
#include <queue>
#include <string>
#include <fstream>
#include "postgres.hpp"

using namespace std;

void GraphLog::getAtt()
{
    std::queue<string> q = readLog();

    while (!q.empty())
    {
        string front = q.front();
        string name = "";
        if (front.find("start") != std::string::npos)
        {
            int i = 0;

            while (true)
            {
                if (front[i] == 'T')
                {
                    while(front[i] != '>')
                    {
                        name += front[i];
                        i++;
                    }
                }
                i++;
                if (name.length()) break;
            }

            insertNode(name);
        }
        else if (front.find("commit") != std::string::npos)
        {
            int i = 0;

            while (true)
            {
                if (front[i] == 'T')
                {
                    while(front[i] != '>')
                    {
                        name += front[i];
                        i++;
                    }
                }
                i++;
                if (name.length()) break;
            }

            listNodes[name]->commited = true;
        }
        else
        {
            int i = 0;
            bool end = false;

            string column = "", oldValue = "", newValue = "", id = "";

            while (true)
            {
                if (front[i] == 'T')
                {
                    while (front[i] != ',')
                    {
                        name += front[i];
                        i++;
                    }
                    i++;
                    while (front[i] != ',')
                    {
                        id += front[i];
                        i++;
                    }
                    i++;
                    while (front[i] != ',')
                    {
                        column += front[i];
                        i++;
                    }
                    i++;
                    while (front[i] != ',')
                    {
                        oldValue += front[i];
                        i++;
                    }
                    i++;
                    while (front[i] != '>')
                    {
                        newValue += front[i];
                        i++;
                        end = true;
                    }
                }
                i++;
                if (end) break;
            }
            operations.push_back(name);
            insertEdge(name, stoi(id), column, oldValue, newValue);
        }
        q.pop();
    }
}

std::queue<string> GraphLog::readLog()
{
    std::queue<string> q;

    std::ifstream log("log.txt");

    if (!log) {
        std::cout << "Erro ao abrir o log." << std::endl;
        return q;
    }

    std::string line;

    while (std::getline(log, line))
    {
        q.push(line);
    }

    log.close();

    return q;
}

NodeLog *createNode (string name)
{
    NodeLog *newOne = new NodeLog;
    newOne->first = NULL;
    newOne->last = NULL;
    newOne->name = name;
    newOne->commited = false;

    return newOne;
}

void GraphLog::insertNode(string name)
{
    NodeLog *newOne = createNode(name);

    listNodes[name] = newOne;
    return;
}

EdgeLog *createEdge(int id, string column, string oldValue, string newValue)
{
    EdgeLog *e = new EdgeLog;
    e->id = id;
    e->column = column;
    e->oldValue = oldValue;
    e->newValue = newValue;
    e->next = NULL;
    e->prev = NULL;

    return e;
}

void GraphLog::insertEdge(string node, int id, string column, string oldValue, string newValue)
{   
    unordered_map<string, NodeLog*>::const_iterator got = listNodes.find(node);

    if (got->second->first == NULL) //if the list of edges is null
    {
        EdgeLog *e = createEdge(id, column, oldValue, newValue);
        got->second->first = e;
        got->second->last = e;
        return;
    }
    else
    {
        for (EdgeLog *aux = got->second->first; aux != NULL; aux = aux->next)
        {
            if (aux->next == NULL)
            {
                EdgeLog *e = createEdge(id, column, oldValue, newValue);
                e->next = NULL;
                e->prev = aux;
                aux->next = e;
                got->second->last = e;
                return;
            }
        }
    }
}

void GraphLog::removeREDO(string node)
{
    EdgeLog *aux = listNodes[node]->first;
    listNodes[node]->first = listNodes[node]->first->next;
    free(aux);
}

void GraphLog::removeUNDO(string node)
{
    EdgeLog *aux = listNodes[node]->last;
    listNodes[node]->last = listNodes[node]->last->prev;
    free(aux);
}

void GraphLog::printGraph()
{
    for (auto x : listNodes)
    {
        cout << x.first;
        cout << " -> " << x.second->commited << ": ";
        EdgeLog *aux = x.second->first;
        while (aux != NULL)
        {
            cout << " -> " << aux->column << " -> " << aux->id << " -> " << aux->oldValue << " -> " << aux->newValue << "\n";
            aux = aux->next;
        }
        
        cout << "\n";
    }

    for (auto x : operations) cout << x << "\n";
}

void GraphLog::freeGraph()
{
    listNodes.clear();
}