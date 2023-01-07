#pragma once

#include "InputDefines.h"

#include <vector>
#include <memory>
#include <string>
#include <functional>

#include "Interface/ITickable.h"

namespace Graphics
{
    namespace Window
    {
        class GLWindow;
    }
}

namespace Core
{
    namespace Input
    {
        struct DragData
        {
            double X = 0;
            double Y = 0;
            double DeltaX = 0;
            double DeltaY = 0;
        };

        struct DragBinding
        {
            std::function<void(const DragData& data)> Callback = {};
            std::string Name = "";
        };

        class MouseInputSystem : public Interface::ITickable
        {
        public:
            MouseInputSystem(Graphics::Window::GLWindow& window) :
                m_window(window)
            {
                glfwSetInputMode(m_window.GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            };

            void EarlyTick() override
            {
                UpdateMouseDrag();
            }

            void AddDragMouseCallback(std::function<void(const DragData&)> callback, std::string_view name = "")
            {
                DragBinding dragBinding;
                dragBinding.Callback = callback;
                dragBinding.Name = name;

                m_dragBindingMap.push_back(dragBinding);
            }

        private:
            void UpdateMouseDrag()
            {
                double xpos, ypos;
                glfwGetCursorPos(m_window.GetGLFWWindow(), &xpos, &ypos);
                const auto& activeState = m_window.GetActiveState();
                double deltaX = xpos - (activeState.Width * 0.5);
                double deltaY = ypos - (activeState.Height * 0.5);

                for (const DragBinding& dragBinding : m_dragBindingMap)
                {
                    dragBinding.Callback({ xpos, ypos, deltaX, deltaY });
                }
            }

            std::vector<DragBinding> m_dragBindingMap;
            Graphics::Window::GLWindow& m_window;
        };
    }
}
