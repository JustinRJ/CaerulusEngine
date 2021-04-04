#include "stdafx.h"

#include "Timer.h"
#include "Time.h"

namespace Core
{
    using namespace std::chrono;

    namespace Time
    {
        Timer::Timer(bool start) :
            m_running(false)
        {
            if (start)
            {
                Start();
            }
        }

        void Timer::Start()
        {
            if (m_running == false)
            {
                m_running = true;
                m_lastTimePoint = high_resolution_clock::now();
            }
        }

        float Timer::Delta()
        {
            float deltaTime = 0.f;
            if (m_running)
            {
                high_resolution_clock::time_point now = high_resolution_clock::now();
                deltaTime = duration_cast<duration<float>>(now - m_lastTimePoint).count();
                m_lastTimePoint = now;
            }
            else
            {
                Start();
            }

            return deltaTime;
        }

        void Timer::Stop()
        {
            if (m_running)
            {
                m_running = false;
            }
        }

        void Timer::Sleep(time_t sec)
        {
            ::Sleep(static_cast<DWORD>(sec * 1000.0));
        }

        void Timer::Sleep(const Time& time)
        {
            ::Sleep(static_cast<DWORD>(time.GetTime() * 1000.0));
        }
    }
}