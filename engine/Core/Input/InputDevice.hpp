#pragma once

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
        public:

            InputDevice()
            {
            }

            virtual ~InputDevice()
            {
            }

            virtual void Initialize(void* hwnd)
            {
            }

            virtual void Uninitialize()
            {
                for (std::map<std::string, Mapping*>::iterator it = m_Mappings.begin(); it != m_Mappings.cend(); it++)
                {
                    delete it->second;
                }
                m_Mappings.clear();
                for (std::map<Controllers, Controller*>::iterator it = m_Controllers.begin(); it != m_Controllers.cend(); it++)
                {
                    delete it->second;
                }
                m_Controllers.clear();
            }

            virtual void Update()
            {
                for (std::map<Controllers, Controller*>::iterator it = m_Controllers.begin(); it != m_Controllers.cend(); it++)
                {
                    it->second->Update();
                }
                for (std::map<std::string, Mapping*>::iterator it = m_Mappings.begin(); it != m_Mappings.cend(); it++)
                {
                    it->second->Update();
                }
            }

            virtual void RemoveBinding(const std::string& mapping, bool complete = false)
            {
                if (complete)
                {
                    RemoveMapping(mapping);
                }
                else
                {
                    Mapping* m = GetMapping(mapping, false);
                    if (m != nullptr)
                    {
                        m->SetBinding(nullptr);
                    }
                }
            }

            virtual void SetBinding(const std::string& mapping, Binding* m_Binding)
            {
                GetMapping(mapping)->SetBinding(m_Binding);
            }
            virtual void SetBinding(const std::string& mapping, Binding* m_Binding, const Key key)
            {
                SetBinding(mapping, m_Binding);
                AddControl(mapping, key);
            }
            virtual void SetBinding(const std::string& mapping, Binding* m_Binding, const MouseButton key)
            {
                SetBinding(mapping, m_Binding);
                AddControl(mapping, key);
            }

            virtual void RemoveControl(const std::string& mapping, const Key key)
            {
                Control* control = GetControl(key);
                GetMapping(mapping)->RemoveControl(control);
            }
            virtual void RemoveControl(const std::string& mapping, const MouseButton key)
            {
                Control* control = GetControl(key);
                GetMapping(mapping)->RemoveControl(control);
            }

            virtual void AddControl(const std::string& mapping, const Key key)
            {
                Control* control = GetControl(key);
                GetMapping(mapping)->AddControl(control);
            }
            virtual void AddControl(const std::string& mapping, const MouseButton key)
            {
                Control* control = GetControl(key);
                GetMapping(mapping)->AddControl(control);
            }

            virtual bool GetKey(const Key key)
            {
                return GetKeyboard()->GetControlValue<bool>(static_cast<int>(key));
            }

            virtual bool GetMouseButton(const MouseButton button)
            {
                return GetMouse()->GetControlValue<bool>(static_cast<int>(button));
            }

            virtual void GetMouseMovement(int& x, int& y)
            {
                glm::vec2 xy = GetMouse()->GetControlValue<glm::vec2>(static_cast<int>(MouseButton::MOUSE_XY_DELTA));
                x = static_cast<int>(xy.x);
                y = static_cast<int>(xy.y);
            }

            virtual void GetMousePosition(int& x, int& y)
            {
                glm::vec2 xy = GetMouse()->GetControlValue<glm::vec2>(static_cast<int>(MouseButton::MOUSE_XY));
                x = static_cast<int>(xy.x);
                y = static_cast<int>(xy.y);
            }

            virtual bool Invoke(const std::string& name)
            {
                if (m_Mappings.at(name))
                {
                    return m_Mappings.at(name)->Invoke();
                }
                return false;
            }

            virtual void PrintCommands()
            {
                std::cout << "Input Mappings:" << std::endl;
                for (std::map<std::string, Mapping*>::iterator it = m_Mappings.begin(); it != m_Mappings.cend(); it++)
                {
                    std::cout << it->second->ToString() << std::endl;
                }
            }

        protected:

            std::map<Controllers, Controller*> m_Controllers;

        private:

            Controller * GetKeyboard(const int id = 0)
            {
                return m_Controllers[Controllers::KEYBOARD];
            }

            Controller* GetMouse(const int id = 0)
            {
                return m_Controllers[Controllers::MOUSE];
            }

            Control* GetControl(const Key key)
            {
                return GetKeyboard()->GetControl(key);
            }

            Control* GetControl(const MouseButton key)
            {
                return GetMouse()->GetControl(key);
            }

            Mapping* GetMapping(const std::string& mapping, bool create = true)
            {
                if (create)
                {
                    Mapping*& m = m_Mappings[mapping];
                    return (m == nullptr) ? (m = new Mapping(mapping)) : m;
                }
                else
                {
                    std::map<std::string, Mapping*>::const_iterator it = m_Mappings.find(mapping);
                    return (it != m_Mappings.cend()) ? it->second : nullptr;
                }
            }

            void RemoveMapping(const std::string& mapping)
            {
                delete m_Mappings[mapping];
                m_Mappings.erase(mapping);
            }

            std::map<std::string, Mapping*> m_Mappings;

            class InputCommand
            {
            public:

                InputCommand(InputDevice* input, const std::string& id) :
                    m_Input(input),
                    m_ID(id)
                {
                }

                InputCommand& Set(const std::function<void()>& func, State state = State::STATE_PRESS)
                {
                    return Set<bool>([func](bool) { func(); }, state);
                }

                template <typename T>
                InputCommand& Set(const std::function<void(T)>& func, State state = State::STATE_PRESS)
                {
                    return Set(BindingT<T>::Create(func, state));
                }

                InputCommand& Unset(bool complete = false)
                {
                    m_Input->RemoveBinding(m_ID, complete);
                    return *this;
                }

                InputCommand& Set(Binding* m_Binding)
                {
                    m_Input->SetBinding(m_ID, m_Binding);
                    return *this;
                }

                InputCommand& Unbind(const Key key)
                {
                    m_Input->RemoveControl(m_ID, key);
                    return *this;
                }
                InputCommand& Unbind(const MouseButton key)
                {
                    m_Input->RemoveControl(m_ID, key);
                    return *this;
                }

                InputCommand& Bind(const Key key)
                {
                    m_Input->AddControl(m_ID, key);
                    return *this;
                }
                InputCommand& Bind(const MouseButton key)
                {
                    m_Input->AddControl(m_ID, key);
                    return *this;
                }

            private:

                InputDevice * m_Input;
                std::string m_ID;
            };

            public:

                InputCommand Command(const std::string& mapping)
                {
                    return InputCommand(this, mapping);
                }
        };
    }
}