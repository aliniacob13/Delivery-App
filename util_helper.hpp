#pragma once
#include <memory>
#include <algorithm>
#include <string>

class Util {
    public:
    template <class T>
    static T* rawptr(T* p) noexcept { return p; }

    template <class T>
    static T* rawptr(const std::unique_ptr<T>& p) noexcept { return p.get(); }

    template <class Vec, class Predicat>
    static bool erase_first_if(Vec& v, Predicat pred) {
        for (std::size_t i = 0; i < v.size(); ++i) {
            auto* p = rawptr(v[i]);
            if (p && pred(*p)) {
                v.erase(v.begin() + static_cast<long>(i));
                return true;
            }
        }
        return false;
    }

    template <class Vec, class Predicat>
    static void erase_all_if(Vec& v, Predicat pred) {
        v.erase(std::remove_if(v.begin(), v.end(),
                               [&](auto& h) {
                                   auto* p = rawptr(h);
                                   return p && pred(*p);
                               }),
                v.end());
    }

    static int asInt(const char* s) {
        return s ? std::stoi(s) : 0;
    }
    static float asFloat(const char* s) {
        return s ? std::stof(s) : 0.0f;
    }
    static std::string asStr(const char* s) {
        return s ? std::string(s) : std::string();
    }
};