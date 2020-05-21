#include "stdafx.h"

#include "Time.h"

namespace Core
{
    namespace Time
    {
        tm InternalTime;

        std::string GetTime(const std::string& format, size_t extralength)
        {
            size_t length = strlen(format.c_str()) + extralength;
            char* buffer = new char[length];
            strftime(buffer, length, format.c_str(), &InternalTime);
            std::string outFormat(buffer);
            delete[] buffer;

            return outFormat;
        }

        std::string GetLocalTime(const std::string& format)
        {
            time_t rawTime;
            time(&rawTime);
            localtime_s(&InternalTime, &rawTime);

            char buffer[200];
            strftime(buffer, 200, format.c_str(), &InternalTime);

            return std::string(buffer);
        }

        Time::Time(bool update) :
            m_time(0)
        {
            if (update)
            {
                Update();
            }
        }

        void Time::Update()
        {
            time(&m_time);
        }

        time_t Time::GetTime() const
        {
            return m_time;
        }

        void Time::Increment(time_t time)
        {
            m_time += time;
        }
    }
}
