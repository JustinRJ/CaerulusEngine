#include "stdafx.h"
#include "Bindings.h"
#include "../Parser/StringHelper.h"
#include <stdarg.h>
#include <sstream>

namespace Core
{
    namespace Input
    {
        std::map<size_t, Bindings::BindingMap> Bindings::s_Data;

        const Bindings::BindingMap& Bindings::Add(size_t type, const char* str, long arg, ...)
        {
            BindingMap& lookup = s_Data[type];

            if (lookup.empty())
            {
                va_list arguments;
                va_start(arguments, arg);

                long val = -1LL;
                std::vector<std::string> vars = Parser::StringHelper::Tokenize(str, ",");
                for (size_t i = 0; i < vars.size(); ++i)
                {
                    const char* var = vars[i].c_str();

                    size_t start = strspn(var, " \t\r\n");
                    size_t end = strcspn(&var[start], " \t\r\n");
                    std::string name(&var[start], &var[start + end]);

                    strchr(var, '=') ? val = arg : ++val;

                    lookup[name] = val;
                    arg = va_arg(arguments, long);
                }
                va_end(arguments);
            }
            return lookup;
        }

        bool Bindings::Convert(const BindingMap& lookup, long& dest, const std::string& src)
        {
            // Find number from string
            BindingMap::const_iterator var = lookup.find(src);
            if (var != lookup.cend())
            {
                dest = var->second;
            }
            // Try to convert from number
            std::stringstream(src) >> dest;
            return true;
        }

        bool Bindings::Convert(const BindingMap& lookup, std::string& dest, const long& src)
        {
            // Find string from number
            for (BindingMap::const_iterator it = lookup.cbegin(); it != lookup.cend(); ++it)
            {
                if (it->second == src)
                {
                    dest = it->first;
                    return true;
                }
            }
            // Convert to decimal
            std::stringstream str("");
            str << src;
            dest = str.str();
            return true;
        }
    }
}