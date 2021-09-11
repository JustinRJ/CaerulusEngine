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

        class MouseInputManager : public Interface::ITickable
        {
        public:
            MouseInputManager(std::shared_ptr<Graphics::Window::GLWindow> window) :
                m_window(window)
            {
                glfwSetInputMode(m_window->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            };

            void Reset() override {}

            void PreUpdate(float deltaTime) override
            {
                UpdateMouseDrag();
            }

            void Update(float deltaTime) override {}

            void FixedUpdate(float fixedTime) override {}

            void AddDragMouseCallback(std::shared_ptr<Graphics::Window::GLWindow> window, std::function<void(const DragData&)> callback, const std::string& name = "")
            {
                DragBinding dragBinding;
                dragBinding.Callback = callback;
                dragBinding.Name = name;

                m_dragBindingMap.push_back(dragBinding);
            }

            const DragData& GetDragData() const
            {
                return m_dragData;
            }

        private:
            void UpdateMouseDrag()
            {
                double xpos, ypos;
                glfwGetCursorPos(m_window->GetGLFWWindow(), &xpos, &ypos);
                double deltaX = xpos - (m_window->GetActiveState().Width * 0.5);
                double deltaY = ypos - (m_window->GetActiveState().Height * 0.5);

                for (const DragBinding& dragBinding : m_dragBindingMap)
                {
                    dragBinding.Callback({ xpos, ypos, deltaX, deltaY });
                }
            }

            DragData m_dragData;
            std::vector<DragBinding> m_dragBindingMap;
            std::shared_ptr<Graphics::Window::GLWindow> m_window;
        };
    }
}
