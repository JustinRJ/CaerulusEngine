#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include <string>
#include <iostream>

namespace Core
{
    namespace Logging
    {
        class CAERULUS_CORE Log
        {
        public:

            static void LogInDebug(const char* debug)
            {
                #ifdef _DEBUG
                    std::cout << debug << std::endl;
                #endif
            }

            static void LogInDebug(const std::string& debug)
            {
                LogInDebug(debug.c_str());
            }

            static void LogMessage(const char* message)
            {
                std::cout << message << std::endl;
            }

            static void LogMessage(const std::string& message)
            {
                LogMessage(message.c_str());
            }

            static void LogError(const char* error)
            {
                std::cerr << error << std::endl;
            }

            static void LogError(const std::string& error)
            {
                LogMessage(error.c_str());
            }

            static void LogError(const char* message, const char* error)
            {
                std::cerr << error << std::endl;
            }

            static void LogError(const std::string& message, const std::string& error)
            {
                LogError(message.c_str(), error.c_str());
            }

            static void LogException(const std::string& message, const std::string& exception = "")
            {
                std::cerr << message + "\n" + exception + "\n" + "\n In file: " +
                    __FILE__ + "\n On line: " + std::to_string(__LINE__) << std::endl;
            }
        };
    }
}
