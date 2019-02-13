#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <string>

namespace Graphics
{
    namespace Window
    {
        struct State
        {
            std::string Title;
            int Width;
            int Height;
            int Bits;
            bool Fullscreen;

            CAERULUS_GRAPHICS State(std::string title = "", int x = 800, int y = 600, int b = 32, bool f = false) :
                Title(title),
                Width(x),
                Height(y),
                Bits(b == 0 ? 32 : b),
                Fullscreen(f)
            {
            }

            CAERULUS_GRAPHICS State& operator= (const State& state)
            {
                Title = state.Title;
                Width = state.Width;
                Height = state.Height;
                Bits = state.Bits;
                Fullscreen = state.Fullscreen;
                return *this;
            }

            CAERULUS_GRAPHICS bool Compare(const State& state) const
            {
                return
                    Title == state.Title &&
                    Width == state.Width &&
                    Height == state.Height &&
                    Bits == state.Bits &&
                    Fullscreen == state.Fullscreen;
            }
        };

        class Window
        {
        public:
            CAERULUS_GRAPHICS Window();
            CAERULUS_GRAPHICS virtual ~Window();

            CAERULUS_GRAPHICS virtual const State& GetActiveState() const;
            CAERULUS_GRAPHICS virtual void Set(const std::string& title, int x, int y, int bits, bool fullscreen = false);

            CAERULUS_GRAPHICS virtual void Apply() = 0;
            CAERULUS_GRAPHICS virtual void Focus() = 0;
            CAERULUS_GRAPHICS virtual void SwapBuffer() const = 0;
            CAERULUS_GRAPHICS virtual void* GetHandle() const = 0;

        protected:
            CAERULUS_GRAPHICS virtual bool Compare(const State& state) const;

            State m_ActiveState;
            State m_NewState;
        };
    }
}
