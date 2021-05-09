#ifndef MATRIX_GRAPH_H
#define MATRIX_GRAPH_H

#include <iostream>
#include <fstream>
#include <random>

class matrix_Graph
{
    int** m_array_matrix;               //Macierz sąsiedztwa
    int m_V;                            //Ilość wierzchołków grafu
    int m_E;                            //Ilość krawędzi w grafie
    int m_start_vertex;                 //Wierzchołek startowy dla algorytmu Bellmana-Forda
    int* m_array_of_costs;              //Tablica kosztów najkrótszych ścieżek od wierzchołka startowego do pozostałych
    int* m_array_of_previous;           //Tablica poprzedników, dzięki której można odtworzyć wszystkie ścieżki
    const int m_max_int = 2147483647;   //Stała symulująca wartość 'nieskończoność'
public:
    //Konstruktor dla grafu niewczytywanego z pliku
    matrix_Graph(int vertices, int start_vertex);

    //Konstruktor dla grafu wczytywanego z pliku
    matrix_Graph(std::ifstream& fin);

    //Destruktor
    ~matrix_Graph();

    //Dodawanie krawędzi
    void add_edge(int top_1, int top_2, int weight);

    //Dodawanie wierzchołka
    void add_vertex();

    //Algorytm Bellmana-Forda dla wybranego wierzchołka startowego
    bool bellman_ford_algorithm_matrix(int start_vertex);

    //Algorytm Bellmana-Forda dla wierzchołka domyślnego w polu m_start_vertex
    bool bellman_ford_algorithm_matrix() const;

    //Wypisanie najkrótszych ścieżek
    void show_shortest_paths() const;

    //Wyświetlenie list sąsiedztwa
    void show_adj_matrix_graph() const;

    //Przekierowanie ścieżek do pliku wynikowego
    void send_data_to_file(std::ofstream& fout) const;

    //Wygenerowanie danych grafu
    void data_generation(double density);

    //Zwraca liczbę wierzchołków w grafie
    int get_number_of_vertex(){return m_V;}

    //Sprawdza,czy występuje krawędź skierowana pomiędzy tymi wierzchołkami
    bool is_edge_between(int vert1, int vert2);
};

#endif

