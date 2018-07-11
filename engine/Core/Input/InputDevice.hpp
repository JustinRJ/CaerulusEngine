#pragma once

#include "InputEnums.h"
#include "../Parser/StringHelper.h"
#include "../Math/MathHelper.h"

#include <map>
#include <iostream>
#include <functional>

namespace Core
{
    namespace Input
    {
        struct Control
        {
            std::string Name;

            Control(const std::string& name) :
                Name(name)
            {
            }

            virtual operator bool() = 0;
            virtual operator float() = 0;
            virtual operator glm::vec2() = 0;
        };

        template <typename T>
        struct ControlT : public Control
        {
            T Value;

            ControlT(const std::string& name) :
                Control(name),
                Value()
            {
            }

            virtual operator bool() { return AsBool(Value); }
            virtual operator float() { return AsFloat(Value); }
            virtual operator glm::vec2() { return AsVec2(Value); }

        protected:

            static bool AsBool(bool val) { return val; }
            static bool AsBool(float val) { return val > 0; }
            static bool AsBool(glm::vec2 val) { return val.length() > 0; }

            static float AsFloat(float val) { return val; }
            static float AsFloat(bool val) { return val ? 1.0f : 0.0f; }
            static float AsFloat(glm::vec2 val) { return static_cast<float>(val.length()); }

            static glm::vec2 AsVec2(glm::vec2 val) { return val; }
            static glm::vec2 AsVec2(bool val) { return glm::vec2(static_cast<float>(val), static_cast<float>(val)); }
            static glm::vec2 AsVec2(float val) { return glm::vec2(val, val); }
        };

        struct IBoxed
        {
            int State;

            IBoxed() :
                State(0)
            {
            }

            virtual void Update(std::vector<Control*>& list) = 0;
        };

        template <typename T>
        struct Boxed : public IBoxed
        {
            float DeadZone;
            T Value;

            Boxed(T val = T()) : 
                DeadZone(0.0f),
                Value(val)
            {
            }

            virtual void Update(std::vector<Control*>& list)
            {
                Pressed(SetT<T>(list));
            }

            operator T()
            {
                return Value;
            }

        protected:

            virtual int Pressed(bool down)
            {
                return State = ((State & 1) << 1) | static_cast<int>(down);
            }

            template <typename U>
            bool SetT(std::vector<Control*>& list)
            {
                return false;
            }

            template <>
            bool SetT<bool>(std::vector<Control*>& list)
            {
                for (size_t i = 0; i < list.size(); ++i)
                {
                    if (static_cast<bool>(*list[i]))
                    {
                        Value = true;
                        return Value;
                    }
                }
                Value = false;
                return Value;
            }

            template <>
            bool SetT<float>(std::vector<Control*>& list)
            {
                Value = 0.0f;

                for (size_t i = 0; i < list.size(); ++i)
                {
                    Value += static_cast<float>(*list[i]);
                }

                return (Value < 0 ? -Value : Value) > DeadZone;
            }

            template <>
            bool SetT<glm::vec2>(std::vector<Control*>& list)
            {
                Value = glm::vec2(0, 0);

                for (size_t i = 0; i < list.size(); ++i)
                {
                    Value += (glm::vec2)*list[i];
                }

                return Value.length() > DeadZone;
            }
        };

        struct Binding
        {
        protected:

            Binding(IBoxed& val) : 
                m_ValueBase(&val)
            {
            }

            IBoxed* m_ValueBase;

        public:

            virtual ~Binding()
            {
            }

            virtual void Invoke() = 0;

            virtual void Update(std::vector<Control*>& list)
            {
                m_ValueBase->Update(list);
            }
        };

        template <typename T>
        struct BindingT : public Binding
        {
        protected:

            Boxed<T> m_Value;
            std::function<void(T)> m_Callback;

        public:

            BindingT() : 
                Binding(m_Value)
            {
            }

            virtual ~BindingT()
            {
            }

            virtual void Invoke()
            {
                m_Callback(m_Value);
            }

            template<typename FT>
            void Set(const FT& func)
            {
                m_Callback = func;
            }

            template<typename FT>
            static Binding* Create(const FT& func, State state = State::STATE_NONE)
            {
                BindingT<T>* m_Binding = new BindingT<T>();
                m_Binding->Set(func);
                return m_Binding;
            }
        };

        template <>
        struct BindingT<bool> : public Binding
        {
        protected:

            Boxed<bool> m_Value;
            typedef std::vector<std::function<void(bool)>> CallbackMap;
            CallbackMap m_Callbacks;

        public:

            BindingT() : 
                Binding(m_Value),
                m_Value(false),
                m_Callbacks(4)
            {
            }

            virtual ~BindingT()
            {
                m_Callbacks.clear();
            }

            virtual void Invoke()
            {
                const std::function<void(bool)>& func = m_Callbacks[m_Value.State];
                if (func)
                {
                    func(m_Value);
                }
            }

