#pragma once

#include "InputDefines.h"
#include <functional>
#include <string>
#include "../Interface/ITickable.h"

namespace Core
{
    namespace Input
    {
        struct DragData
        {
            int X = 0;
            int Y = 0;
        };

        struct DragBinding
        {
            std::function<void(DragData data)> Callback = {};
            std::string Name = "";
        };

        class MouseInputManager : public Interface::ITickable
        {
        public:

            MouseInputManager(GLFWwindow* window) :
                m_DragBindingMap(*new std::vector<DragBinding*>()),
                m_DragData(*new DragData()),
                m_Window(window)
            {
            };

            virtual ~MouseInputManager() {};

            virtual void Reset() override
            {
            }

            virtual void Update(float deltaTime) override
            {
            }

            void AddDragMouseCallback(GLFWwindow* window, std::function<void(DragData)> callback, std::string name = "")
            {
                DragBinding dragBinding;
                dragBinding.Callback = callback;
                dragBinding.Name = name;

                m_DragBindingMap.push_back(std::make_unique<DragBinding>(dragBinding).release());
            }


            virtual void FixedUpdate(float fixedTime) override {};

            virtual void LateUpdate(float deltaTime) override {};

        private:
            DragData& m_DragData;
            std::vector<DragBinding*>& m_DragBindingMap;

            GLFWwindow* m_Window;
        };
    }
}
