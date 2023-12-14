#ifndef _PERSISTENT_NULL_H_
#define _PERSISTENT_NULL_H_

namespace null {

template <typename T>
T value() {
    return T();
}

template <typename T>
void construct(T&) {
}

struct Nil {
    template <typename T>
    operator T() {
        return value<T>();
    }
};

template <typename T>
bool operator==(const T& t, const Nil&) {
    return t == value<T>();
}

template <typename T>
bool operator!=(const T& t, const Nil&) {
    return t != value<T>();
}

template <>
int value();

template <>
double value();

template <>
void construct(int& val);

template <>
void construct(double& val);
}  // namespace null

extern null::Nil nil;

#endif  //_PERSISTENT_NULL_H_
