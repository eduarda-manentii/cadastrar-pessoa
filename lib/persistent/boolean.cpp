#include "boolean.h"

#include "driver/Extractor.h"
#include "driver/Statement.h"

namespace persistent {

boolean::boolean() : tribool(indeterminate) {
}
boolean::boolean(const boolean& b) : tribool(b) {
}
boolean::boolean(const tribool& b) : tribool(b) {
}
boolean::boolean(bool b) : tribool(b) {
}

bool operator==(const boolean& b1, const boolean& b2) {
    if (indeterminate(b1)) {
        return indeterminate(b2);
    }
    return b1 == b2;
}

template <typename T>
std::string column_typename();

template <>
std::string column_typename<boolean>() {
    return "integer";
}

template <>
void Extractor::extract(boolean& var) {
    int n;
    extract(n);
    var = n;
}

template <>
void Statement::bind(boolean val) {
    bind_int(val ? 1 : 0);
}

std::string to_string(const boolean& b) {
    if (indeterminate(b)) {
        return "";
    } else if (b) {
        return "true";
    } else {
        return "false";
    }
}

void from_string(boolean& b, const std::string& s) {
    if (!s.empty()) {
        if (s == "true") {
            b = true;
        } else if (s == "false") {
            b = false;
        }
    }
}

}  // namespace persistent
