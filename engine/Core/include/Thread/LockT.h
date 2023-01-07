#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include <mutex>

namespace Core
{
    namespace Thread
    {
        template <class T>
        class CAERULUS_CORE LockT
        {
        public:
            LockT() = default;

            std::pair<T&, std::scoped_lock<std::mutex>> GetT()
            {
                return std::pair<T&, std::scoped_lock<std::mutex>>{ m_T, m_mutex };
            }

            std::pair<const T&, std::scoped_lock<std::mutex>> GetT() const
            {
                return std::pair<const T&, std::scoped_lock<std::mutex>>{ m_T, m_mutex };
            }

        private:
            mutable std::mutex m_mutex;
            T m_T;
        };
    }
}