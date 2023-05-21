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
        void insertNode(string name, string type);
        void insertEdgeArray(string nameNode, string nameEdge, string type);
        void insertEdge(string node, string nameEdge, string type);
        int findNodeIndex(string name, string type);
        void percentualEdge();
        void printGraph();
        void printTypes();
        void freeGraph();
};

#endif