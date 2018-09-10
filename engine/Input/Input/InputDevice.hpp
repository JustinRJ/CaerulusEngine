#pragma once

#define CAERULUS_INPUT __declspec(dllexport)

#include "Mapping.h"
#include "Controller.hpp"

#include <map>
#include <iostream>

namespace Core
{
    namespace Input
    {
        class InputDevice
        {
        private:

            class InputCommand
            {
            public:

                CAERULUS_INPUT InputCommand(InputDevice& input, const std::string& id);
                CAERULUS_INPUT virtual ~InputCommand();

                template <typename T>
                CAERULUS_INPUT InputCommand& Set(const std::function<void(T)>& func, State state = State::STATE_PRESS)
                {
                    return Set(BindingT<T>::Create(func, state));
                }

                CAERULUS_INPUT InputCommand& Set(const std::function<void()>& func, State state = State::STATE_PRESS);
                CAERULUS_INPUT InputCommand& Set(std::unique_ptr<Binding> binding);
                CAERULUS_INPUT InputCommand& Unset(bool complete = false);

                CAERULUS_INPUT InputCommand& Unbind(const Key key);
                CAERULUS_INPUT InputCommand& Unbind(const MouseButton key);

                CAERULUS_INPUT InputCommand& Bind(const Key key);
                CAERULUS_INPUT InputCommand& Bind(const MouseButton key);

            private:

                InputDevice& m_Input;
                std::string m_ID;
            };

        public:
            CAERULUS_INPUT InputDevice();
            CAERULUS_INPUT virtual ~InputDevice();

            CAERULUS_INPUT virtual void Initialize(void* hwnd);
            CAERULUS_INPUT virtual void Uninitialize();

            CAERULUS_INPUT virtual void Update();

            CAERULUS_INPUT virtual void SetBinding(const std::string& mapping, std::unique_ptr<Binding> binding);
            CAERULUS_INPUT virtual void SetBinding(const std::string& mapping, std::unique_ptr<Binding> binding, const Key key);
            CAERULUS_INPUT virtual void SetBinding(const std::string& mapping, std::unique_ptr<Binding> binding, const MouseButton key);

            CAERULUS_INPUT virtual void AddControl(const std::string& mapping, const Key key);
            CAERULUS_INPUT virtual void AddControl(const std::string& mapping, const MouseButton key);

            CAERULUS_INPUT virtual void RemoveBinding(const std::string& mapping, bool complete = false);

            CAERULUS_INPUT virtual void RemoveControl(const std::string& mapping, const Key key);
            CAERULUS_INPUT virtual void RemoveControl(const std::string& mapping, const MouseButton key);

            CAERULUS_INPUT virtual bool GetKey(const Key key) const;
            CAERULUS_INPUT virtual bool GetMouseButton(const MouseButton button) const;
            CAERULUS_INPUT virtual glm::vec2 GetMouseMovement(int x, int y) const;
            CAERULUS_INPUT virtual glm::vec2 GetMousePosition(int x, int y) const;

            CAERULUS_INPUT virtual bool Invoke(const std::string& name);

            CAERULUS_INPUT virtual void PrintCommands() const;

            CAERULUS_INPUT InputCommand Command(const std::string& mapping);

        protected:

            std::map<Controllers, std::shared_ptr<Controller>> m_Controllers;

        private:

            std::shared_ptr<Controller> GetKeyboard() const;
            std::shared_ptr<Controller> GetMouse() const;

            std::shared_ptr<Control> GetControl(const Key key) const;
            std::shared_ptr<Control> GetControl(const MouseButton key) const;

            std::shared_ptr<Mapping> GetMapping(const std::string& mapping, bool create = true);
            void RemoveMapping(const std::string& mapping);

            std::map<std::string, std::shared_ptr<Mapping>> m_Mappings;
        };
    }
}