#pragma once

#include "Core/Interface/NonMovable.h"
#include "Core/Interface/NonCopyable.h"

namespace Core
{
    namespace Logging
    {
        class LogToFile : public Interface::NonCopyable, public Interface::NonMovable
        {
        public:
            LogToFile() = delete;

            explicit LogToFile(const std::string& path) :
                log_file(path)
            {
                if (!log_file.is_open())
                {
                    Log::LogError("Unable to open log file.");
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
