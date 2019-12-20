#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <string>

namespace Graphics
{
    namespace Window
    {
        struct CAERULUS_GRAPHICS State
        {
            std::string Title;
            int Width;
            int Height;
            int Bits;
            bool Fullscreen;

            State(std::string title = "", int x = 800, int y = 600, int b = 32, bool f = false) :
                Title(title),
                Width(x),
                Height(y),
                Bits(b == 0 ? 32 : b),
                Fullscreen(f)
            {
            }

            State& operator= (const State& state)
            {
                Title = state.Title;
                Width = state.Width;
                Height = state.Height;
                Bits = state.Bits;
                Fullscreen = state.Fullscreen;
                return *this;
            }

            bool Compare(const State& state) const
            {
                return
                    Title == state.Title &&
                    Width == state.Width &&
                    Height == state.Height &&
                    Bits == state.Bits &&
                    Fullscreen == state.Fullscreen;
            }
        };

        class CAERULUS_GRAPHICS Window
        {
        public:
            Window();
            virtual ~Window() = default;

            virtual const State& GetActiveState() const;
            virtual void Set(const std::string& title, int x, int y, int bits, bool fullscreen = false);

            virtual void Apply() = 0;
            virtual void Focus() = 0;
            virtual void SwapBuffer() const = 0;
            virtual void* GetHandle() const = 0;

        protected:
            virtual bool Compare(const State& state) const;

            State m_ActiveState;
            State m_NewState;
        };
    }
}