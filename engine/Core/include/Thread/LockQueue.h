#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include <mutex>
#include <queue>

namespace Core
{
    namespace Thread
    {
        template <class T>
        class CAERULUS_CORE LockQueue
        {
        public:
            LockQueue() = default;

            void Push(const T& value)
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_queue.push(value);
            }

            T Pop() const
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                return m_queue.pop();
            }

            bool Empty() const
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                return m_queue.empty();
            }

            bool Size() const
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                return m_queue.size();
            }
        private:
            mutable std::mutex m_mutex;
            std::queue<T> m_queue;
        };
    }
}