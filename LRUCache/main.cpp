#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <cassert>
#include "LRUCache.h"

// Тест 1: Проверка базовой логики LRU (вытеснение самого старого)
void test_lru_logic() {
    std::cout << "Running LRU Logic Test..." << std::endl;

    // Кэш на 2 элемента
    LRUCache<int, std::string> cache(2);

    cache.put(1, "A");
    cache.put(2, "B");

    // Сейчас порядок: [2:B, 1:A]
    // Обращаемся к 1, теперь 1 — "свежий", 2 — "старый"
    std::string val;
    cache.get(1, val);

    // Добавляем 3, должен вытесниться ключ 2
    cache.put(3, "C");

    assert(cache.get(1, val) == true);  // 1 на месте
    assert(cache.get(3, val) == true);  // 3 на месте
    assert(cache.get(2, val) == false); // 2 удален

    std::cout << "LRU Logic Test: PASSED" << std::endl;
}

// Тест 2: Проверка потокобезопасности (одновременная запись и чтение)
void test_multithreading() {
    std::cout << "Running Multithreading Test..." << std::endl;

    LRUCache<int, int> cache(100);
    const int num_threads = 10;
    const int ops_per_thread = 500;

    auto worker = [&](int id) {
        for (int i = 0; i < ops_per_thread; ++i) {
            cache.put(i, i + id);
            int val;
            cache.get(i, val);
        }
        };

    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(worker, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    // Если мы здесь и программа не "упала" — мьютекс отработал верно
    std::cout << "Multithreading Test: PASSED" << std::endl;
}

// Тест 3: Проверка ограничений размера из ТЗ
void test_capacity_limits() {
    std::cout << "Running Capacity Limits Test..." << std::endl;

    LRUCache<int, int> cache(15000); // По ТЗ ограничение 10000
    // Мы не можем проверить приватное поле напрямую, но можем проверить поведение
    for (int i = 0; i < 11000; ++i) {
        cache.put(i, i);
    }

    // Если лимит в 10000 сработал, то 0-й элемент уже должен быть вытеснен
    assert(cache.size() <= 10000);
    std::cout << "Capacity Limits Test: PASSED" << std::endl;
}

void test_remove_and_clear() {
    std::cout << "Running Remove and Clear Test..." << std::endl;

    LRUCache<int, std::string> cache(10);
    cache.put(1, "data");

    assert(cache.size() == 1);
    assert(cache.remove(1) == true);
    assert(cache.size() == 0);

    cache.put(2, "new_data");
    cache.clear();
    assert(cache.size() == 0);

    std::cout << "Remove and Clear Test Passed!\n" << std::endl;
}

int main() {
    try {
        test_lru_logic();
        std::cout << "-----------------------" << std::endl;
        test_multithreading();
        std::cout << "-----------------------" << std::endl;
        test_capacity_limits();
        std::cout << "-----------------------" << std::endl;
        test_remove_and_clear();
        std::cout << "\nALL TESTS PASSED SUCCESSFULLY!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Test failed with error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}