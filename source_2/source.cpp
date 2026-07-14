//подключаем макросы catch2
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <cstdint>
#include <stdexcept>

struct ListNode
{
public:
    ListNode(int value, ListNode* prev = nullptr, ListNode* next = nullptr)
        : value(value), prev(prev), next(next)
    {
        if (prev != nullptr) prev->next = this;
        if (next != nullptr) next->prev = this;
    }

public:
    int value;
    ListNode* prev;
    ListNode* next;
};

class List
{
public:
    List()
        : m_head(new ListNode(static_cast<int>(0))), m_size(0),
        m_tail(new ListNode(0, m_head))
    {
    }

    virtual ~List()
    {
        Clear();
        delete m_head;
        delete m_tail;
    }

    bool Empty() { return m_size == 0; }

    unsigned long Size() { return m_size; }

    void PushFront(int value)
    {
        new ListNode(value, m_head, m_head->next);
        ++m_size;
    }

    void PushBack(int value)
    {
        new ListNode(value, m_tail->prev, m_tail);
        ++m_size;
    }

    int PopFront()
    {
        if (Empty()) throw std::runtime_error("list is empty");
        auto node = extractPrev(m_head->next->next);
        int ret = node->value;
        delete node;
        return ret;
    }

    int PopBack()
    {
        if (Empty()) throw std::runtime_error("list is empty");
        auto node = extractPrev(m_tail);
        int ret = node->value;
        delete node;
        return ret;
    }

    void Clear()
    {
        auto current = m_head->next;
        while (current != m_tail)
        {
            current = current->next;
            delete extractPrev(current);
        }
    }

private:
    ListNode* extractPrev(ListNode* node)
    {
        auto target = node->prev;
        target->prev->next = target->next;
        target->next->prev = target->prev;
        --m_size;
        return target;
    }

private:
    ListNode* m_head;
    ListNode* m_tail;
    unsigned long m_size;
};

// ТЕСТЫ

TEST_CASE("Тестирование PushFront и PushBack") {

    SECTION("PushFront должен добавлять элементы в начало списка") {
        List list;

        list.PushFront(10);
        REQUIRE(list.Size() == 1);
        REQUIRE(list.Empty() == false);

        list.PushFront(20);
        REQUIRE(list.Size() == 2);

        list.PushFront(30);
        REQUIRE(list.Size() == 3);

        REQUIRE(list.PopFront() == 30);
        REQUIRE(list.PopFront() == 20);
        REQUIRE(list.PopFront() == 10);
        REQUIRE(list.Empty() == true);
    }

    SECTION("PushBack должен добавлять элементы в конец списка") {
        List list;

        list.PushBack(10);
        REQUIRE(list.Size() == 1);
        REQUIRE(list.Empty() == false);

        list.PushBack(20);
        REQUIRE(list.Size() == 2);

        list.PushBack(30);
        REQUIRE(list.Size() == 3);

        REQUIRE(list.PopFront() == 10);
        REQUIRE(list.PopFront() == 20);
        REQUIRE(list.PopFront() == 30);
        REQUIRE(list.Empty() == true);
    }

    SECTION("Комбинированное использование PushFront и PushBack") {
        List list;

        list.PushBack(10);
        list.PushFront(5);
        list.PushBack(20);
        list.PushFront(1);

        REQUIRE(list.Size() == 4);

        REQUIRE(list.PopFront() == 1);
        REQUIRE(list.PopFront() == 5);
        REQUIRE(list.PopFront() == 10);
        REQUIRE(list.PopFront() == 20);
        REQUIRE(list.Empty() == true);
    }
}

TEST_CASE("Тестирование PopFront и PopBack") {

    SECTION("PopFront должен удалять элемент из начала и возвращать его значение") {
        List list;

        list.PushBack(10);
        list.PushBack(20);
        list.PushBack(30);
        REQUIRE(list.Size() == 3);

        REQUIRE(list.PopFront() == 10);
        REQUIRE(list.Size() == 2);

        REQUIRE(list.PopFront() == 20);
        REQUIRE(list.Size() == 1);

        REQUIRE(list.PopFront() == 30);
        REQUIRE(list.Size() == 0);
        REQUIRE(list.Empty() == true);
    }

    SECTION("PopBack должен удалять элемент из конца и возвращать его значение") {
        List list;

        list.PushBack(10);
        list.PushBack(20);
        list.PushBack(30);
        REQUIRE(list.Size() == 3);

        REQUIRE(list.PopBack() == 30);
        REQUIRE(list.Size() == 2);

        REQUIRE(list.PopBack() == 20);
        REQUIRE(list.Size() == 1);

        REQUIRE(list.PopBack() == 10);
        REQUIRE(list.Size() == 0);
        REQUIRE(list.Empty() == true);
    }

    SECTION("PopFront на пустом списке должен выбрасывать исключение") {
        List list;
        REQUIRE(list.Empty() == true);

        REQUIRE_THROWS_AS(list.PopFront(), std::runtime_error);

        REQUIRE(list.Size() == 0);
        REQUIRE(list.Empty() == true);
    }

    SECTION("PopBack на пустом списке должен выбрасывать исключение") {
        List list;
        REQUIRE(list.Empty() == true);

        REQUIRE_THROWS_AS(list.PopBack(), std::runtime_error);

        REQUIRE(list.Size() == 0);
        REQUIRE(list.Empty() == true);
    }

    SECTION("Комбинированное использование PopFront и PopBack") {
        List list;

        list.PushBack(10);
        list.PushBack(20);
        list.PushBack(30);
        list.PushBack(40);
        list.PushBack(50);
        REQUIRE(list.Size() == 5);

        REQUIRE(list.PopFront() == 10);
        REQUIRE(list.PopBack() == 50);
        REQUIRE(list.PopFront() == 20);
        REQUIRE(list.PopBack() == 40);
        REQUIRE(list.Size() == 1);

        REQUIRE(list.PopFront() == 30);
        REQUIRE(list.Empty() == true);
    }
}

