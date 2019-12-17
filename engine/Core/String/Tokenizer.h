#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include <string>

namespace Core
{
    namespace String
    {
        class CAERULUS_CORE Tokenizer
        {
        public:
            Tokenizer(const std::string& str, const std::string& delims = " ");
            virtual ~Tokenizer();

            const char* Start();
            const char* Next();
            const char* Get();
            void Reset();

        private:

            const char* Start(const std::string& delims);
            const char* Next(const std::string& delims);

            char* m_String;
            char* m_Context;
            const char* m_Original;
            const char* m_Delims;
        };
    }
}
