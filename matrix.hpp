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
        : data(cols * rows), rows_{rows}, cols_{cols};

    virtual ~Matrix() = default;

    T &operator()(size_type row, size_type col);
    const T &operator()(size_type row, size_type col) const;

    size_type rows() const;
    size_type cols() const;

    void traverse(std::function<void(T &)> f);
    void traverse(std::function<void(T &)> f, size_type row, size_type col);

    auto begin();
    auto end();

    auto cbegin() const;
    auto cend() const;

    auto neighbours(size_type row, size_type col);

    friend std::ostream &operator<<(std::ostream &os, const Matrix &m);

   protected:
    container_type data;
    size_type rows_;
    size_type cols_;

    static constexpr std::array<std::pair<int, int>, 8> neighbours_delta = {
        {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}};

    struct NeighbourIterator;
};
