#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include <string>
#include <vector>

namespace Core
{
    namespace Parser
    {
        struct StringHelper
        {
            CAERULUS_CORE static bool IsValid(const char* str);
            CAERULUS_CORE static bool Compare(const char* str1, const char* str2, bool ignore_case = true);
            CAERULUS_CORE static bool EndsWith(const char* str1, const char* str2, bool ignore_case = true);
            CAERULUS_CORE static bool BeginsWith(const char* str1, const char* str2, bool ignore_case = true);
            CAERULUS_CORE static bool IsWhitespace(char c);

            CAERULUS_CORE static std::string Trim(const std::string& str, const char* delim = " \t\r\n\f\v");
            CAERULUS_CORE static std::string ToLower(const std::string& str);
            CAERULUS_CORE static std::string ToUpper(const std::string& str);

            CAERULUS_CORE static std::string Format(const char* format, ...);
            CAERULUS_CORE static std::string& Format(std::string& dest, const char* format, ...);
            CAERULUS_CORE static std::string& FormatArgs(std::string& dest, const char* format, va_list args);
            CAERULUS_CORE static std::string& FormatAppend(std::string& dest, const char* format, ...);
            CAERULUS_CORE static std::string& FormatStartArgs(std::string& dest, size_t start, const char* format, va_list args);
            CAERULUS_CORE static std::string& FormatAppendArgs(std::string& dest, const char* format, va_list args);

            CAERULUS_CORE static std::string Type(const char* name);

            CAERULUS_CORE static std::vector<std::string> Tokenize(const char* str, const char* delim = " ");

            template <typename T>
            CAERULUS_CORE static const std::string& Type()
            {
                static const std::string str = Type(typeid(T).name());
                return str;
            }

            template<typename T>
            CAERULUS_CORE static std::string ToBinary(T number)
            {
                size_t size = sizeof(T) * 8;
                std::string str(size, "0");
                for (size_t i = 0; i < size; ++i)
                {
                    if (number % 2)
                    {
                        str[i] = "1";
                    }
                    number >>= 1;
                }
                return str;
            }

            template<typename T>
            CAERULUS_CORE static void FromString(const std::string& str, T& value)
            {
                std::std::stringstream(str) >> value;
            }

            template<typename T>
            CAERULUS_CORE static T FromString(const std::string& str)
            {
                T value;
                FromString(str, value);
                return value;
            }
        };
    }
}