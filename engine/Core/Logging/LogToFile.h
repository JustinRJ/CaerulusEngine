#pragma once

#include "Core/Interface/NonCopyable.h"

namespace Core
{
    namespace Logging
    {
        class LogToFile : public Interface::NonCopyable
        {
        public:
            LogToFile() = delete;
            virtual ~LogToFile() = delete;

            static void LogToFileInDebug(const char* debug)
            {
                // TODO
            }

            static void LogToFileInDebug(const std::string& debug)
            {
                LogToFileInDebug(debug.c_str());
            }

            static void LogMessageToFile(const char* message)
            {
                // TODO
            }

            static void LogMessageToFile(const std::string& message)
            {
                LogMessageToFile(message.c_str());
            }

            static void LogErrorToFile(const char* error)
            {
                // TODO
            }

            static void LogErrorToFile(const std::string& error)
            {
                LogMessageToFile(error.c_str());
            }

            static void LogErrorToFile(const char* message, const char* error)
            {
                // TODO
            }

            static void LogErrorToFile(const std::string& message, const std::string& error)
            {
                LogErrorToFile(message.c_str(), error.c_str());
            }

            static void LogExceptionToFile(const std::string& message, const std::string& exception = "")
            {
                // TODO
            }
        };
    }
}
