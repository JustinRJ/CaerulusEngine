#include "stdafx.h"

#include "InputDevice.hpp"

namespace Input
{
    InputDevice::InputDevice()
    {
    }

    InputDevice::~InputDevice()
    {
    }

    void InputDevice::Initialize(void* hwnd)
    {
    }

    void InputDevice::Uninitialize()
    {
        m_Mappings.clear();
        m_Controllers.clear();
    }

    void InputDevice::Update(float deltaTime)
    {
        for (auto it = m_Controllers.begin(); it != m_Controllers.cend(); it++)
        {
            it->second->Update();
        }
        for (auto it = m_Mappings.begin(); it != m_Mappings.cend(); it++)
        {
            it->second->Update();
        }
    }

    void InputDevice::RemoveBinding(const std::string& mapping, bool complete)
    {
        if (complete)
        {
            RemoveMapping(mapping);
        }
        else
        {
            std::shared_ptr<Mapping> m = GetMapping(mapping, false);
            if (m)
            {
                m->SetBinding(nullptr);
            }
        }
    }

    void InputDevice::SetBinding(const std::string& mapping, std::unique_ptr<Binding> binding)
    {
        GetMapping(mapping)->SetBinding(std::move(binding));
    }

    void InputDevice::SetBinding(const std::string& mapping, std::unique_ptr<Binding> binding, const Key key)
    {
        SetBinding(mapping, std::move(binding));
        AddControl(mapping, key);
    }

    void InputDevice::SetBinding(const std::string& mapping, std::unique_ptr<Binding> binding, const MouseButton key)
    {
        SetBinding(mapping, std::move(binding));
        AddControl(mapping, key);
    }

    void InputDevice::RemoveControl(const std::string& mapping, const Key key)
    {
        std::shared_ptr<Control> control = GetControl(key);
        GetMapping(mapping)->RemoveControl(control);
    }
    void InputDevice::RemoveControl(const std::string& mapping, const MouseButton key)
    {
        std::shared_ptr<Control> control = GetControl(key);
        GetMapping(mapping)->RemoveControl(control);
    }

    void InputDevice::AddControl(const std::string& mapping, const Key key)
    {
        std::shared_ptr<Control> control = GetControl(key);
        GetMapping(mapping)->AddControl(control);
    }
    void InputDevice::AddControl(const std::string& mapping, const MouseButton key)
    {
        std::shared_ptr<Control> control = GetControl(key);
        GetMapping(mapping)->AddControl(control);
    }

    bool InputDevice::GetKey(const Key key) const
    {
        return GetKeyboard()->GetControlValue<bool>(static_cast<int>(key));
    }

    bool InputDevice::GetMouseButton(const MouseButton button) const
    {
        return GetMouse()->GetControlValue<bool>(static_cast<int>(button));
    }

    glm::vec2 InputDevice::GetMouseMovement(int x, int y) const
    {
        glm::vec2 xy = GetMouse()->GetControlValue<glm::vec2>(static_cast<int>(MouseButton::MOUSE_XY_DELTA));
        return glm::vec2(static_cast<int>(xy.x), static_cast<int>(xy.y));
    }

    glm::vec2 InputDevice::GetMousePosition(int x, int y) const
    {
        glm::vec2 xy = GetMouse()->GetControlValue<glm::vec2>(static_cast<int>(MouseButton::MOUSE_XY));
        return glm::vec2(static_cast<int>(xy.x), static_cast<int>(xy.y));
    }

    bool InputDevice::Invoke(const std::string& name)
    {
        if (m_Mappings.at(name))
        {
            return m_Mappings.at(name)->Invoke();
        }
        return false;
    }

    void InputDevice::PrintCommands() const
    {
        std::cout << "Input Mappings:" << std::endl;
        for (auto it = m_Mappings.begin(); it != m_Mappings.cend(); it++)
        {
            std::cout << it->second->ToString() << std::endl;
        }
    }

    std::shared_ptr<Controller> InputDevice::GetKeyboard() const
    {
        return m_Controllers.at(Controllers::KEYBOARD);
    }

    std::shared_ptr<Controller> InputDevice::GetMouse() const
    {
        return m_Controllers.at(Controllers::MOUSE);
    }

    std::shared_ptr<Control> InputDevice::GetControl(const Key key) const
    {
        return GetKeyboard()->GetControl(key);
    }

    std::shared_ptr<Control> InputDevice::GetControl(const MouseButton key) const
    {
        return GetMouse()->GetControl(key);
    }

    std::shared_ptr<Mapping> InputDevice::GetMapping(const std::string& mapping, bool create)
    {
        auto it = m_Mappings.find(mapping);
        if (create)
        {
            if (it != m_Mappings.cend())
            {
                return it->second;
            }
            else
            {
                m_Mappings[mapping] = std::make_shared<Mapping>(mapping);
                return m_Mappings[mapping];
            }
        }
        else
        {
            return (it != m_Mappings.cend()) ? it->second : nullptr;
        }
    }

    void InputDevice::RemoveMapping(const std::string& mapping)
    {
        m_Mappings.erase(mapping);
    }

    InputDevice::InputCommand InputDevice::Command(const std::string& mapping)
    {
        return InputCommand(*this, mapping);
    }

    InputDevice::InputCommand::InputCommand(InputDevice& input, const std::string& id) :
        m_Input(input),
        m_ID(id)
    {
    }

    InputDevice::InputCommand::~InputCommand()
    {
    }

    InputDevice::InputCommand& InputDevice::InputCommand::Set(const std::function<void()>& func, State state)
    {
        return Set<bool>([func](bool) { func(); }, state);
    }

    InputDevice::InputCommand& InputDevice::InputCommand::Set(std::unique_ptr<Binding> binding)
    {
        m_Input.SetBinding(m_ID, std::move(binding));
        return *this;
    }

    InputDevice::InputCommand& InputDevice::InputCommand::Unset(bool complete)
    {
        m_Input.RemoveBinding(m_ID, complete);
        return *this;
    }

    InputDevice::InputCommand& InputDevice::InputCommand::Bind(const Key key)
    {
        m_Input.AddControl(m_ID, key);
        return *this;
    }

    InputDevice::InputCommand& InputDevice::InputCommand::Bind(const MouseButton key)
    {
        m_Input.AddControl(m_ID, key);
        return *this;
    }

    InputDevice::InputCommand& InputDevice::InputCommand::Unbind(const Key key)
    {
        m_Input.RemoveControl(m_ID, key);
        return *this;
    }

    InputDevice::InputCommand& InputDevice::InputCommand::Unbind(const MouseButton key)
    {
        m_Input.RemoveControl(m_ID, key);
        return *this;
    }
}
