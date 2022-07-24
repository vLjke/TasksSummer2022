#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

class Polynom {
private:
    std::vector<double> coeffs;
public:
    Polynom() = default;
    Polynom(std::initializer_list<double> list) {
        coeffs.resize(list.size());
        for (int i = 0; i < list.size(); ++i)
            coeffs[i] = *(list.begin() + i);
        for (int i = list.size() - 1; i >= 0; --i) {
            if (coeffs[i] == 0)
                coeffs.erase(std::next(coeffs.begin(), i));
            else break;
        }
    }
    Polynom(std::vector<double>& vec) {
        coeffs = vec;
        for (int i = vec.size() - 1; i >= 0; --i) {
            if (coeffs[i] == 0)
                coeffs.erase(std::next(coeffs.begin(), i));
            else break;
        }
    }

    double &operator[](size_t index) {
        if (index >= 0 && index <= this->getMaxPower())
            return coeffs[index];
        else {
            std::cout << "Error! Wrong index given." << std::endl;
            exit(1);
        }
    }

    std::vector<double> getCoeffs() const {
        return coeffs;
    }
    unsigned int getMaxPower() const {
        if (!this->coeffs.empty())
            return coeffs.size() - 1;
        else
            return 0;
    }

    Polynom &operator=(const Polynom& right) {
        if (!right.coeffs.empty()) {
            this->coeffs.resize(right.getMaxPower() + 1);
            for (int i = 0; i < coeffs.size(); ++i)
                coeffs[i] = right.getCoeffs()[i];
        }
        else {
            this->coeffs.clear();
        }
        return *this;
    }
    bool operator==(const Polynom& right) const {
        if (this->getMaxPower() != right.getMaxPower())
            return false;
        for (int i = 0; i < right.getMaxPower() + 1; ++i)
            if (right.getCoeffs()[i] != this->coeffs[i])
                return false;
        return true;
    }

    Polynom operator+(const Polynom& right) const {
        Polynom result {};
        unsigned int minPower;
        bool rightPowerIsBigger;
        if (right.getMaxPower() > this->getMaxPower()) {
            result.coeffs.resize(right.getMaxPower() + 1);
            minPower = this->getMaxPower();
            rightPowerIsBigger = true;
        }
        else {
            result.coeffs.resize(this->getMaxPower() + 1);
            minPower = right.getMaxPower();
            rightPowerIsBigger = false;
        }
        for (int i = 0; i < minPower + 1; ++i)
            result[i] = this->coeffs[i] + right.getCoeffs()[i];
        for (int i = minPower + 1; i < result.coeffs.size(); ++i) {
            if (rightPowerIsBigger)
                result[i] = right.getCoeffs()[i];
            else
                result[i] = this->coeffs[i];
        }
        for (int i = result.coeffs.size() - 1; i >= 0; --i) {
            if (result.coeffs[i] == 0)
                result.coeffs.erase(std::next(result.coeffs.begin(), i));
            else break;
        }
        return result;
    }
    Polynom operator-(const Polynom& right) const {
        Polynom result {};
        unsigned int minPower;
        bool rightPowerIsBigger;
        if (right.getMaxPower() > this->getMaxPower()) {
            result.coeffs.resize(right.getMaxPower() + 1);
            minPower = this->getMaxPower();
            rightPowerIsBigger = true;
        }
        else {
            result.coeffs.resize(this->getMaxPower() + 1);
            minPower = right.getMaxPower();
            rightPowerIsBigger = false;
        }
        for (int i = 0; i < minPower + 1; ++i)
            result[i] = this->coeffs[i] - right.getCoeffs()[i];
        for (int i = minPower + 1; i < result.coeffs.size(); ++i) {
            if (rightPowerIsBigger)
                result[i] = -right.getCoeffs()[i];
            else
                result[i] = this->coeffs[i];
        }
        for (int i = result.coeffs.size() - 1; i >= 0; --i) {
            if (result.coeffs[i] == 0)
                result.coeffs.erase(std::next(result.coeffs.begin(), i));
            else break;
        }
        return result;
    }

    friend Polynom& operator-(Polynom& vect) {
        for (int i = 0; i < vect.getMaxPower() + 1; ++i)
            vect[i] = -vect[i];
        return vect;
    }
    friend Polynom& operator+(Polynom& vect) {
        return vect;
    }

    Polynom& operator*(const double elem) {
        if (elem != 0) {
            for (int i = 0; i < this->getMaxPower() + 1; ++i)
                coeffs[i] *= elem;
        }
        else {
            this->coeffs.clear();
            this->coeffs.push_back(0);
        }
        return *this;
    }
    friend Polynom& operator*(const double number, Polynom& vect) {
        if (number != 0) {
            for (int i = 0; i < vect.getMaxPower() + 1; ++i)
                vect[i] *= number;
        }
        else {
            vect.coeffs.clear();
            vect.coeffs.push_back(0);
        }
        return vect;
    }

