#include "stdafx.h"
#include "StringHelper.h"
#include <algorithm>
#include <cstdarg>
#include <cstring>
#include <cctype>

namespace Core
{
    namespace Parser
    {
        const std::string WHITESPACE(" \t\r\n\f\v");

        bool StringHelper::IsValid(const char * str)
        {
            return str && *str;
        }

        bool StringHelper::Compare(const char * str1, const char * str2, bool ignore_case)
        {
            return ignore_case ? _stricmp(str1, str2) : strcmp(str1, str2) == 0;
        }

        bool StringHelper::BeginsWith(const char * str1, const char * str2, bool ignore_case)
        {
            size_t len1 = strlen(str1);
            size_t len2 = strlen(str2);
            if (len1 >= len2)
            {
                std::string str1s(str1, len2);
                return Compare(str1s.c_str(), str2, ignore_case);
            }
            return false;
        }

        bool StringHelper::EndsWith(const char * str1, const char * str2, bool ignore_case)
        {
            size_t len1 = strlen(str1);
            size_t len2 = strlen(str2);
            if (len1 >= len2)
            {
                std::string str1s(str1, len1 - len2, len2);
                return Compare(str1s.c_str(), str2, ignore_case);
            }
            return false;
        }

        bool StringHelper::IsWhitespace(char c)
        {
            return strchr(WHITESPACE.c_str(), c) != NULL;
        }

        std::string StringHelper::Trim(const std::string & str, const char * delim)
        {
            size_t start = str.find_first_not_of(delim);
            if (start == std::string::npos)
            {
                return "";
            }
            size_t end = str.find_last_not_of(delim);
            return str.substr(start, end - start + 1);
        }

        std::string StringHelper::ToLower(const std::string& str)
        {
            std::string s = str;
            std::transform(s.begin(), s.end(), s.begin(), ::tolower);
            return s;
        }

        std::string StringHelper::ToUpper(const std::string& str)
        {
            std::string s = str;
            std::transform(s.begin(), s.end(), s.begin(), ::toupper);
            return s;
        }


        std::string& StringHelper::FormatStartArgs(std::string& dest, size_t start, const char* format, va_list args)
        {
            int size = _vscprintf(format, args);
            dest.resize(size + start);
            _vsnprintf_s(&dest[start], size + 1, size, format, args);
            return dest;
        }

        std::string& StringHelper::FormatArgs(std::string& dest, const char* format, va_list args)
        {
            return FormatStartArgs(dest, 0, format, args);
        }

        std::string& StringHelper::FormatAppendArgs(std::string& dest, const char* format, va_list args)
        {
            return FormatStartArgs(dest, dest.size(), format, args);
        }

        std::string& StringHelper::FormatAppend(std::string& dest, const char* format, ...)
        {
            va_list args;
            va_start(args, format);
            FormatAppendArgs(dest, format, args);
            va_end(args);
            return dest;
        }

        std::string& StringHelper::Format(std::string& dest, const char* format, ...)
        {
            va_list args;
            va_start(args, format);
            FormatArgs(dest, format, args);
            va_end(args);
            return dest;
        }

        std::string StringHelper::Format(const char* format, ...)
        {
            va_list args;
            va_start(args, format);
            std::string str;
            FormatArgs(str, format, args);
            va_end(args);
            return str;
        }

        std::vector<std::string> StringHelper::Tokenize(const char* str, const char* delim)
        {
            char* context = NULL;
            std::vector<std::string> tokens;
            char* val = _strdup(str);
            char* pch = strtok_s(val, delim, &context);
            while (pch != NULL)
            {
                tokens.push_back(pch);
                pch = strtok_s(NULL, delim, &context);
            }
            free(val);
            return tokens;
        }

        std::string StringHelper::Type(const char* name)
        {
            std::string out = "";
            std::vector<std::string> tokens = Tokenize(name, "<,");

            for (std::string& token : tokens)
            {
                bool first = out.empty();
                size_t pos = token.find_last_of(':');
                if (pos != std::string::npos)
                {
                    out += token.substr(pos + 1);

                    if (first && tokens.size() > 1)
                    {
                        out += '<';
                    }
                }
            }
            return out;
        }
    }
}