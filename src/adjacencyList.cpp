#include "../inc/adjacencyList.h"

adjacencyList::adjacencyList()
{
    m_head = nullptr;
    m_tail = nullptr;
}

adjacencyList::~adjacencyList()
{
    adjacencyListNode* q;
    while (m_head != nullptr)
    {
        q = m_head->m_next;
        delete m_head;
        m_head = q;
    }
    m_tail = nullptr;
    delete q;
}

adjacencyList::adjacencyList(int item, int weight)
{
    auto* new_item = new adjacencyListNode;
    new_item->m_vertex = item;
    new_item->m_next = nullptr;
    new_item->m_weight = weight;
    m_head = new_item;
    m_tail = new_item;
}

void adjacencyList::push_front(int item, int weight)
{
    auto* new_item = new adjacencyListNode;
    new_item->m_vertex = item;
    new_item->m_next = m_head;
    new_item->m_weight = weight;
    if (m_head == nullptr)
    {
        m_tail = new_item;
    }
    m_head = new_item;
}

void adjacencyList::push_back(int item, int weight)
{
    auto* new_item = new adjacencyListNode;
    new_item->m_vertex = item;
    new_item->m_next = nullptr;
    new_item->m_weight = weight;
    if (m_head == nullptr)
    {
        m_head = new_item;
        m_tail = new_item;
    }
    else
    {
        m_tail->m_next = new_item;
        m_tail = new_item;
    }
}

void adjacencyList::pop_front()
{
    if (this->is_empty())
    {
        std::cout << "Lista jest pusta" << std::endl;
    }
    else
    {
        adjacencyListNode* old_head = m_head;
        m_head = old_head->m_next;
        delete old_head;
    }
}

bool adjacencyList::is_empty()
{
    if (m_head == nullptr)
    {
        return true;
    }
    return false;
}

void adjacencyList::show_items()
{
    if (m_head == nullptr)
    {
        std::cout << " Lista jest pusta, wierzcholek nie ma krawedzi!" << std::endl;
    }
    else
    {
        auto* current = new adjacencyListNode;
        current = m_head;
        std::cout << " head";
        while (current != nullptr)
        {
            std::cout << " -> " << current->m_vertex;
            current = current->m_next;
        }
        std::cout << std::endl;
        delete current;
    }
}

adjacencyListNode* adjacencyList::get_head()
{
    return m_head;
}

adjacencyListNode* adjacencyList::get_tail()
{
    return m_tail;
}

void adjacencyList::set_head(adjacencyListNode* elem)
{
    m_head = elem;
}

void adjacencyList::set_tail(adjacencyListNode* elem)
{
    m_tail = elem;
}