    Polynom operator*(const Polynom& right) const {
        if (!right.coeffs.empty() && !this->coeffs.empty()) {
            Polynom result{};
            result.coeffs.resize(this->coeffs.size() + right.coeffs.size() - 1);
            for (int i = 0; i < this->coeffs.size(); ++i)
                for (int j = 0; j < right.coeffs.size(); ++j)
                    result[i + j] += this->coeffs[i] * right.coeffs[j];
            for (int i = result.coeffs.size() - 1; i >= 0; --i) {
                if (result.coeffs[i] == 0)
                    result.coeffs.erase(std::next(result.coeffs.begin(), i));
                else break;
            }
            return result;
        }
        else {
            std::cout << "Cannot multiply empty polynom!" << std::endl;
            exit(1);
        }
    }

    ///Возвращает целую часть(first) и остаток(second) от деления одного многочлена на другой
    std::pair<Polynom, Polynom> operator/(const Polynom& right) const {
        Polynom f {};
        Polynom s = *this;
        if (this->getMaxPower() >= right.getMaxPower()) {
            f.coeffs.resize(this->getMaxPower() - right.getMaxPower() + 1);
            for (int i = 0; i < this->getMaxPower() - right.getMaxPower() + 1; ++i) {
                f.coeffs[this->getMaxPower() - right.getMaxPower() - i] = s.coeffs[s.getMaxPower()]
                                                                          / right.coeffs[right.getMaxPower()];
                s = *this - f * right;
            }
        }
        for (int i = f.coeffs.size() - 1; i >= 0; --i) {
            if (f.coeffs[i] == 0)
                f.coeffs.erase(std::next(f.coeffs.begin(), i));
            else break;
        }
        for (int i = s.coeffs.size() - 1; i >= 0; --i) {
            if (s.coeffs[i] == 0)
                s.coeffs.erase(std::next(s.coeffs.begin(), i));
            else break;
        }
        return std::make_pair(f, s);
    }

    friend std::ostream& operator<<(std::ostream& os, const Polynom& vect) {
        if (!vect.coeffs.empty()) {
            for (int i = 0; i < vect.getMaxPower() + 1; ++i)
                os << vect.getCoeffs()[vect.getMaxPower() - i] << "x^" << vect.getMaxPower() - i << " ";
        }
        else
            os << "Polynom is empty(identically equal to 0).";
        return os;
    }
    friend std::ostream& operator<<(std::ostream &os, const Polynom&& vect) {
        if (!vect.coeffs.empty()) {
            for (int i = 0; i < vect.getMaxPower() + 1; ++i)
                os << vect.getCoeffs()[vect.getMaxPower() - i] << "x^" << vect.getMaxPower() - i << " ";
        }
        else
            os << "Polynom is empty(identically equal to 0).";
        return os;
    }

    ///Возвращает один элемент из множества первообразных (C == 0)
    Polynom antiderivative() const {
        Polynom result {};
        result.coeffs.resize(this->getMaxPower() + 2);
        result[0] = 0;
        for (int i = 1; i < result.getMaxPower() + 1; ++i) {
            if (this->coeffs[i - 1] != 0) {
                result[i] = this->coeffs[i - 1] / i;
            } else
                result[i] = 0;
        }
        return result;
    }

    Polynom derivative() const {
        Polynom result {};
        if (this->getMaxPower() != 0) {
            result.coeffs.resize(this->getMaxPower());
            for (int i = 0; i < this->getMaxPower(); ++i)
                result[i] = this->coeffs[i + 1] * (i + 1);
        }
        else {
            result.coeffs.push_back(0);
        }
        return result;
    }

