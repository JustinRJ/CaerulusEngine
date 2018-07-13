#pragma once

#include "Boxed.hpp"
#include "InputEnums.h"

#include <functional>

namespace Core
{
    namespace Input
    {
        class Binding
        {
        public:

            virtual ~Binding()
            {
            }

            virtual void Invoke() = 0;

            virtual void Update(std::vector<Control*>& list)
            {
                m_ValueBase->Update(list);
            }

        protected:

            Binding(Boxed& val) :
                m_ValueBase(&val)
            {
            }

        private:

            Boxed* m_ValueBase;
        };

        template <typename T>
        class BindingT : public Binding
        {
        public:

            BindingT() :
                Binding(m_Value)
            {
            }

            virtual ~BindingT()
            {
            }

            virtual void Invoke() override
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

        private:

            BoxedT<T> m_Value;
            std::function<void(T)> m_Callback;
        };

        template <>
        class BindingT<bool> : public Binding
        {
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

            virtual void Invoke() override
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

        private:

            BoxedT<bool> m_Value;
            typedef std::vector<std::function<void(bool)>> CallbackMap;
            CallbackMap m_Callbacks;
        };
    }
}