            template<typename FT>
            void Set(int state, const FT& func)
            {
                m_Callbacks[state] = func;
            }

            template<typename FT>
            static Binding* Create(const FT& func, State state = State::STATE_PRESS)
            {
                BindingT<bool>* m_Binding = new BindingT<bool>();
                if (state == State::STATE_HOLD)
                {
                    m_Binding->Set(static_cast<int>(State::STATE_PRESS), func);
                    m_Binding->Set(static_cast<int>(State::STATE_RELEASE), func);
                }
                m_Binding->Set(static_cast<int>(state), func);
                return m_Binding;
            }
        };

        struct Mapping
        {
        protected:

            std::string m_ID;
            std::vector<Control*> m_Controls;
            Binding* m_Binding;

        public:

            Mapping(const std::string& id) :
                m_ID(id),
                m_Binding(nullptr)
            {
            }

            virtual ~Mapping()
            {
                m_Controls.clear();
                delete m_Binding;
            }

            void AddControl(Control* control)
            {
                if (control == nullptr)
                {
                    std::cerr << "Failed to add control!" << std::endl;
                    return;
                }
                for (auto c : m_Controls)
                {
                    if (c == control)
                    {
                        // Already added
                        return;
                    }
                }
                m_Controls.push_back(control);
            }

            void RemoveControl(Control* control)
            {
                for (std::vector<Control*>::const_iterator it = m_Controls.cbegin(); it < m_Controls.cend(); ++it)
                {
                    if (*it == control)
                    {
                        m_Controls.erase(it);
                        return;
                    }
                }
                std::cerr << "Failed to remove control!" << std::endl;
            }

            void SetBinding(Binding* binding)
            {
                delete m_Binding;
                m_Binding = binding;
            }

            void Update()
            {
                if (m_Binding != nullptr)
                {
                    m_Binding->Update(m_Controls);
                    m_Binding->Invoke();
                }
            }

            std::string ToString()
            {
                std::string str = Parser::StringHelper::Format("[%-0s]", m_ID.c_str());
                for (Control* control : m_Controls)
                {
                    Parser::StringHelper::FormatAppend(str, " %-0s", control->Name.c_str());
                }
                return str;
            }
        };

        class Controller
        {
        public:

            Controller(int controls) :
                m_Controls(controls)
            {
            }

            virtual ~Controller()
            {
                for (size_t i = 0; i < m_Controls.size(); ++i)
                {
                    delete m_Controls[i];
                }
                m_Controls.clear();
            }

            virtual void Update() = 0;

            virtual Control* GetControl(int key)
            {
                return m_Controls[key];
            }

            template<typename T>
            T GetControlValue(int key)
            {
                return GetControl<T>(key)->Value;
            }

            template<typename T>
            void SetControlValue(int key, T value)
            {
                GetControl<T>(key)->Value = value;
            }

        protected:

            std::vector<Control*> m_Controls;

            template<typename T>
            void SetControl(int key, const std::string& name)
            {
                m_Controls[key] = new ControlT<T>(name);
            }

            template<typename T>
            ControlT<T>* GetControl(int key)
            {
                return static_cast<ControlT<T>*>(m_Controls[key]);
            }
        };

        class InputDevice
        {
        protected:

            std::map<Controllers, Controller*> m_Controllers;
            std::map<std::string, Mapping*> m_Mappings;

            Controller* GetKeyboard(const int id = 0)
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

        public:

            class InputCommand
            {
            protected:

                InputDevice* m_Input;
                std::string m_ID;

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
            };

            friend class InputCommand;

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

            InputCommand Command(const std::string& mapping)
            {
                return InputCommand(this, mapping);
            }

            virtual bool Key(const Key key)
            {
                return GetKeyboard()->GetControlValue<bool>(static_cast<int>(key));
            }

            virtual bool MouseButton(const MouseButton button)
            {
                return GetMouse()->GetControlValue<bool>(static_cast<int>(button));
            }

            virtual void MouseMovement(int& x, int& y)
            {
                glm::vec2 xy = GetMouse()->GetControlValue<glm::vec2>(static_cast<int>(MouseButton::MOUSE_XY_DELTA));
                x = static_cast<int>(xy.x);
                y = static_cast<int>(xy.y);
            }

            virtual void MousePosition(int& x, int& y)
            {
                glm::vec2 xy = GetMouse()->GetControlValue<glm::vec2>(static_cast<int>(MouseButton::MOUSE_XY));
                x = static_cast<int>(xy.x);
                y = static_cast<int>(xy.y);
            }

            virtual void PrintCommands()
            {
                std::cout << "Input Mappings:" << std::endl;
                for (std::map<std::string, Mapping*>::iterator it = m_Mappings.begin(); it != m_Mappings.cend(); it++)
                {
                    std::cout << it->second->ToString() << std::endl;
                }
            }
        };
    }
}