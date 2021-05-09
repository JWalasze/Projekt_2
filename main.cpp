#include "inc/adjacency_list_Graph.h"
#include "inc/matrix_Graph.h"
#include <ctime>

int main()
{
    //Tworzymy obiekty do pobierania danych z pliku tekstowego i sprawdzamy, czy jest dobrze otwarty
    std::ifstream adj_fin, matrix_fin;

    adj_fin.open("C:/Users/Kuba Walaszek/Desktop/PAMSI/Projekt2/plik_dane.txt");
    if (!adj_fin)
    {
        std::cout << "Blad przy otwieraniu pliku" << std::endl;
        return -1;
    }

    matrix_fin.open("C:/Users/Kuba Walaszek/Desktop/PAMSI/Projekt2/plik_dane.txt");
    if (!matrix_fin)
    {
        std::cout << "Blad przy otwieraniu pliku" << std::endl;
        return -1;
    }

    //Tworzymy obiekt do wysyłania uzyskanych danych do pliku
    std::ofstream fout, f_to_report;
    fout.open("C:/Users/Kuba Walaszek/Desktop/PAMSI/Projekt2/plik_sciezki_i_wagi.txt");
    if (!fout)
    {
        std::cout << "Blad przy otwieraniu pliku" << std::endl;
        return -1;
    }

    f_to_report.open("C:/Users/Kuba Walaszek/Desktop/PAMSI/Projekt2/do_sprawozdania.txt");
    if (!f_to_report)
    {
        std::cout << "Blad przy otwieraniu pliku" << std::endl;
        return -1;
    }

    //Zmienna do wyboru opcji działania programu
    char c;
    std::cin >> c;

    //Reprezentacja za pomocą macierzy sąsiedztwa
    if (c == 'm')
    {
        //Generator do losowania wierzchołku startowego
        std::random_device generator;

        //Zmienna do odmierzania ilości poprawnych instancji
        int count = 0;
        while (count != 100)
        {
            //Tworzymy graf (macierz sąsiedztwa) i generujemy dane
            matrix_Graph Graph1(100,0);
            Graph1.data_generation(0.75);
            //Graph1.show_adj_matrix_graph();

            //Generujemy losowy startowy wierzchołek
            std::uniform_int_distribution<int> distribution_vertex(0,Graph1.get_number_of_vertex()-1);
            int random_start_vertex = distribution_vertex(generator);

            //Odmierzamy czas trwania algorytmu Bellmana-Forda
            clock_t start_time = clock();
            bool b = Graph1.bellman_ford_algorithm_matrix(random_start_vertex);
            clock_t  end_time = clock();

            if (b)
            {
                //Wysyłamy pomiary czasu do pliku "do_sprawozdania.txt"
                f_to_report << static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC << std::endl;
                ++count;
                std::cout << "Instancja " << count << std::endl;
            }
        }
    }

    //Reprezentacja za pomocą listy sąsiedztwa
    else if (c == 'a')
    {
        //Generator do losowania wierzchołka startowego
        std::random_device generator;

        //Zmienna do odmierzania ilości poprawnych instancji
        int count = 0;
        while (count != 100)
        {
            //Tworzymy graf (lista sąsiedztwa) i generujemy dane
            adjacency_list_Graph Graph2(100,0);
            Graph2.data_generation(1);
            //Graph2.show_adj_list_graph();

            //Losujemy wierzchołek startowy
            std::uniform_int_distribution<int> distribution_vertex(0,Graph2.get_number_of_vertex()-1);
            int random_start_vertex = distribution_vertex(generator);

            //Odmierzamy czas trwania algorytmu Bellmana-Forda
            clock_t start_time = clock();
            bool b = Graph2.bellman_ford_algorithm_adj_list(random_start_vertex);
            clock_t  end_time = clock();

            if (b)
            {
                f_to_report << static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC << std::endl;
                ++count;
                std::cout << "Instancja " << count << std::endl;
            }
        }
    }

    //Pojedyncze stworzenie grafu (lista) za pomocą pobrania danych z pliku, zapisu wyniku do pliku, wyświetlenie list sąsiedztwa oraz ścieżek
    else if (c == 'p')
    {
        //Tworzymy graf (lista sąsiedztwa) i generujemy dane
        adjacency_list_Graph Graph2(adj_fin);
        Graph2.show_adj_list_graph();

        //Odmierzamy czas trwania algorytmu Bellmana-Forda
        clock_t start_time = clock();
        bool b = Graph2.bellman_ford_algorithm_adj_list();
        clock_t  end_time = clock();

        if (b)
        {
            Graph2.show_shortest_paths();
            Graph2.send_data_to_file(fout);
            std::cout << "Czas trwania algorytmu: " << static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC << std::endl;
        }
        else
        {
            std::cout << "Wystepuje cykl ujemny!!!" << std::endl;
        }
    }

    //Pojedyncze stworzenie grafu (macierz) za pomocą pobrania danych z pliku, zapisu wyniku do pliku, wyświetlenie macierzy oraz ścieżek
    else if (c == 'k')
    {
        //Tworzymy graf (macierz sąsiedztwa) i generujemy dane
        matrix_Graph Graph1(adj_fin);
        Graph1.show_adj_matrix_graph();

        //Odmierzamy czas trwania algorytmu Bellmana-Forda
        clock_t start_time = clock();
        bool b = Graph1.bellman_ford_algorithm_matrix();
        clock_t  end_time = clock();

        if (b)
        {
            Graph1.show_shortest_paths();
            Graph1.send_data_to_file(fout);
            std::cout << "Czas trwania algorytmu: " << static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC << std::endl;
        }
        else
        {
            std::cout << "Wystepuje cykl ujemny!!!" << std::endl;
        }
    }
    else
    {
        std::cout << "Niepoprawny znak!!!" << std::endl;
    }
    adj_fin.close();
    matrix_fin.close();
    fout.close();
    f_to_report.close();
    return 0;
}
