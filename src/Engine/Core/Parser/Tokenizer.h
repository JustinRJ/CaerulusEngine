#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include <string>

namespace Core
{
    namespace Parser
    {
        struct Tokenizer
        {
            CAERULUS_CORE Tokenizer(const std::string&, const std::string& = " ");
            CAERULUS_CORE ~Tokenizer();

            CAERULUS_CORE const char* Start();
            CAERULUS_CORE const char* Start(const char* delims);
            
            CAERULUS_CORE const char* Next();
            CAERULUS_CORE const char* Next(const char* delims);

            CAERULUS_CORE const char* Get();
            CAERULUS_CORE void Reset();

        protected:

            char* m_String;
            char* m_Context;
            const char* m_Original;
            const char* m_Delims;
        };
    }
}
