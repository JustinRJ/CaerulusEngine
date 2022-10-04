#pragma once

#include <string>
#include <iostream>

namespace Core
{
    namespace Logging
    {
        template<typename... Args>
        void Log(const std::string& file, int lineNum, const Args& ... args)
        {
            std::stringstream ss;
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

        inline void LogInDebug(const std::string& debug)
        {
            LogInDebug(debug.c_str());
        }

        inline void LogMessage(const char* message)
        {
            std::cout << message << std::endl;
        }

        inline void LogMessage(const std::string& message)
        {
            LogMessage(message.c_str());
        }

        inline void LogError(const char* error)
        {
            std::cerr << error << std::endl;
        }

        inline void LogError(const std::string& error)
        {
            LogError(error.c_str());
        }

        inline void LogError(const char* message, const char* error)
        {
            std::cerr <<  message << "\n" << error << std::endl;
        }

        inline void LogError(const std::string& message, const std::string& error)
        {
            LogError(message.c_str(), error.c_str());
        }

        inline void LogException(const std::string& message, const std::string& exception = "")
        {
            std::cerr << message +
                "\nException: " + exception +
                "\nIn file: " +  __FILE__ +
                "\nOn line: " + std::to_string(__LINE__) << std::endl;
        }
    }
}
