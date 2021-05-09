#include "../inc/matrix_Graph.h"

matrix_Graph::matrix_Graph(int vertices, int start_vertex)
{
    //Inicjujemy wartościami ilość wierzchołków oraz startowy wierzchołek dla algorytmu Bellmana-Forda
    m_V = vertices;
    m_E = 0;
    m_start_vertex = start_vertex;

    //Tworzymy macierz sąsiedztwa
    auto** matrix = new int* [m_V];
    for (int i = 0; i < m_V; ++i)
    {
        matrix[i] = new int [m_V];
        for (int j = 0; j < m_V; ++j)
        {
            matrix[i][j] = 0;
        }
    }
    m_array_matrix = matrix;

    //Tworzymy tablicę kosztów dojścia i inicjalizujemy ją na wartości "nieskończoność"
    auto* array_of_costs = new int [m_V];
    for (int i = 0; i < m_V; ++i)
    {
        array_of_costs[i] = m_max_int;
    }

    //Tworzymy tablicę poprzedników i inicjalizujemy ją na wartości -1
    auto* array_of_previous_vertex = new int [m_V];
    for (int i = 0; i < m_V; ++i)
    {
        array_of_previous_vertex[i] = -1;
    }

    //Wybieramy element startowy
    array_of_costs[start_vertex] = 0;
    m_array_of_costs = array_of_costs;
    m_array_of_previous = array_of_previous_vertex;
}

matrix_Graph::matrix_Graph(std::ifstream &fin)
{
    int number_of_vertices, number_of_edges, start_vertex;
    fin >> number_of_vertices >> number_of_edges >> start_vertex;
    //std::cout << number_of_vertices << "\t" << number_of_edges << "\t" << start_vertex;
    m_V = number_of_vertices;
    m_E = 0;
    m_start_vertex = start_vertex;

    //Tworzymy macierz sąsiedztwa
    auto** matrix = new int* [m_V];
    for (int i = 0; i < m_V; ++i)
    {
        matrix[i] = new int [m_V];
        for (int j = 0; j < m_V; ++j)
        {
            matrix[i][j] = 0;
        }
    }
    m_array_matrix = matrix;

    //Pobieramy krawędzie dopóki są dane w pliku
    while (!fin.eof())
    {
        int v_1, v_2, w;
        fin >> v_1 >> v_2 >> w;
        this->add_edge(v_1, v_2, w);
    }
    if (m_E != number_of_edges)
    {
        std::cout << "Niepoprawne wczytanie wierzcholkow" << std::endl;
    }

    //Tablica kosztów dojścia
    auto* array_of_costs = new int [m_V];
    for (int i = 0; i < m_V; ++i)
    {
        array_of_costs[i] = m_max_int;
    }

    //Tablica poprzedników
    auto* array_of_previous_vertex = new int [m_V];
    for (int i = 0; i < m_V; ++i)
    {
        array_of_previous_vertex[i] = -1;
    }

    //Wybieramy element startowy
    array_of_costs[start_vertex] = 0;
    m_array_of_costs = array_of_costs;
    m_array_of_previous = array_of_previous_vertex;
}

matrix_Graph::~matrix_Graph()
{
    for (int i = 0; i < m_V; ++i)
    {
        delete [] m_array_matrix[i];
    }
    delete [] m_array_matrix;
    delete [] m_array_of_previous;
    delete [] m_array_of_costs;
}

void matrix_Graph::add_vertex()
{
    //Tworzymy powiększoną macierz sąsiedztwa
    auto** matrix = new int* [m_V+1];

    //Dane z poprzedniej macierzy sąsiedztwa przypisujemy do nowej tablicy
    for (int i = 0; i < m_V; ++i)
    {
        matrix[i] = new int [m_V+1];
        for (int j = 0; j < m_V; ++j)
        {
            matrix[i][j] = m_array_matrix[i][j];
        }
    }

    //Alokujemy pamięć na ostatni wiersz tablicy macierzy sąsiedztwa
    matrix[m_V] = new int [m_V+1];

    //Wypełniamy puste elementy tablicy zerami
    for (int i = 0; i < m_V+1; ++i)
    {
        matrix[m_V][i] = 0;
        matrix[i][m_V] = 0;
    }

    //Usuwamy wcześniejszą macierz sąsiedztwa
    for (int i = 0; i < m_V; ++i)
    {
        delete [] m_array_matrix[i];
    }
    delete [] m_array_matrix;

    //Przypisujemy nową tablicę list do odpowiedniego pola grafu i zwiększamy ilość wierzchołków grafu
    m_array_matrix = matrix;
    ++m_V;
}

