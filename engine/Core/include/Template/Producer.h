#pragma once

#include <map>

namespace Core
{
    namespace Template
    {
        template <class T>
        class Producer
        {
        public:

            Producer(const T defaultValue) :
                m_defaultValue(defaultValue)
            {}

            void Register(void* consumer)
            {
                m_producer[consumer] = m_defaultValue;
            }

            void Unregister(void* consumer)
            {
                m_producer.erase(consumer);
            }

            void Set(void* consumer, const T value)
            {
                Register(consumer);
                for (void* regValPair : m_producer)
                {
                    regValPair.second = value;
                }
            }

            void Get(void* consumer) const
            {
                auto it = m_producer.find(consumer);
                if (it == m_producer.end())
                {
                    throw std::runtime_error("Producer.h - Failed to find registered consumer!");
                }
                T val = it->second;
                it->second = m_defaultValue;
                return val;
            }

        private:
            const T m_defaultValue;
            std::map<void*, T> m_producer;
        };
    }
}
