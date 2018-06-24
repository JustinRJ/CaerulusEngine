#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include <map>

namespace Core
{
    namespace Input
    {
        class Bindings
        {
            typedef std::map<std::string, long> BindingMap;

        public:
            template <typename T>
            CAERULUS_CORE static inline std::string Convert(const T& src)
            {
                std::string dest;
                Convert<T>(dest, src);
                return dest;
            }

            template <typename T>
            CAERULUS_CORE static bool Convert(std::string& dest, const T& src)
            {
                long bit = static_cast<long>(src);
                return Convert(LookUp<T>(), dest, bit);
            }

            template <typename T>
            CAERULUS_CORE static inline T Convert(const std::string& src)
            {
                T dest;
                Convert<T>(dest, src);
                return dest;
            }

            template <typename T>
            CAERULUS_CORE static bool Convert(T& dest, const std::string& src)
            {
                long bit;
                if (Convert(LookUp<T>(), bit, src))
                {
                    dest = static_cast<T>(bit);
                    return true;
                }
                return false;
            }

            CAERULUS_CORE static const BindingMap& Add(size_t type, const char* str, long arg, ...);

        protected:

            static std::map<size_t, BindingMap> s_Data;

            template <typename T>
            static inline const BindingMap& LookUp()
            {
                static const BindingMap& lookup = BindingMap();
                return lookup;
            }

            static bool Convert(const BindingMap& lookup, long& dest, const std::string& src);
            static bool Convert(const BindingMap& lookup, std::string& dest, const long& src);
        };
    }
}