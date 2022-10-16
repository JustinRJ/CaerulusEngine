#pragma once

#include <string>
#include <iostream>

namespace Core
{
    namespace Log
    {
        template<typename... Args>
        void Log(std::string_view file, int lineNum, const Args& ... args)
        {
            std::stringstream ss();
            (ss << ... << args) << "\n";
            std::cout << "FILE: " << file << " LINE: " << lineNum << " MESSAGE: " << ss.str() << std::endl;
        }

        #define Log(...) Log(__FILE__, __LINE__, __VA_ARGS__);

        inline void LogInDebug(const char* debug)
        {
            #ifdef _DEBUG
                std::cout << debug << std::endl;
            #endif
        }

        inline void LogInDebug(std::string_view debug)
        {
            LogInDebug(debug.data());
        }

        inline void LogMessage(const char* message)
        {
            std::cout << message << std::endl;
        }

        inline void LogMessage(std::string_view message)
        {
            LogMessage(message.data());
        }

        inline void LogError(const char* error)
        {
            std::cerr << error << std::endl;
        }

        inline void LogError(std::string_view error)
        {
            LogError(error.data());
        }

        inline void LogError(const char* message, const char* error)
        {
            std::cerr <<  message << "\n" << error << std::endl;
        }

        inline void LogError(std::string_view message, std::string_view error)
        {
            LogError(message.data(), error.data());
        }

        inline void LogException(std::string_view message, std::string_view exception = "")
        {
            std::cerr << std::string(message) +
                "\nException: " + std::string(exception) +
                "\nIn file: " +  __FILE__ +
                "\nOn line: " + std::to_string(__LINE__) << std::endl;
        }
    }
}
