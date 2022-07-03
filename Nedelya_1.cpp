#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <array>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Задание 1
struct Point {double x; double y;};

//Вспомогательные функции
std::ostream& operator<<(std::ostream& os, const Point& point) {
    std::cout << "{" << point.x << ", " << point.y << "}";
    return os;
};

std::ostream& operator<<(std::ostream& os, const std::vector<Point>& points) {
    std::cout << "{";
    if (!points.empty()) {
        for (int i = 0; i < points.size() - 1; ++i)
            std::cout << points[i] << ", ";
        std::cout << points[points.size() - 1];
    }
    std::cout << "}" << std::endl;
    return os;
}

//Требуемая функция
std::vector<Point> terribleTrigonometry(unsigned int n) {
    std::vector<Point> result;
    result.resize(n);
    if (n != 0)
        for (int i = 0; i < n; ++i) {
            double tempx, tempy;
            if (4 * i == n || 4 * i == 3 * n)
                tempx = 0;
            else
                tempx = cos(i * 2 * M_PI / n);
            if (i == 0 || 2 * i == n)
                tempy = 0;
            else
                tempy = sin(i * 2 * M_PI / n);
            Point temp{tempx, tempy};
            result[i] = temp;
        }
    return result;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Задание 2
template<unsigned int N>
constexpr unsigned int factorial() {
    unsigned int result = 1;
    if (N > 1) {
        for (int i = 2; i < N + 1; ++i)
            result *= i;
    }
    else
        result = 1;
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Задание 3
template<typename T, int N>
class geomVector {
private:
    std::array<T, N> data;
public:
    geomVector() = default;
    geomVector(std::initializer_list<T> list) {
        if (list.size() != N) {
            std::cout << "Error! Vector's size is different from list's." << std::endl;
            exit(1);
        }
        else
            for (int i = 0; i < N; ++i)
                data[i] = *(list.begin() + i);
    }

    T& operator[](size_t index) {
        if (index >=0 && index < N)
            return data[index];
        else {
            std::cout << "Error! Wrong index given." << std::endl;
            exit(1);
        }
    }
    std::array<T, N> getData() const {
        return data;
    }

    bool operator==(const geomVector<T, N>& right) const {
        for (int i = 0; i < N; ++i)
            if (data[i] != right[i])
                return false;
        return true;
    }

    geomVector<T, N> operator+(const geomVector<T, N>& right) const {
        geomVector<T, N> result;
        for (int i = 0; i < N; ++i)
            result[i] = this->data[i] + right.getData()[i];
        return result;
    }
    geomVector<T, N> operator-(const geomVector<T, N>& right) const {
        geomVector<T, N> result;
        for (int i = 0; i < N; ++i)
            result[i] = this->data[i] - right[i];
        return result;
    }
    friend geomVector<T, N>& operator-(geomVector<T, N>& vect) {
        for (int i = 0; i < N; ++i)
            vect[i] = -vect[i];
        return vect;
    }
    friend geomVector<T, N>& operator+(geomVector<T, N>& vect) {
        return vect;
    }

    geomVector<T, N>& operator*(T elem) {
        for (int i = 0; i < N; ++i)
            data[i] *= elem;
        return *this;
    }
    friend geomVector<T, N>& operator*(T number, geomVector<T, N>& vect) {
        for (int i = 0; i < N; ++i)
            vect[i] *= number;
        return vect;
    }

    friend  std::ostream& operator<<(std::ostream& os, const geomVector<T, N>& vect) {
        for (int i = 0; i < N; ++i)
            std::cout << vect.getData()[i] << " ";
        return os;
    }
    friend  std::ostream& operator<<(std::ostream& os, const geomVector<T, N>&& vect) {
        for (int i = 0; i < N; ++i)
            std::cout << vect.getData()[i] << " ";
        return os;
    }

    geomVector<T, N>& operator=(const geomVector<T, N>& right) {
        for (int i = 0; i < N; ++i)
            data[i] = right.getData()[i];
        return *this;
    }

    ~geomVector() = default;
};

template<typename T>
class geomVector<T, -1> {
private:
    std::vector<T> data;
public:
    geomVector() = default;
    geomVector(std::initializer_list<T> list) {
        data.resize(list.size());
        for (int i = 0; i < list.size(); ++i)
            data[i] = *(list.begin() + i);
    }

    T& operator[](size_t index) {
        if (index >=0 && index < data.size())
            return data[index];
        else {
            std::cout << "Error! Wrong index given." << std::endl;
            exit(1);
        }
    }
    void resize(unsigned int n) {
        data.resize(n);
    }

    size_t getSize() const {
        return data.size();
    }
    std::vector<T> getData() const {
        return data;
    }

    bool operator==(const geomVector<T, -1>& right) const {
        for (int i = 0; i < data.size(); ++i)
            if (data[i] != right[i])
                return false;
        return true;
    }

    geomVector<T, -1> operator+(const geomVector<T, -1>& right) const {
        if (this->getSize() == right.getSize()) {
            geomVector<T, -1> result;
            result.resize(right.getSize());
            for (int i = 0; i < right.getSize(); ++i)
                result[i] = this->data[i] + right.getData()[i];
            return result;
        }
        else {
            std::cout << "Error! Can't sum vectors from different dimensions." << std::endl;
            exit(1);
        }
    }
    geomVector<T, -1> operator-(const geomVector<T, -1>& right) const {
        if (this->getSize() == right.getSize()) {
            geomVector<T, -1> result;
            for (int i = 0; i < right.getSize(); ++i)
                result[i] = this->data[i] - right.getData()[i];
            return result;
        }
        else {
            std::cout << "Error! Can't sum vectors from different dimensions." << std::endl;
            exit(1);
        }
    }
    friend geomVector<T, -1>& operator-(geomVector<T, -1>& vect) {
        for (int i = 0; i < vect.getSize(); ++i)
            vect[i] = -vect[i];
        return vect;
    }
    friend geomVector<T, -1>& operator+(geomVector<T, -1>& vect) {
        return vect;
    }

    geomVector<T, -1>& operator*(T elem) {
        for (int i = 0; i < data.size(); ++i)
            data[i] *= elem;
        return *this;
    }
    friend geomVector<T, -1>& operator*(T number, geomVector<T, -1>& vect) {
        for (int i = 0; i < vect.getSize(); ++i)
            vect[i] *= number;
        return vect;
    }

    friend  std::ostream& operator<<(std::ostream& os, geomVector<T, -1>& vect) {
        for (int i = 0; i < vect.getSize() - 1; ++i)
            std::cout << vect[i] << " ";
        std::cout << vect[vect.getSize() - 1];
        return os;
    }
    friend  std::ostream& operator<<(std::ostream& os, geomVector<T, -1>&& vect) {
        for (int i = 0; i < vect.getSize() - 1; ++i)
            std::cout << vect[i] << " ";
        std::cout << vect[vect.getSize() - 1];
        return os;
    }

    geomVector<T, -1>& operator=(const geomVector<T, -1>& right) {
        if (right.getData() != this->data) {
            data.resize(right.getSize());
            for (int i = 0; i < right.getSize(); ++i)
                data[i] = right.getData()[i];
        }
        return *this;
    }

    void push_back(T elem) {
        data.push_back(elem);
    }

    ~geomVector() = default;
};

int main()
{
    geomVector<int, 4> din {1, 2, 3, 5};
    geomVector<int, 4> dinn {1, 2, 3, 4};
    dinn = din;
    std::cout << dinn << std::endl;
    return 0;
}
