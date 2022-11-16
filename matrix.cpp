#include <cstddef>
#include <functional>
#include <iostream>
#include <iterator>
#include <utility>
#include <vector>

template <typename T>
class Matrix {
   public:
    using container_type = std::vector<T>;
    using size_type = std::size_t;
    Matrix(size_type rows, size_type cols)
        : data(cols * rows), rows_{rows}, cols_{cols} {}

    virtual ~Matrix() = default;

    T &operator()(size_type row, size_type col) {
        return data[row * cols_ + col];
    }

    const T &operator()(size_type row, size_type col) const {
        return data[row * cols_ + col];
    }

    size_type rows() const { return rows_; }
    size_type cols() const { return cols_; }

    void traverse(std::function<void(T &)> f) {
        for (auto &el : data) f(el);
    }

    void traverse(std::function<void(T &)> f, size_type row, size_type col) {
        for (auto &[rd, cd] : neighbours_delta) {
            size_type r = row + rd, c = col + cd;
            if (r >= 0 && r < rows_ && c >= 0 && c < cols_) f((*this)(r, c));
        }
    }

    auto begin() { return data.begin(); }
    auto end() { return data.end(); }

    auto cbegin() const { return data.cbegin(); }
    auto cend() const { return data.cend(); }

    auto neighbours(size_type row, size_type col) {
        return NeighbourIterator(row, col, *this);
    }

    friend std::ostream &operator<<(std::ostream &os, const Matrix &m) {
        for (size_type row = 0; row < m.rows_; ++row) {
            for (size_type col = 0; col < m.cols_; ++col) {
                os << m(row, col) << " ";
            }
            os << '\n';
        }
        return os;
    }

   protected:
    container_type data;
    size_type rows_;
    size_type cols_;

    static constexpr std::array<std::pair<int, int>, 8> neighbours_delta = {
        {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}};

    struct NeighbourIterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;

        NeighbourIterator(size_type row, size_type col, Matrix<T> &m,
                          int neighbours = 0)
            : m{m}, row{row}, col{col}, neighbours{neighbours} {}

        auto get_delta() const { return m.neighbours_delta[neighbours]; }

        bool exists() {
            auto [rd, cd] = get_delta();
            size_type r = row + rd, c = col + cd;
            return r >= 0 && r < m.rows_ && c >= 0 && c < m.cols_;
        }

        T &get_neighbor() const {
            auto [rd, cd] = get_delta();
            return m(row + rd, col + cd);
        }

        reference operator*() const { return get_neighbor(); }
        pointer operator->() { return get_neighbor(); }
        NeighbourIterator &operator++() {
            do {
                neighbours++;
            } while (neighbours < neighbours_delta.size() && !exists());

            return *this;
        }
        NeighbourIterator operator++(int) {
            NeighbourIterator tmp = *this;
            ++(*this);
            return tmp;
        }
        auto begin() { return NeighbourIterator(row, col, m, 0); }
        auto end() {
            return NeighbourIterator(row, col, m, neighbours_delta.size());
        }

        friend bool operator==(const NeighbourIterator &a,
                               const NeighbourIterator &b) {
            return a.neighbours == b.neighbours;
        };
        friend bool operator!=(const NeighbourIterator &a,
                               const NeighbourIterator &b) {
            return a.neighbours != b.neighbours;
        };

       private:
        Matrix<T> &m;
        size_type row, col;
        int neighbours;
    };
};

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
