#pragma once

#include "InputDefines.h"
#include <functional>
#include <string>
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

        class MouseInputManager : public Interface::ITickable
        {
        public:

            MouseInputManager(Graphics::Window::GLWindow* window) :
                m_Window(window)
            {
                glfwSetInputMode(m_Window->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            };

            virtual ~MouseInputManager() {};

            virtual void Reset() override
            {
            }

            virtual void Update(float deltaTime) override
            {
                UpdateMouseDrag();
            }

            void AddDragMouseCallback(Graphics::Window::GLWindow* window, std::function<void(DragData)> callback, std::string name = "")
            {
                DragBinding dragBinding;
                dragBinding.Callback = callback;
                dragBinding.Name = name;

                m_DragBindingMap.push_back(std::make_unique<DragBinding>(dragBinding).release());
            }

            virtual void FixedUpdate(float fixedTime) override {};

            virtual void LateUpdate(float deltaTime) override {};

        private:
            DragData m_DragData;
            std::vector<DragBinding*> m_DragBindingMap;
            Graphics::Window::GLWindow* m_Window;

            void UpdateMouseDrag()
            {
                double xpos, ypos;
                glfwGetCursorPos(m_Window->GetGLFWWindow(), &xpos, &ypos);
                double deltaX = xpos - (m_Window->GetActiveState().Width / 2);
                double deltaY = ypos - (m_Window->GetActiveState().Height / 2);

                for (auto& dragBindingPair : m_DragBindingMap)
                {
                    dragBindingPair->Callback(DragData({ xpos, ypos, deltaX, deltaY }));
                }
            }
        };
    }
}
