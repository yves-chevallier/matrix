#include <algorithm>
#include <iostream>

#include "matrix.hpp"

int main() {
    Matrix<int> m(10, 10);

    // Can use STL algorithms
    std::fill(m.begin(), m.end(), 0);

    // Can use range-based for
    for (auto &el : m) el += 1;

    // Can use lambdas
    int sum = 0;
    m.traverse([&sum](int &el) { sum += el; });

    // Can use lambdas over neighbours
    m.traverse([](int &el) { el = 0; }, 5, 5);

    // Can use range-based for over neighbours
    for (auto &el : m.neighbours(8, 8)) el = 8;

    // For-each
    auto it = m.neighbours(0, 2);
    std::for_each(it.begin(), it.end(), [](int &el) { el = 4; });

    // Can display on cout
    std::cout << m << std::endl;
}
