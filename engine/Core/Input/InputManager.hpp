#pragma once

#include <glfw3.h>
#include <functional>
#include <iostream>
#include <map>
#include "../Interface/ITickable.h"

namespace Core
{
    namespace Input
    {
        // Window -> Key -> Action -> Mode -> Callback
        typedef std::map<GLFWwindow*, std::map<int, std::map<int, std::map<int, std::function<void(void)>>>>> KeyInputCallbackMap;

        class InputManager : public Interface::ITickable
        {
        public:

            InputManager() {};
            virtual ~InputManager(){};

            virtual void Reset() override
            {
                m_WindowKeyCallbacks.clear();
            }

            virtual void PreUpdate() override
            {
                glfwPollEvents();
            };

            virtual void Update(float deltaTime) override {}

            virtual void FixedUpdate(float fixedTime) override {};

            virtual void PostUpdate() override {};

            void AddWindowKeyCallback(GLFWwindow* window, int key, int action, int mode, std::function<void(void)> callback)
            {
                m_WindowKeyCallbacks[window][key][action][mode] = callback;
                glfwSetWindowUserPointer(window, this);
                glfwSetKeyCallback(window, [](GLFWwindow* windowI, int keyI, int scancodeI, int actionI, int modeI)
                {
                    auto& self = *static_cast<InputManager*>(glfwGetWindowUserPointer(windowI));
                    KeyInputCallbackMap keyInputCallMap = self.GetWindowKeyCallbacks();
                    try
                    {
                        if (keyInputCallMap.find(windowI) != keyInputCallMap.end() &&
                            keyInputCallMap.at(windowI).find(keyI) != keyInputCallMap.at(windowI).end() &&
                            keyInputCallMap.at(windowI).at(keyI).find(actionI) != keyInputCallMap.at(windowI).at(keyI).end() &&
                            keyInputCallMap.at(windowI).at(keyI).at(actionI).find(modeI) != keyInputCallMap.at(windowI).at(keyI).at(actionI).end())
                        {
                            keyInputCallMap[windowI][keyI][actionI][modeI]();
                        }
                    }
                    catch(...)
                    {
                        std::cerr << "Error in Caerulus Core!" << std::endl;
                    }
                });
            }

        private:

            KeyInputCallbackMap m_WindowKeyCallbacks;
            const KeyInputCallbackMap& GetWindowKeyCallbacks() const
            {
                return m_WindowKeyCallbacks;
            }
        };
    }
}
