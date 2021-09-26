#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <string>

#include "Interface/NonCopyable.h"

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

        class CAERULUS_GRAPHICS IWindow : Core::Interface::NonCopyable
        {
        public:
            virtual ~IWindow() = default;

            virtual void Update() = 0;
            virtual void Apply() = 0;
            virtual void Focus() = 0;
            virtual void SwapBuffer() const = 0;
            virtual void* GetHandle() const = 0;

            virtual void Set(const std::string& title, int x, int y, int bits, bool fullscreen)
            {
                m_newState = State(title, x, y, bits, fullscreen);
            }

            virtual const State& GetActiveState() const
            {
                return m_activeState;
            }

        protected:
            virtual bool Compare(const State& state) const
            {
                return m_activeState.Compare(m_newState);
            }

            State m_activeState;
            State m_newState;
        };
    }
}