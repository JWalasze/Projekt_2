#ifndef ADJACENCYLIST_H
#define ADJACENCYLIST_H
#include <iostream>

struct adjacencyListNode
{
    int m_vertex;                 //Wierzchołek
    int m_weight;                 //Waga krawędzi
    adjacencyListNode* m_next;    //Wskaźnik na następny element listy sąsiedztwa
};

class adjacencyList
{
    adjacencyListNode* m_head;    //Wskaźnik na pierwszy element (głowę) listy
    adjacencyListNode* m_tail;    //Wskaźnik na ostatni element listy (ogon)
public:
    adjacencyList();                            //Konstruktor bezparametrowy
    ~adjacencyList();                            //Destruktor
    adjacencyList(int item, int weight);            //Konstruktor
    void push_front(int item, int weight);          //Dodawanie elementu na początek listy
    void push_back(int item, int weight);           //Dodawanie elementu na koniec listy
    void pop_front();                           //Usunięcie pierwszego elementu
    bool is_empty();                            //Sprawdza, czy lista jest pusta
    void show_items();                          //Wypisuje elementy pojedynczej listy
    adjacencyListNode* get_head();
    adjacencyListNode* get_tail();
    void set_head(adjacencyListNode* elem);
    void set_tail(adjacencyListNode* elem);
};

#endif


