#pragma once

#include <string>
#include <iostream>

#define Debug(x) std::cout << #x << " " << x << std::endl;

namespace Core
{
    namespace Logging
    {
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
            LogMessage(error.c_str());
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
