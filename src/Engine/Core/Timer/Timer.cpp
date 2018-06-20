#include "stdafx.h"

#include "Timer.h"
#include <windows.h>

namespace Core
{
    namespace Timer
    {
        void SleepMilli(const TimeUnits milli)
        {
            ::Sleep((DWORD)milli);
        }
        void Sleep(const TimeUnits sec)
        {
            ::Sleep((DWORD)sec * 1000);
        }
        void Sleep(const Time sec)
        {
            SleepMilli((TimeUnits)(sec * 1000));
        }

        std::string CurrentTime(const char* format/* = "%H:%M:%S"*/)
        {
            time_t rawtime;
            time(&rawtime);

            struct tm timeinfo;
            localtime_s(&timeinfo, &rawtime);

            char buf[200];
            strftime(buf, 200, format, &timeinfo);

            return std::string(buf);
        }

        std::string CurrentDate()
        {
            return CurrentTime("%Y-%m-%d");
        }

        std::string CurrentDateTime()
        {
            return CurrentTime("%Y-%m-%d_%H-%M-%S");
        }

        TimeValue::TimeValue(bool update)
        {
            if (update)
            {
                Update();
            }
        }

        TimeValue::~TimeValue()
        {
        }

        void TimeValue::Update()
        {
            time(&m_Time);
        }

        std::string TimeValue::Format(const char* format, size_t extralength) const
        {
            size_t len = strlen(format) + extralength;

            char* buf = new char[len];

            strftime(buf, len, format, &m_TimeLocal);

            std::string ret(buf);

            delete[] buf;

            return ret;
        }

        TimeUnits Timer::NanoTime()
        {
            LARGE_INTEGER time;
            QueryPerformanceCounter(&time);
            return (TimeUnits)time.QuadPart;
        }

        TimeUnits Timer::Frequency()
        {
            LARGE_INTEGER frequency;
            QueryPerformanceFrequency(&frequency);
            return (TimeUnits)frequency.QuadPart;
        }

        Timer::Timer(bool start) :
            m_Running(false),
            m_DeltaTime(0.0f),
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
                m_Frequency = Frequency();
                m_Multiplier = 1 / (Time)m_Frequency;
                m_DeltaTime = 0.0f;
                m_TimeLastFrame = NanoTime();
                StartTime.Update();
                m_Running = true;
            }
        }

        Time Timer::Total()
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

        Time Timer::Delta()
        {
            if (m_Running)
            {
                TimeUnits currentTime = NanoTime();
                TimeUnits diffTime = currentTime - m_TimeLastFrame;
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
    }
}