    ~Polynom() = default;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class RazreszhPolynom {
private:
    std::map<unsigned int, double> data;
public:
    RazreszhPolynom() = default;
    RazreszhPolynom(std::initializer_list<std::pair<unsigned int, double>> list) {
        for (int i = 0; i < list.size(); ++i)
            if ((list.begin() + i)->second != 0)
                data.insert(*(list.begin() + i));
    }
    RazreszhPolynom(std::initializer_list<double> list) {
        for (int i = 0; i < list.size(); ++i)
            if (*(list.begin() + i) != 0)
                data.insert(std::make_pair(i, *std::next(list.begin(), i)));
    }
    RazreszhPolynom(std::vector<double> vec) {
        for (int i = 0; i < vec.size(); ++i)
            if (*(vec.begin() + i) != 0)
                data.insert(std::make_pair(i, *std::next(vec.begin(), i)));
    }

    RazreszhPolynom& operator=(const RazreszhPolynom& right) {
        if (this != &right) {
            this->data.clear();
            this->data.insert(right.data.begin(), right.data.end());
        }
        return *this;
    }
    bool operator==(const RazreszhPolynom& right) const {
        if (this->data.size() != right.data.size())
            return false;
        else
            for (int i = 0; i < this->data.size(); ++i) {
                auto it1 = std::next(this->data.begin(), i);
                auto it2 = std::next(right.data.begin(), i);
                if (it1->first != it2->first)
                    return false;
                else if (it1->second != it2->second)
                    return false;
            }
        return true;
    }

    RazreszhPolynom operator+(const RazreszhPolynom& right) const {
        RazreszhPolynom result {};
        result = *this;
        for (auto& v : right.data) {
            if (result.data.find(v.first) != result.data.end()) {
                result.data.find(v.first)->second += v.second;
                if (result.data.find(v.first)->second == 0)
                    result.data.erase(v.first);
            }
            else
                result.data.insert(v);
        }
        return result;
    }
    RazreszhPolynom operator-(const RazreszhPolynom& right) const {
        RazreszhPolynom result {};
        result = *this;
        for (auto& v : right.data) {
            if (result.data.find(v.first) != result.data.end()) {
                result.data.find(v.first)->second -= v.second;
                if (result.data.find(v.first)->second == 0)
                    result.data.erase(v.first);
            }
            else {
                result.data.insert(v);
                result.data[v.first] = -result.data[v.first];
            }
        }
        return result;
    }

    friend RazreszhPolynom& operator-(RazreszhPolynom& vec) {
        for (auto& v : vec.data)
            v.second = -v.second;
        return vec;
    }
    friend RazreszhPolynom& operator+(RazreszhPolynom& vec) {
        return vec;
    }

    RazreszhPolynom& operator*(const double number) {
        for (auto& v : this->data)
            v.second *= number;
        return *this;
    }
    friend RazreszhPolynom& operator*(const double number, RazreszhPolynom& right) {
        for (auto& v : right.data)
            v.second += number;
        return right;
    }

    RazreszhPolynom operator*(const RazreszhPolynom& right) const {
        RazreszhPolynom result {};
        for (auto& x : right.data)
            for (auto& y : this->data) {
                if (result.data.find(x.first + y.first) == result.data.end())
                    result.data.insert(std::make_pair(x.first + y.first, x.second * y.second));
                else
                        result.data[x.first + y.first] += x.second * y.second;
            }
        for (auto& v : result.data)
            if (v.second == 0)
                result.data.erase(v.first);
        return result;
    }

    ///Возвращает целую часть(first) и остаток(second) от деления одного многочлена на другой
    std::pair<RazreszhPolynom, RazreszhPolynom> operator/(const RazreszhPolynom& right) const {
        RazreszhPolynom f {};
        RazreszhPolynom s = *this;
        if (this->data.rbegin()->first >= right.data.rbegin()->first) {
            for (int i = 0; i < this->data.rbegin()->first - right.data.rbegin()->first + 1; ++i) {
                f.data.insert(std::make_pair(this->data.rbegin()->first - right.data.rbegin()->first - i,
                                             s.data.rbegin()->second / right.data.rbegin()->second));
                s = *this - f * right;
            }
        }
        return std::make_pair(f, s);
    }

    friend std::ostream& operator<<(std::ostream& os, RazreszhPolynom& right) {
        if (!right.data.empty()) {
            for (auto& it = --right.data.end(); it != right.data.begin(); --it) {
                os << it->second << "x^" << it->first << " ";
            }
            os << right.data.begin()->second << "x^" << right.data.begin()->first << " ";
        }
        else
            std::cout << "Polynom is empty(identically equal to 0)." << std::endl;
        return os;
    }
    friend std::ostream& operator<<(std::ostream& os, RazreszhPolynom&& right) {
        if (!right.data.empty()) {
            for (auto& it = --right.data.end(); it != right.data.begin(); --it) {
                os << it->second << "x^" << it->first << " ";
            }
            os << right.data.begin()->second << "x^" << right.data.begin()->first << " ";
        }
        else
            std::cout << "Polynom is empty(identically equal to 0)." << std::endl;
        return os;
    }

    ///Возвращает один элемент из множества первообразных (C == 0)
    RazreszhPolynom antiderivative() const {
        RazreszhPolynom result {};
        for (auto& v : this->data)
            result.data.insert(std::make_pair(v.first + 1, v.second / (v.first + 1)));
        return result;
    }
    RazreszhPolynom derivative() const {
        RazreszhPolynom result {};
        for (auto& v : this->data)
            if (v.first != 0)
                result.data.insert(std::make_pair(v.first - 1, v.second * v.first));
        return result;
    }

    ~RazreszhPolynom() = default;
};

int main() {
    return 0;
}
