#include "../inc/adjacency_list_Graph.h"

adjacency_list_Graph::adjacency_list_Graph(int vertices, int start_vertex)
{
    //Inicjujemy wartościami ilość wierzchołków oraz startowy wierzchołek dla algorytmu Bellmana-Forda
    m_V = vertices;
    m_E = 0;
    m_start_vertex = start_vertex;

    //Tworzymy tablicę list sąsiedztwa i przypisujemy ją do pola grafu
    auto** array = new adjacencyList* [m_V];
    for (int i = 0; i < m_V; ++i)
    {
        array[i] = new adjacencyList();
    }
    m_array_list = array;

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

adjacency_list_Graph::adjacency_list_Graph(std::ifstream &fin)
{
    int number_of_vertices, number_of_edges, start_vertex;
    fin >> number_of_vertices >> number_of_edges >> start_vertex;
    //std::cout << number_of_vertices << "\t" << number_of_edges << "\t" << start_vertex;
    m_V = number_of_vertices;
    m_E = 0;
    m_start_vertex = start_vertex;

    //Tworzymy listy sąsiedztwa
    auto** array = new adjacencyList* [m_V];
    for (int i = 0; i < m_V; ++i)
    {
        array[i] = new adjacencyList();
    }
    m_array_list = array;

    //Pobieramy krawędzie z pliku
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

adjacency_list_Graph::~adjacency_list_Graph()
{
    //usuwamy dynamicznie alokowane struktury
    adjacencyList* lista;
    for (int i = 0; i < m_V; ++i)
    {
        lista = m_array_list[i];
        adjacencyListNode *q;
        while (lista->get_head() != nullptr)
        {
            q = lista->get_head()->m_next;
            delete lista->get_head();
            lista->set_head(q);
        }
            lista->set_tail(nullptr);
    }
    delete [] m_array_list;
    delete [] m_array_of_costs;
    delete [] m_array_of_previous;
}

void adjacency_list_Graph::add_vertex()
{
    //Tworzymy nową tablicę list sąsiedztwa, aby zmieściła dodatkowy wierzchołek
    auto** array = new adjacencyList* [m_V+1];

    //Dane z obecnych list sąsiedztwa przypisujemy do nowej tablicy list
    for (int i = 0; i < m_V; ++i)
    {
        array[i] = new adjacencyList();
        *array[i] = *m_array_list[i];
    }

    //Alokujemy pamięć na ostatni element tablicy list sąsiedztwa
    array[m_V] = new adjacencyList();

    //Usuwamy wcześniejszą listę sąsiedztwa
    delete [] m_array_list;

    //Przypisujemy nową tablicę list do odpowiedniego pola grafu i zwiększamy ilość wierzchołków grafu
    m_array_list = array;
    ++m_V;
}

void adjacency_list_Graph::add_edge(int top_1, int top_2, int weight)
{
    //Sprawdzamy, czy podane indeksy wierzchołków istnieją
    if (top_1 >= m_V || top_2 >= m_V || top_2 < 0 || top_1 < 0)
    {
        std::cout << std::endl << "Nie ma takiego indeksu wierzcholka: " << top_1 << " lub " << top_2 << std::endl << std::endl;
    }

    //Dodaje element do odpowiedniej listy sąsiedztwa
    else
    {
        m_array_list[top_1]->push_back(top_2, weight);
    }

    //Zwiększamy ilość krawędzi grafu
    ++m_E;
}

void adjacency_list_Graph::show_adj_list_graph()
{
    //Wypisujemy listy sąsiedztw każdego wierzchołka
    for (int i = 0; i < m_V; ++i)
    {
        std::cout << "Wierzcholek " << i << ":";
        m_array_list[i]->show_items();
        std::cout << std::endl;
    }
}

bool adjacency_list_Graph::bellman_ford_algorithm_adj_list(int start_vertex)
{
    m_start_vertex = start_vertex;
    if (this->bellman_ford_algorithm_adj_list())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool adjacency_list_Graph::bellman_ford_algorithm_adj_list()
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

    //Pierwsza pętla wyznacza ile razy relaksujemy cały graf
    for (int i = 0; i <= m_V-1; ++i)
    {
        //Druga pętla przechodzi przez każdy element tablicy list sąsiedztwa
        for (int j = 0; j <= m_V-1; ++j)
        {
            //Tworzymy wskaźnik na element wskazujący przez "głowę", aby przejść przez całą listę sąsiedztwa
            adjacencyListNode* temp_head;
            temp_head = m_array_list[j]->get_head();

            //Pętla trwa dopóki lista ma elementy
            while (temp_head != nullptr)
            {
                //Relaksujemy krawędzie
                if (m_array_of_costs[j] != m_max_int && m_array_of_costs[j] + temp_head->m_weight < m_array_of_costs[temp_head->m_vertex])
                {
                    m_array_of_costs[temp_head->m_vertex] = m_array_of_costs[j] + temp_head->m_weight;
                    m_array_of_previous[temp_head->m_vertex] = j;
                }

                //Wskazujemy na kolejny element (wierzchołek) listy sąsiedztwa
                temp_head = temp_head->m_next;
            }
        }
    }

    //Sprawdzamy, czy występują cykle ujemne
    for (int j = 0; j <= m_V-1; ++j)
    {
        adjacencyListNode* temp_head;
        temp_head = m_array_list[j]->get_head();

        while (temp_head != nullptr)
        {
            if (m_array_of_costs[j] != m_max_int && m_array_of_costs[j] + temp_head->m_weight < m_array_of_costs[temp_head->m_vertex])
            {
                return false;
            }
            temp_head = temp_head->m_next;
        }
    }
    return true;
}

void adjacency_list_Graph::show_shortest_paths() const
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

            //Odczytujemy ścieżkę i zapisujemy ją, lecz w odwrotnej kolejności
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

void adjacency_list_Graph::send_data_to_file(std::ofstream& fout)
{
    //Funkcja jest analogiczna do funkcji 'show_shortest_paths'
    fout << std::endl << std::endl;
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

void adjacency_list_Graph::data_generation(double density)
{
    //Maksymalna ilość krawędzi pomiędzy wszystkimi wierzchołkami
    int max_edges = m_V*(m_V-1)/2;
    //std::cout << max_edges;

    //Tworzymy generatory licz pseudolosowych do losowania wag i indeksów wierzchołków, które będą tworzyć krawędzie
    std::random_device generator;
    std::uniform_int_distribution<int> distribution_vertex(0,m_V-1);
    std::uniform_int_distribution<int> distribution_positive_weight(1,100);
    std::uniform_int_distribution<int> distribution_negative_weight(-20,-1);
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

bool adjacency_list_Graph::is_edge_between(int vert1, int vert2)
{
    auto* current = new adjacencyListNode;
    current = m_array_list[vert1]->get_head();
    while (current != nullptr)
    {
        if (current->m_vertex == vert2)
        {
            return true;
        }
        current = current->m_next;
    }
    delete current;
    return  false;
}
