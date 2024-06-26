#pragma once

#include "Interface\NonCopyable.h"

namespace Core
{
    namespace Interface
    {
        class Singleton : Interface::NonCopyable
        {
        public:
            virtual ~Singleton() = default;

            static Singleton& GetInstance()
            {
                return s_instance;
            }

        private:
            Singleton() = default;
            static Singleton s_instance;
        };
    }
}
