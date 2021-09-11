#pragma once

#include <map>
#include <functional>

namespace Core
{
    namespace Template
    {
        template <class T, class R>
        class Cacher
        {
        public:

            Cacher(std::function<R(T)> calculation) :
                m_calculation(calculation)
            {}

            void ClearCache()
            {
                m_cache.clear();
            }

            R Calculate(T value)
            {
                auto it = m_cache.find(value);
                return it == m_cache.end() ? m_cache[value] = m_calculation(value) : it->second;
            }

        private:
            std::function<R(T)> m_calculation;
            std::map<T, R> m_cache;
        };
    }
}
