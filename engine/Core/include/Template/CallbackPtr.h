#pragma once

#include <functional>

namespace Core
{
    namespace Template
    {
        template <class T>
        class CallbackPtr
        {
        public:

            CallbackPtr(T* pointer, std::function<void(void)> callback) :
                m_pointer(pointer),
                m_callback(callback)
            {}

            ~CallbackPtr()
            {
                m_callback();
            }

            T* Get()
            {
                return m_pointer;
            }

            const T* Get() const
            {
                return m_pointer;
            }

            T* operator->()
            {
                return m_pointer;
            }

            const T* operator->() const
            {
                return m_pointer;
            }

            operator bool() const
            {
                return m_pointer != nullptr;
            }

        private:
            std::function<void(void)> m_callback;
            T* m_pointer;
        };
    }
}