TEST_CASE("Сложный сценарий использования всех функций") {

    SECTION("Сложный сценарий: чередование добавлений и удалений") {
        List list;

        list.PushFront(1);
        list.PushBack(2);
        list.PushFront(0);
        list.PushBack(3);
        REQUIRE(list.Size() == 4);

        REQUIRE(list.PopFront() == 0);
        REQUIRE(list.PopBack() == 3);
        REQUIRE(list.Size() == 2);

        list.PushFront(-1);
        list.PushBack(4);
        REQUIRE(list.Size() == 4);

        REQUIRE(list.PopFront() == -1);
        REQUIRE(list.PopFront() == 1);
        REQUIRE(list.PopFront() == 2);
        REQUIRE(list.PopFront() == 4);
        REQUIRE(list.Empty() == true);
    }

    SECTION("Сложный сценарий: работа с большим количеством элементов") {
        List list;
        const int N = 100;

        for (int i = 0; i < N; ++i) {
            if (i % 2 == 0) {
                list.PushFront(i);
            }
            else {
                list.PushBack(i);
            }
        }
        REQUIRE(list.Size() == N);

        for (int i = 0; i < N / 2; ++i) {
            list.PopFront();
        }
        REQUIRE(list.Size() == N / 2);

        for (int i = 0; i < N / 4; ++i) {
            list.PopBack();
        }
        REQUIRE(list.Size() == N / 4);

        REQUIRE(list.Empty() == false);
        REQUIRE(list.Size() > 0);
    }

    SECTION("Сложный сценарий: Push и Pop в случайном порядке") {
        List list;

        list.PushBack(1);
        list.PushBack(2);
        list.PushFront(0);

        REQUIRE(list.PopFront() == 0);
        REQUIRE(list.PopBack() == 2);

        list.PushFront(-1);
        list.PushBack(3);

        REQUIRE(list.PopFront() == -1);
        REQUIRE(list.PopFront() == 1);
        REQUIRE(list.PopFront() == 3);
        REQUIRE(list.Empty() == true);

        list.PushBack(100);
        list.PushBack(200);
        REQUIRE(list.Size() == 2);
        REQUIRE(list.PopFront() == 100);
        REQUIRE(list.PopFront() == 200);
        REQUIRE(list.Empty() == true);
    }

    SECTION("Сложный сценарий: проверка корректности после удаления") {
        List list;

        for (int i = 1; i <= 5; ++i) {
            list.PushBack(i);
        }
        REQUIRE(list.Size() == 5);

        REQUIRE(list.PopFront() == 1);
        list.PushBack(6);
        REQUIRE(list.Size() == 5);

        REQUIRE(list.PopBack() == 6);
        list.PushFront(1);
        REQUIRE(list.Size() == 5);

        for (int i = 1; i <= 5; ++i) {
            REQUIRE(list.PopFront() == i);
        }
        REQUIRE(list.Empty() == true);
    }

    SECTION("Сложный сценарий: проверка исключений в сложном сценарии") {
        List list;

        list.PushBack(10);
        list.PushBack(20);
        list.PushBack(30);
        REQUIRE(list.Size() == 3);

        REQUIRE(list.PopFront() == 10);
        REQUIRE(list.PopFront() == 20);
        REQUIRE(list.PopFront() == 30);
        REQUIRE(list.Empty() == true);

        REQUIRE_THROWS_AS(list.PopFront(), std::runtime_error);
        REQUIRE_THROWS_AS(list.PopBack(), std::runtime_error);

        REQUIRE(list.Empty() == true);
        REQUIRE(list.Size() == 0);

        list.PushBack(100);
        REQUIRE(list.Size() == 1);
        REQUIRE(list.PopFront() == 100);
        REQUIRE(list.Empty() == true);
    }
}
