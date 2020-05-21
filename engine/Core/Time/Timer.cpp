#include "stdafx.h"
#include "Timer.h"

namespace Core
{
    namespace Time
    {
        void SleepMilli(time_t milli)
        {
            ::Sleep(static_cast<DWORD>(milli));
        }
        void Sleep(time_t sec)
        {
            ::Sleep(static_cast<DWORD>(sec * 1000.0));
        }
        void Sleep(Time sec)
        {
            SleepMilli(static_cast<time_t>(sec.GetTime() * 1000.0));
        }

        Timer::Timer(bool start) :
            m_running(false),
            m_deltaTime(0.0f),
            m_multiplier(0.0f),
            m_frequency(0),
            m_timeLastFrame(0),
            StartTime(false),
            EndTime(false)
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
                m_deltaTime = 0.0f;
                m_frequency = Frequency();
                m_multiplier = (1.0f / m_frequency);
                m_timeLastFrame = NanoTime();
                StartTime.Update();
            }
        }

        float Timer::Total()
        {
            if (m_running)
            {
                m_deltaTime = (NanoTime() - m_timeLastFrame) * m_multiplier;
            }
            else
            {
                Start();
            }

            return m_deltaTime;
        }

        float Timer::Delta()
        {
            if (m_running)
            {
                time_t currentTime = NanoTime();
                time_t diffTime = currentTime - m_timeLastFrame;
                m_timeLastFrame = currentTime;

                m_deltaTime = diffTime * m_multiplier;
            }
            else
            {
                Start();
            }

            return m_deltaTime;
        }

        void Timer::Stop()
        {
            if (m_running)
            {
                EndTime.Update();
                m_running = false;
            }
        }

        time_t Timer::NanoTime()
        {
            LARGE_INTEGER time;
            QueryPerformanceCounter(&time);
            return static_cast<time_t>(time.QuadPart);
        }

        time_t Timer::Frequency()
        {
            LARGE_INTEGER frequency;
            QueryPerformanceFrequency(&frequency);
            return static_cast<time_t>(frequency.QuadPart);
        }
    }
}