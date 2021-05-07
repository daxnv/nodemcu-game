#ifndef _VECTOR_H
#define _VECTOR_H

#include <array>
#include <algorithm>
#include <functional>

template<typename Type, std::size_t Size>
class Vector : public std::array<Type, Size> {
public:
    template<typename ...I>
    Vector(I... inits) : std::array<Type, Size>{inits...} {

    }

    Vector operator+(const Vector &other) const {
        return binary_op(other, std::plus<Type>());
    }

    Vector operator-(const Vector &other) const {
        return binary_op(other, std::minus<Type>());
    }

    Vector operator-() const {
        return unary_op(std::negate<Type>());
    }

    Vector &operator+=(const Vector &other) {
        return binary_change(other, std::plus<Type>());
    }

    Vector &operator-=(const Vector &other) {
        return binary_change(other, std::minus<Type>());
    }

private:
    template<typename UnOp>
    Vector &unary_change(UnOp func) {
        std::transform(this->begin(), this->end(), this->begin(), func);
        return *this;
    }

    template<typename ResType, typename UnOp>
    Vector<ResType, Size> unary_op(UnOp func) const {
        Vector<ResType, Size> result;
        std::transform(this->begin(), this->end(), result.begin(), func);
        return result;
    }

    template<typename ArgType, typename BiOp>
    Vector &binary_change(const Vector<ArgType, Size> &other, BiOp func) {
        std::transform(this->begin(), this->end(), other.begin(), this->begin(), func);
        return *this;
    }

    template<typename ArgType, typename ResType, typename BiOp>
    Vector<ResType, Size> binary_op(const Vector<ArgType, Size> &other, BiOp func) const {
        Vector<ResType, Size> result;
        std::transform(this->begin(), this->end(), other.begin(), result.begin(), func);
        return result;
    }
};

#endif //_VECTOR_H
