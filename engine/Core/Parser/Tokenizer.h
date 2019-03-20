#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include <string>

namespace Core
{
    namespace Parser
    {
        class CAERULUS_CORE Tokenizer
        {
        public:
            Tokenizer(const std::string&, const std::string& = " ");
            virtual ~Tokenizer();

            const char* Start();
            const char* Start(const std::string& delims);
            
            const char* Next();
            const char* Next(const std::string& delims);

            const char* Get();
            void Reset();

        private:

            char* m_String;
            char* m_Context;
            const char* m_Original;
            const char* m_Delims;
        };
    }
}
