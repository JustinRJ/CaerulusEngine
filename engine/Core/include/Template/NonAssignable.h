#pragma once

namespace Core
{
    namespace Template
    {
        template <class T>
        class NonAssignable
        {
        public:
            NonAssignable(T* nonAssignable) :
                m_nonAssignable(nonAssignable)
            {}

            T* operator->()
            {
                return m_nonAssignable;
            }

            const T* operator->() const
            {
                return m_nonAssignable;
            }

            operator bool () const
            {
                return m_nonAssignable != nullptr;
            }

        private:
            T* m_nonAssignable;
        };
    }
}