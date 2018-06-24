#include "stdafx.h"
#include "Timer.h"

namespace Core
{
    namespace Time
    {
        extern tm InternalTime;

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
            m_Running(false),
            m_DeltaTime(0.0f),
            m_Multiplier(0.0f),
            m_Frequency(0),
            m_TimeLastFrame(0),
            StartTime(false),
            EndTime(false)
        {
            if (start)
            {
                Start();
            }
        }

        Timer::~Timer()
        {
        }

        void Timer::Start()
        {
            if (m_Running == false)
            {
                m_Running = true;
                m_DeltaTime = 0.0f;
                m_Frequency = Frequency();
                m_Multiplier = (1.0f / m_Frequency);
                m_TimeLastFrame = NanoTime();
                StartTime.Update();
            }
        }

        float Timer::Total()
        {
            if (m_Running)
            {
                m_DeltaTime = (NanoTime() - m_TimeLastFrame) * m_Multiplier;
            }
            else
            {
                Start();
            }

            return m_DeltaTime;
        }

        float Timer::Delta()
        {
            if (m_Running)
            {
                time_t currentTime = NanoTime();
                time_t diffTime = currentTime - m_TimeLastFrame;
                m_TimeLastFrame = currentTime;

                m_DeltaTime = diffTime * m_Multiplier;
            }
            else
            {
                Start();
            }

            return m_DeltaTime;
        }

        void Timer::Stop()
        {
            if (m_Running)
            {
                EndTime.Update();
                m_Running = false;
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