void matrix_Graph::add_edge(int top_1, int top_2, int weight)
{
    //Sprawdzamy, czy podane indeksy wierzchołków istnieją
    if (top_1 >= m_V || top_2 >= m_V || top_2 < 0 || top_1 < 0)
    {
        std::cout << std::endl << "Nie ma takiego indeksu wierzcholka: " << top_1 << " lub " << top_2 << std::endl << std::endl;
    }

    //Dodaje element do odpowiedniej listy sąsiedztwa
    else
    {
        m_array_matrix[top_1][top_2] = weight;
    }

    //Zwiększamy ilość krawędzi grafu
    ++m_E;
}

void matrix_Graph::show_adj_matrix_graph() const
{
    //Wypisujemy indeksy każdej kolumny
    for (int i = 0; i < m_V; ++i)
    {
        std::cout << "\t" << i << ".";
    }
    std::cout << std::endl;

    //Wypisujemy indeksy wierszy wraz z elementami macierzy sąsiedztwa
    for (int i = 0; i < m_V; ++i)
    {
        std::cout << i << "." << "\t";
        for (int j = 0; j < m_V; ++j)
        {
            std::cout << m_array_matrix[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool matrix_Graph::bellman_ford_algorithm_matrix(int start_vertex)
{
    m_start_vertex = start_vertex;
    if (this->bellman_ford_algorithm_matrix())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool matrix_Graph::bellman_ford_algorithm_matrix() const
{
    //Każdy element tablicy na początku jest równy "nieskończoność"
    for (int i = 0; i < m_V; ++i)
    {
        m_array_of_costs[i] = m_max_int;
    }

    //Każdy element tablicy na początku jest równy "nieskończoność"
    for (int i = 0; i < m_V; ++i)
    {
        m_array_of_previous[i] = -1;
    }

    //Wybieramy element startowy
    m_array_of_costs[m_start_vertex] = 0;

    //Pierwsza pętla wyznacza ile razy relaksujemy cały graf. Relaksujemy krawędzie wierzchołek po wierzchołku
    for (int i = 0; i <= m_V-1; ++i)
    {
        //Druga pętla przechodzi przez wiersze macierzy sąsiedztwa
        for (int j = 0; j <= m_V-1; ++j)
        {
            //Trzecia pętla przechodzi przez kolumny macierzy sąsiedztwa
            for (int k = 0; k <= m_V-1; ++k)
            {
                //Musi istnieć krawędź, dlatego nie może być równe 0 w macierzy - indeks k będzie zawsze wyznaczać wierzchołek końcowy krawędzi
                if (m_array_of_costs[j] != m_max_int && m_array_matrix[j][k] != 0 && m_array_of_costs[j] + m_array_matrix[j][k] < m_array_of_costs[k])
                {
                    m_array_of_costs[k] = m_array_of_costs[j] + m_array_matrix[j][k];
                    m_array_of_previous[k] = j;
                }
            }
        }
    }

    //Wykonujemy cykl relaksowania krawędzi, aby zobaczyć czy istnieje ujemny cykl
    for (int j = 0; j <= m_V-1; ++j)
    {
        for (int k = 0; k <= m_V-1; ++k)
        {
            //Wykonujemy relaksowanie tak jak wcześniej
            if (m_array_of_costs[j] != m_max_int && m_array_matrix[j][k] != 0 && m_array_of_costs[j] + m_array_matrix[j][k] < m_array_of_costs[k])
            {
                //std::cout << "Wystepuje cykl ujemny!!!" << std::endl;
                return false;
            }
        }
    }
    return true;
}

void matrix_Graph::show_shortest_paths() const
{
    //Przechodzimy przez wszystkie wierzchołki i wypisujemy ich ścieżki do wierzchołka startowego
    for (int i = 0; i < m_V; ++i)
    {
        //Sprawdzamy, czy jest to wierzchołek początkowy/startowy
        if (i == m_start_vertex)
        {
            std::cout << i << " jest wierzcholekiem poczatkowym" << std::endl << std::endl;
        }

        //Sprawdzamy, czy istnieje ścieżka do wierzchołka i od wierzchołka startowego
        else if (m_array_of_previous[i] == -1)
        {
            std::cout << "Wierzcholek docelowy: " << i << ". Nie ma polaczenia z tym wierzcholkiem!!!" << std::endl << std::endl;
        }
        else
        {
            //Tworzymy tymczasową tablicę, która będzie przechowywać ścieżkę
            auto* temp_array = new int [m_V];
            for (int j = 0; j < m_V; ++j)
            {
                temp_array[j] = -1;
            }
            int v = i;
            int c = 0;

            //Odczytujemy ścieżkę, lecz w odwrotnej kolejności
            while (m_array_of_previous[v] != m_start_vertex)
            {
                temp_array[c] = v;
                v = m_array_of_previous[v];
                ++c;
            }
            temp_array[c] = v;
            std::cout << "Koszt drogi z wierzcholka " << m_start_vertex << " do " << i << " wynosi " << m_array_of_costs[i] << ". ";
            std::cout << "Wierzcholek docelowy: " << i << ".  " << m_start_vertex;

            //Wypisujemy ścieżkę w odpowiedniej kolejności
            for (int j = c; j >= 0; --j)
            {
                std::cout << "  ->  " << temp_array[j];

            }
            std::cout << std::endl << std::endl;
            delete [] temp_array;
        }
    }
}

void matrix_Graph::send_data_to_file(std::ofstream& fout) const
{
    //Funkcja jest analogiczna do funkcji 'show_shortest_paths'
    for (int i = 0; i < m_V; ++i)
    {
        if (i == m_start_vertex)
        {
            fout << i << " jest wierzcholekiem poczatkowym" << std::endl << std::endl;
        }
        else if (m_array_of_previous[i] == -1)
        {
            fout << "Wierzcholek docelowy: " << i << ". Nie ma polaczenia z tym wierzcholkiem!!!" << std::endl << std::endl;
        }
        else
        {
            auto* temp_array = new int [m_V];
            for (int j = 0; j < m_V; ++j)
            {
                temp_array[j] = -1;
            }
            int v = i;
            int c = 0;
            while (m_array_of_previous[v] != m_start_vertex)
            {
                temp_array[c] = v;
                v = m_array_of_previous[v];
                ++c;
            }
            temp_array[c] = v;
            fout << "Koszt drogi z wierzcholka " << m_start_vertex << " do " << i << " wynosi " << m_array_of_costs[i] << ". ";
            fout << "Wierzcholek docelowy: " << i << ".  " << m_start_vertex;
            for (int j = c; j >= 0; --j)
            {
                fout << "  ->  " << temp_array[j];

            }
            fout << std::endl << std::endl;
            delete [] temp_array;
        }
    }
}

void matrix_Graph::data_generation(double density)
{
    //Maksymalna ilość krawędzi pomiędzy wszystkimi wierzchołkami
    int max_edges = m_V*(m_V-1)/2;

    //Tworzymy generatory licz pseudolosowych do losowania wag i indeksów wierzchołków, które będą tworzyć krawędzie
    std::random_device generator;
    std::uniform_int_distribution<int> distribution_vertex(0,m_V-1);
    std::uniform_int_distribution<int> distribution_positive_weight(1,100);
    std::uniform_int_distribution<int> distribution_negative_weight(-50,-1);
    //std::cout << std::endl << distribution_vertex(generator) << std::endl << distribution_weight(generator) << std::endl;

    //Zmienna odliczająca ile krawędzi zostało utworzonych
    int edge_count = 0;
    while (edge_count != static_cast<int>(density*max_edges))
    {
        //Generujemy losowe wartości wag oraz losowe wierzchołki pomiędzy którymi będzie krawędź
        int vert_1 = distribution_vertex(generator);
        int vert_2 = distribution_vertex(generator);
        int weight = distribution_positive_weight(generator);
        int n_weight = distribution_negative_weight(generator);

        //Sprawdzamy, czy nie wylosowano krawędzi na wierzchołek z którego wychodzi
        while (vert_1 == vert_2 || this->is_edge_between(vert_1, vert_2))
        {
            vert_1 = distribution_vertex(generator);
            vert_2 = distribution_vertex(generator);
        }

        //Kontrolujemy ilość występujących wag ujemnych
        if (edge_count % 1000 == 0)
        {
            this->add_edge(vert_1, vert_2, n_weight);
            ++edge_count;

        }

        //W większości przypadków losujemy wagi dodatnie
        else
        {
            this->add_edge(vert_1, vert_2, weight);
            ++edge_count;
        }
    }
}

bool matrix_Graph::is_edge_between(int vert1, int vert2)
{
    if (m_array_matrix[vert1][vert2] == 0)
    {
        return false;
    }
    return true;
}

