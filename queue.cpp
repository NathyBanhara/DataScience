#include <iostream>
#include "queue.hpp"

using namespace std;

//Problemas: desconheço número de vértices (posso navegar primeiramente pelo documento json e contar quantos atributos com nomes diferentes eu tenho); fazer edge e node separado dá problema para percorer
//checar se nodo é válido ao colocar uma nova aresta, pensar em outros tipos de checagens necessárias

Queue::Queue()
{
    this->queue = NULL;
}

QueueStruct* Queue::createStruct(string name, string type)
{
    QueueStruct *new_one = new QueueStruct;
    new_one->name = name;
    new_one->type = type;
    new_one->next = NULL;
    return new_one;
}

void Queue::push(QueueStruct *new_one)
{
    if (this->queue == NULL)
    {
        this->queue = new QueueList;
        this->queue->first = new_one;
        this->queue->last = new_one;
    }
    else
    {
        this->queue->last->next = new_one;
        this->queue->last = new_one;
    }
}

void Queue::pop()
{
    if (this->queue == NULL || this->queue->first == NULL) return;
    QueueStruct *aux;
    aux = queue->first;
    if (this->queue->first == this->queue->last)
    {
        this->queue->first = NULL;
        this->queue->last = NULL;
    }
    else
    {
        this->queue->first = this->queue->first->next;
    }
    free(aux);
}

int Queue::empty()
{
    if (this->queue == NULL || this->queue->first == NULL) return 1;
    return 0;
}

QueueStruct* Queue::front()
{
    return this->queue->first;
}

int Queue::size()
{
    int i = 0;
    for (QueueStruct *aux = this->queue->first; aux != NULL; aux = aux->next)
        i++;
    return i;
}

void Queue::freeQueue()
{
    QueueStruct *aux = this->queue->first;
    while (aux != NULL) {
        this->queue->first = aux->next;
        free(aux);
        aux = this->queue->first;
    }
}

void Queue::printQueue()
{
    if (this->queue == NULL)
    {
        printf("The Queue is empty.\n");
        return;
    }
    for (QueueStruct *aux = this->queue->first; aux != NULL; aux = aux->next)
        cout << aux->name << "->" << aux->type << "\n";
}