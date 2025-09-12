#pragma once
#include <chrono>
#include <thread>
#include <cstdint>

namespace Core::Time
{
    class EngineTimer
    {
    public:
        EngineTimer(double fixedStep, double maxFPS = 0.0)
            : m_fixedStep(fixedStep),
            m_maxFPS(maxFPS),
            m_lastFrame(Clock::now()),
            m_fpsTimer(m_lastFrame)
        {
        }

        void Tick()
        {
            using namespace std::chrono;

            auto now = Clock::now();
            m_deltaTime = duration<double>(now - m_lastFrame).count();

            // --- FPS limiting (high precision) ---
            if (m_maxFPS > 0.0)
            {
                double targetFrameTime = 1.0 / m_maxFPS;

                // Sleep for most of the remaining time
                while (m_deltaTime < targetFrameTime)
                {
                    double timeLeft = targetFrameTime - m_deltaTime;

                    if (timeLeft > 0.002) // sleep if more than ~2ms
                    {
                        std::this_thread::sleep_for(duration<double>(timeLeft - 0.001));
                    }
                    // Busy-wait the last 1ms
                    now = Clock::now();
                    m_deltaTime = duration<double>(now - m_lastFrame).count();
                }
            }

            m_lastFrame = Clock::now();

            // --- Fixed step calculation ---
            m_accumulatedTime += m_deltaTime;
            m_steps = 0;
            while (m_accumulatedTime >= m_fixedStep)
            {
                m_accumulatedTime -= m_fixedStep;
                m_steps++;
            }

            m_alpha = m_accumulatedTime / m_fixedStep;

            // --- FPS counter ---
            m_frameCount++;
            auto fpsDiff = duration<double>(m_lastFrame - m_fpsTimer).count();
            if (fpsDiff >= 1.0)
            {
                m_fps = static_cast<uint32_t>(m_frameCount / fpsDiff);
                m_frameCount = 0;
                m_fpsTimer = m_lastFrame;
            }
        }

        // --- Accessors ---
        double GetDelta() const { return m_deltaTime; }
        uint32_t GetFixedSteps() const { return m_steps; }
        double GetAlpha() const { return m_alpha; }
        uint32_t GetFPS() const { return m_fps; }

    private:
        using Clock = std::chrono::high_resolution_clock;

        // Settings
        double m_fixedStep;   // fixed physics step in seconds
        double m_maxFPS;      // maximum FPS (0 = uncapped)

        // Timing
        Clock::time_point m_lastFrame;
        Clock::time_point m_fpsTimer;

        double m_deltaTime = 0.0;
        double m_accumulatedTime = 0.0;
        uint32_t m_steps = 0;
        double m_alpha = 0.0;

        // FPS counter
        uint32_t m_fps = 0;
        uint32_t m_frameCount = 0;
    };
}
