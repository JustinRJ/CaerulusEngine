#pragma once

#include <chrono>

namespace Core
{
    namespace Time
    {
        class Timer
        {
        public:
            Timer(bool start = false) :
                m_running(false)
            {
                if (start)
                {
                    Start();
                }
            }

            virtual ~Timer() = default;

            void Start()
            {
                if (!m_running)
                {
                    m_running = true;
                    m_lastTimePoint = Clock::now();
                }
            }

            float Delta()
            {
                float deltaTime = 0.f;
                if (m_running)
                {
                    auto now = Clock::now();
                    deltaTime = duration_cast<std::chrono::duration<float>>(now - m_lastTimePoint).count();
                    m_lastTimePoint = now;
                }
                else
                {
                    Start();
                }
                return deltaTime;
            }

            void Stop()
            {
                m_running = false;
            }

        private:
            using Clock = std::chrono::high_resolution_clock;

            bool m_running = false;
            std::chrono::high_resolution_clock::time_point m_lastTimePoint;
        };
    }
}
