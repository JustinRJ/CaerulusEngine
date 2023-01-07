#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include <chrono>

namespace Core
{
    namespace Time
    {
        class CAERULUS_CORE Timer
        {
        public:
            Timer(bool start = false);
            virtual ~Timer() = default;

            void Start();
            void Stop();

            float Delta();

            static void Sleep(time_t sec);

        private:
            bool m_running = false;
            std::chrono::high_resolution_clock::time_point m_lastTimePoint;
        };
    }
}
