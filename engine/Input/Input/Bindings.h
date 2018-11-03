#pragma once

#include <map>

namespace Input
{
    class Bindings
    {
        typedef std::map<std::string, long> BindingMap;

    public:
        template <typename T>
        static std::string Convert(const T& src)
        {
            std::string dest;
            Convert<T>(dest, src);
            return dest;
        }

        template <typename T>
        static void Convert(std::string& dest, const T& src)
        {
            long bit = static_cast<long>(src);
            Convert(LookUp<T>(), dest, bit);
        }

        template <typename T>
        static T Convert(const std::string& src)
        {
            T dest;
            Convert<T>(dest, src);
            return dest;
        }

        template <typename T>
        static void Convert(T& dest, const std::string& src)
        {
            long bit;
            Convert(LookUp<T>(), bit, src);
            dest = static_cast<T>(bit);
        }

        static const BindingMap& Add(size_t type, const std::string& str, long arg, ...);

    private:

        static std::map<size_t, BindingMap> s_Data;

        template <typename T>
        static const BindingMap& LookUp()
        {
            static const BindingMap& lookup = BindingMap();
            return lookup;
        }

        static void Convert(const BindingMap& lookup, long& dest, const std::string& src);
        static void Convert(const BindingMap& lookup, std::string& dest, const long& src);
    };
}
