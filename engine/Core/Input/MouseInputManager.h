#pragma once

#include "InputDefines.h"
#include <string>
#include <functional>
#include "../Interface/ITickable.h"

namespace Core
{
    namespace Input
    {
        class Graphics::Window::GLWindow;

        struct DragData
        {
            double X = 0;
            double Y = 0;
            double DeltaX = 0;
            double DeltaY = 0;
        };

        struct DragBinding
        {
            std::function<void(DragData data)> Callback = {};
            std::string Name = "";
        };

        using namespace Graphics::Window;
        class MouseInputManager : public Interface::ITickable
        {
        public:

            MouseInputManager(std::shared_ptr<GLWindow> window) :
                m_window(window)
            {
                glfwSetInputMode(m_window->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            };

            virtual ~MouseInputManager() = default;

            void Reset() override
            {
            }

            void PreUpdate(float deltaTime) override
            {
                UpdateMouseDrag();
            }

            void Update(float deltaTime) override {}

            void FixedUpdate(float fixedTime) override {}

            void AddDragMouseCallback(std::shared_ptr<GLWindow> window, std::function<void(DragData)> callback, std::string name = "")
            {
                DragBinding dragBinding;
                dragBinding.Callback = callback;
                dragBinding.Name = name;

                m_dragBindingMap.push_back(dragBinding);
            }

            DragData GetDragData() const
            {
                return m_dragData;
            }

        private:
            DragData m_dragData;
            std::vector<DragBinding> m_dragBindingMap;
            std::shared_ptr<GLWindow> m_window;

            void UpdateMouseDrag()
            {
                double xpos, ypos;
                glfwGetCursorPos(m_window->GetGLFWWindow(), &xpos, &ypos);
                double deltaX = xpos - (m_window->GetActiveState().Width / 2);
                double deltaY = ypos - (m_window->GetActiveState().Height / 2);

                for (auto& dragBindingPair : m_dragBindingMap)
                {
                    dragBindingPair.Callback(DragData({ xpos, ypos, deltaX, deltaY }));
                }
            }
        };
    }
}
