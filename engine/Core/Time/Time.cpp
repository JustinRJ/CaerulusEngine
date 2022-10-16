#include "stdafx.h"

#include "Time/Time.h"

namespace Core
{
    namespace Time
    {
        tm InternalTime;

        std::string Time::GetTime(std::string_view format)
        {
            size_t length = strlen(format.data());
            char* buffer = new char[length];
            strftime(buffer, length, format.data(), &InternalTime);
            std::string outFormat(buffer);
            delete[] buffer;

            return outFormat;
        }

        std::string Time::GetLocalTime(std::string_view format)
        {
            time_t rawTime;
            time(&rawTime);
            localtime_s(&InternalTime, &rawTime);

            char buffer[200];
            strftime(buffer, 200, format.data(), &InternalTime);

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
