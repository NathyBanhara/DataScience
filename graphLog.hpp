#ifndef GRAPHLOG_HPP

#define GRAPHLOG_HPP

#include <iostream>
#include <queue>
#include <unordered_map>

using namespace std;

struct EdgeLog
{
    int id;
    string column;
    string oldValue;
    string newValue;
    EdgeLog *next;
    EdgeLog *prev;
};

struct NodeLog
{
    string name;
    bool commited;
    EdgeLog *first;
    EdgeLog *last;
};

class GraphLog
{
    public:
        unordered_map<string, NodeLog*> listNodes;
        vector<string> operations;

        void getAtt();
        std::queue<string> readLog();
        void insertNode(string name);
        void insertEdge(string node, int id, string column, string oldValue, string newValue);
        void removeREDO(string node);
        void removeUNDO(string node);
        void printGraph();
        void freeGraph();
};

#endif