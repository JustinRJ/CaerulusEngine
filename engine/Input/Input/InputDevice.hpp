#pragma once



#define CAERULUS_INPUT __declspec(dllexport)

#include "Mapping.h"
#include "Controller.hpp"
#include "../../Core/Interface/ITickable.h"

#include <map>
#include <iostream>

namespace Input
{
    class CAERULUS_INPUT InputDevice : public Core::Interface::ITickable
    {
    public:

        class CAERULUS_INPUT InputCommand
        {
        public:

            InputCommand(InputDevice& input, const std::string& id);
            virtual ~InputCommand();

            template <typename T>
            InputCommand& Set(const std::function<void(T)>& func, State state = State::STATE_PRESS)
            {
                return Set(BindingT<T>::Create(func, state));
            }

            InputCommand& Set(const std::function<void()>& func, State state = State::STATE_PRESS);
            InputCommand& Set(std::unique_ptr<Binding> binding);
            InputCommand& Unset(bool complete = false);

            InputCommand& Unbind(const Key key);
            InputCommand& Unbind(const MouseButton key);

            InputCommand& Bind(const Key key);
            InputCommand& Bind(const MouseButton key);

        private:

            InputDevice& m_Input;
#pragma warning(push)
#pragma warning( disable : 4251)
            std::string m_ID;
#pragma warning(pop)
        };

        InputDevice();
        virtual ~InputDevice();

        virtual void Initialize(void* hwnd);
        virtual void Uninitialize();

        virtual void Update(float deltaTime) override;
        virtual void FixedUpdate(float fixedTime) override {};
        virtual void LateUpdate(float deltaTime) override {};
        virtual void Reset() override {};

        virtual void SetBinding(const std::string& mapping, std::unique_ptr<Binding> binding);
        virtual void SetBinding(const std::string& mapping, std::unique_ptr<Binding> binding, const Key key);
        virtual void SetBinding(const std::string& mapping, std::unique_ptr<Binding> binding, const MouseButton key);

        virtual void AddControl(const std::string& mapping, const Key key);
        virtual void AddControl(const std::string& mapping, const MouseButton key);

        virtual void RemoveBinding(const std::string& mapping, bool complete = false);

        virtual void RemoveControl(const std::string& mapping, const Key key);
        virtual void RemoveControl(const std::string& mapping, const MouseButton key);

        virtual bool GetKey(const Key key) const;
        virtual bool GetMouseButton(const MouseButton button) const;
        virtual glm::vec2 GetMouseMovement(int x, int y) const;
        virtual glm::vec2 GetMousePosition(int x, int y) const;

        virtual bool Invoke(const std::string& name);

        virtual void PrintCommands() const;

        InputCommand Command(const std::string& mapping);

    protected:
#pragma warning(push)
#pragma warning( disable : 4251)
        std::map<Controllers, std::shared_ptr<Controller>> m_Controllers;
#pragma warning(pop)
    private:

        std::shared_ptr<Controller> GetKeyboard() const;
        std::shared_ptr<Controller> GetMouse() const;

        std::shared_ptr<Control> GetControl(const Key key) const;
        std::shared_ptr<Control> GetControl(const MouseButton key) const;

        std::shared_ptr<Mapping> GetMapping(const std::string& mapping, bool create = true);
        void RemoveMapping(const std::string& mapping);
#pragma warning(push)
#pragma warning( disable : 4251)
        std::map<std::string, std::shared_ptr<Mapping>> m_Mappings;
#pragma warning(pop)
    };
}

