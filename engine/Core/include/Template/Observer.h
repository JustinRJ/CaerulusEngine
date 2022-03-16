#pragma once

#include <list>

namespace Core
{
    namespace Template
    {
        template <class T>
        class Observer;

        template <class T>
        class Observable
        {
        public:
            Observable(T value) :
                m_value(value)
            {}

            void Register(Observer<T>* observer)
            {
                m_observed.push_back(observer);
            }

            void Unregister(Observer<T>* observer)
            {
                m_observed.remove(observer);
            }

            void Notify()
            {
                for (auto it = std::begin(m_observed); it != std::end(m_observed); ++it)
                {
                    (*it)->Update(m_value);
                }
            }

            void SetValue(T value)
            {
                m_value = value;
                Notify();
            }

        private:
            T m_value;
            std::list<Observer<T>*> m_observed;
        };

        template <class T>
        class Observer
        {
        public:
            Observer(Observable<T>& observable, T initialValue = T()) :
                m_value(initialValue),
                m_observable(observable)
            {
                m_observable.Register(this);
            }

            void Register()
            {
                m_observable.Register(this);
            }

            void Unregister()
            {
                m_observable.Unregister(this);
            }

            void Update(T valueFromObservable)
            {
                m_value = valueFromObservable;
            }

        private:
            T m_value;
            Observable<T>& m_observable;
        };
    }
}