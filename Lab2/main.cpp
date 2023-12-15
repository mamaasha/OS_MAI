#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>
#include <mutex>

// вставкой (длина менее 32)
void sort(std::vector<int>& vector, size_t start, size_t len) {
    for (size_t i = start + 1; i < len + start; i++) {
        for (size_t j = i; (j > start) && (vector[j - 1] > vector[j]); j--) {
            std::swap(vector[j - 1], vector[j]);
        }
    }
}

void merge(std::vector<int>& vec, size_t start1, size_t len1, size_t start2, size_t len2) {
    size_t end = start2 + len2;
    if (end > vec.size()) {
        end = vec.size();
    }
    std::inplace_merge(vec.begin() + start1, vec.begin() + start1 + len1, vec.begin() + start2 + len2); 
}


void sortSegment(std::vector<int>& vec, size_t start, size_t minrun, std::mutex& mtx) {
    sort(vec, start, std::min(minrun, vec.size() - start));
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Поток " << std::this_thread::get_id() << " отсортировал сегмент " << start << "-" << start + minrun - 1 << std::endl;
}

void mergeSegments(std::vector<int>& vec, size_t start1, size_t len1, size_t start2, size_t len2, std::mutex& mtx) {
    merge(vec, start1, len1, start2, len2);
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Поток " << std::this_thread::get_id() << " объединил сегменты " << start1 << "-" << start1 + len1 - 1 << " и " << start2 << "-" << start2 + len2 - 1 << std::endl;
}


void timSort(std::vector<int>& vec, size_t n, size_t maxThreads) {
    size_t minrun = std::max<size_t>(32, vec.size() / 64);
    std::cout << "minrun = " << minrun << std::endl;

    std::mutex mtx;
    std::vector<std::thread> threads;

    // Сортировка
    for (size_t i = 0; i < vec.size(); i += minrun) {
        
        threads.emplace_back(std::thread(sortSegment, std::ref(vec), i, minrun, std::ref(mtx)));

        if (threads.size() >= maxThreads) {
            for (auto& thread : threads) {
                thread.join();
            }
            threads.clear();
        }
    }

    for (auto& thread : threads) {
        thread.join();
    }

    // Слияние
    for (size_t size = minrun; size < vec.size(); size *= 2) {
        threads.clear();
        for (size_t start = 0; start < vec.size(); start += 2 * size) {
            size_t mid = start + size;
            size_t end = std::min(start + 2 * size, vec.size());
            threads.emplace_back(std::thread(mergeSegments, std::ref(vec), start, mid - start, mid, end - mid, std::ref(mtx)));

            if (threads.size() >= maxThreads) {
                for (auto& thread : threads) {
                    thread.join();
                }
                threads.clear();
            }
        }
        for (auto& thread : threads) {
            thread.join();
        }
    }
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Использование: " << argv[0] << " <максимальное количество потоков:>" << std::endl;
        return 1;
    }

    int maxThreads = std::stoi(argv[1]); // в целое
    std::cout << "Максимальное количество потоков: " << maxThreads << std::endl;

    // Пмакс кол-во потоков
    int hardwareThreads = std::thread::hardware_concurrency(); // число, которое поддерживается
    if (maxThreads <= 0 || maxThreads > hardwareThreads) {
        std::cout << "Недопустимое количество потоков" << std::endl;
        return 1;
    }

    std::vector<int> forsort;
    std::cout << "Введите вектор для сортировки:" << std::endl;

    int m;
    while (std::cin >> m) {
        forsort.push_back(m);
    }

    if (std::cin.fail()) {
        std::cin.clear();
        std::string invalidInput;
        std::cin >> invalidInput;
    }

    // std::chrono::time_point
    auto t1 = std::chrono::high_resolution_clock::now(); // текущая точка времени
    timSort(forsort, forsort.size(), maxThreads);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count(); // приведение к микросекундам
    std::cout << "Время: " << duration << " микросекунд" << std::endl;

    std::cout << "Отсортированный вектор:" << std::endl;
    for (const auto& num : forsort) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
