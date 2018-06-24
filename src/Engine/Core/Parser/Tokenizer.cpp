#include "stdafx.h"
#include "Tokenizer.h"
#include <stdlib.h>
#include <string.h>

namespace Core
{
    namespace Parser
    {
        Tokenizer::Tokenizer(const std::string& str, const std::string& delim) :
            m_String(NULL),
            m_Context(NULL),
            m_Original(str.c_str()),
            m_Delims(delim.c_str())

        {
        }

        Tokenizer::~Tokenizer()
        {
            Reset();
        }

        void Tokenizer::Reset()
        {
            if (m_String)
            {
                free(m_String);
                m_String = NULL;
            }
            m_Context = NULL;
        }

        const char* Tokenizer::Start(const char* delims)
        {
            Reset();
            m_String = _strdup(m_Original);
            return strtok_s(m_String, delims, &m_Context);
        }

        const char* Tokenizer::Start()
        {
            return Start(m_Delims);
        }

        const char* Tokenizer::Next(const char* delims)
        {
            return strtok_s(NULL, delims, &m_Context);
        }

        const char* Tokenizer::Next()
        {
            return Next(m_Delims);
        }

        const char* Tokenizer::Get()
        {
            return m_Context ? Next() : Start();
        }
    }
}