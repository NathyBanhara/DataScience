#ifndef GRAPH_HPP

#define GRAPH_HPP

#include <iostream>
#include <unordered_map>
#include "queue.hpp"

using namespace std;

struct Edge
{
    string value;
    string type;
    bool read;
    Edge *next;
};

struct Node
{
    string name;
    string type;
    Edge *next;
};

class Graph
{
    public:
        unordered_map<string, Node*> listNodes;

        void getAtt(string name, string type, string parent, Queue q);
        void printGraph();
        void insertNode(string name, string type);
        void insertEdge(string node, string nameEdge, string type);
        void freeGraph();
};

#endif