#pragma once

#include <string>
#include <fstream>

#include "Interface/NonCopyable.h"

namespace Core
{
    namespace Logging
    {
        class LogToFile : private Interface::NonCopyable
        {
        public:
            LogToFile() = delete;

            explicit LogToFile(const std::string& path) :
                log_file(path)
            {
                if (!log_file.is_open())
                {
                    LogError("Unable to open log file.");
                }
            }

            ~LogToFile()
            {
                log_file.close();
            }

            template<typename T>
            void Write(T const& v)
            {
                log_file << v;
            }

            template<typename Arg, typename ...Args>
            void Write(Arg const& arg, Args const&... args)
            {
                Write(arg);
                Write(args...);
            }
        private:
            std::ofstream log_file;
        };
    }
}
