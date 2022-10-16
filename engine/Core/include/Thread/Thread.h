#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include <thread>
#include <atomic>
#include <chrono>

namespace Core
{
    namespace Thread
    {
        class CAERULUS_CORE Thread
        {
        public:
            Thread(bool start = false)
            {
                if (start)
                {
                    Start();
                }
            }

            virtual ~Thread()
            {
                Stop();
            }

            // Run() override must call while(IsRunning()) to loop
            virtual void Run() = 0;

            bool IsRunning() const
            {
                return m_running;
            }

            void Start()
            {
                if (!m_running)
                {
                    m_running = true;
                    m_thread = std::thread(&Thread::Run, this);
                }
            }

            void Stop()
            {
                if (m_running)
                {
                    m_running = false;
                    if (m_thread.joinable())
                    {
                        m_thread.join();
                    }
                }
            }

            template <class T>
            static void YieldMainThread(const T& time)
            {
                std::this_thread::sleep_for(time);
            }

        private:
            std::atomic<bool> m_running = false;
            std::thread m_thread;
        };
    }
}