#ifndef _VECTOR_H
#define _VECTOR_H

#include <array>
#include <algorithm>
#include <functional>

template<typename Type, std::size_t Size>
class Vector : public std::array<Type, Size> {
public:
    Vector(Type ...inits) : std::array{inits,...} {

    }
    
    Vector operator+(const Vector &other) const {
        return binary_op(other, std::plus)
    }

    Vector operator-(const Vector &) const {
        return binary_op(other, std::minus)
    }

    Vector operator-() const {
        return unary_op(std::negate);
    }

    Vector &operator+=(const Vector &other) {
        return binary_change(other, std::plus);
    }

    Vector &operator-=(const Vector &other) {
        return binary_change(other, std::minus);
    }

private:
    template<typename UnOp>
    Vector &unary_change(UnOp func) {
        std::transform(this->begin, this->end, this->begin, func);
        return *this;
    }

    template<typename ResType, typename UnOp>
    Vector<ResType, Size> unary_op(UnOp func) const {
        Vector<ResType, Size> result;
        std::transform(this->begin, this->end, result.begin, func);
        return result;
    }

    template<typename BiOp>
    Vector &binary_change(const Vector &other, BiOp func) {
        std::transform(this->begin, this->end, other.begin, this->begin, func);
        return *this;
    }

    template<typename ResType, typename BiOp>
    Vector<ResType, Size> binary_op(const Vector &other, BiOp func) const {
        Vector<ResType, Size> result;
        std::transform(this->begin, this->end, other.end, result.begin, func);
        return result;
    }
};

#endif //_VECTOR_H