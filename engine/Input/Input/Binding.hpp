#pragma once

#include "Scaled.hpp"
#include "InputEnums.h"

#include <memory>
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
                m_ValueBase.release();
            }

            virtual void Invoke() = 0;

            virtual void InvokeAll() = 0;

            virtual void Update(std::vector<std::shared_ptr<Control>>& list)
            {
                m_ValueBase->Update(list);
            }

        protected:

            Binding(Scaled& val) :
                m_ValueBase(&val)
            {
            }

        private:

            std::unique_ptr<Scaled> m_ValueBase;
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

            virtual void InvokeAll() override
            {
                m_Callback(m_Value);
            }

            template<typename FT>
            void Set(const FT& func)
            {
                m_Callback = func;
            }

            template<typename FT>
            static std::unique_ptr<Binding> Create(const FT& func, State state = State::STATE_NONE)
            {
                std::unique_ptr<BindingT<T>> binding = std::make_unique<BindingT<T>>();
                binding->Set(func);
                return binding;
            }

        private:

            ScaledT<T> m_Value;
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

            virtual void InvokeAll() override
            {
                for (unsigned int i = 0; i < m_Callbacks.size(); ++i)
                {
                    const std::function<void(bool)>& func = m_Callbacks[i];
                    if (func)
                    {
                        func(m_Value);
                    }
                }
            }

            template<typename FT>
            void Set(int state, const FT& func)
            {
                m_Callbacks[state] = func;
            }

            template<typename FT>
            static std::unique_ptr<Binding> Create(const FT& func, State state = State::STATE_PRESS)
            {
                std::unique_ptr<BindingT<bool>> binding = std::make_unique<BindingT<bool>>();
                if (state == State::STATE_HOLD)
                {
                    binding->Set(static_cast<int>(State::STATE_PRESS), func);
                    binding->Set(static_cast<int>(State::STATE_RELEASE), func);
                }
                binding->Set(static_cast<int>(state), func);
                return binding;
            }

        private:

            ScaledT<bool> m_Value;
            typedef std::vector<std::function<void(bool)>> CallbackMap;
            CallbackMap m_Callbacks;
        };
    }
}