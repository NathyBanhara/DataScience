#include <iostream>
#include "graph.hpp"
#include "queue.hpp"

using namespace std;

void Graph::getAtt(string name, string type, string parent, Queue q)
{
    if (type == "array") insertNode(name, type);

    if (type != "array") insertEdge(parent, name, type);

    q.pop();

    if (type != "array") return;

    else
    {
        while (q.front()->name != "end_array")
            getAtt(q.front()->name, q.front()->type, name, q);
    }
    q.pop();
}

Node *createNode (string type, string name)
{
    Node *newOne = new Node;
    newOne->type = type;
    newOne->next = NULL;
    newOne->name = name;
    
    return newOne;
}

void Graph::insertNode(string name, string type)
{
    Node *newOne = createNode(type, name);
    name = name;
    listNodes[name] = newOne;
    return;
}

Edge *createEdge(string nameEdge, string type)
{
    Edge *e = new Edge;
    e->type = type;
    e->value = nameEdge;
    e->read = false;
    e->next = NULL;

    return e;
}

void Graph::insertEdge(string node, string nameEdge, string type)
{   
    unordered_map<string, Node*>::const_iterator got = listNodes.find(node);

    if (got->second->next == NULL) //if the list of edges is null
    {
        Edge *e = createEdge(nameEdge, type);
        got->second->next = e;
        return;
    }
    else
    {
        Edge *e = createEdge(nameEdge, type);
        e->next = got->second->next; //if it doesn't exist yet
        got->second->next = e;
        return;
    }
}

void Graph::printGraph()
{
    for (auto x : listNodes)
    {
        cout << x.first;
        Edge *aux = x.second->next;
        while (aux != NULL)
        {
            cout << " -> " << aux->value << " -> " << aux->type << ";";
            aux = aux->next;
        }
        
        cout << "\n";
    }
}

void Graph::freeGraph()
{
    listNodes.clear();
}