#pragma once

#include <type_traits>

#include "ECS/Entity.h"

#define CAERULUS_COMPONENT_INIT                                                                         \
private:                                                                                                \
    bool m_isEnabled = true;                                                                            \
    Core::ECS::Entity& m_entity;                                                                        \
                                                                                                        \
    template <class U>                                                                                  \
    static auto _caerulus_try_enable(U& u, int) -> decltype(u.OnEnable(), void()) { u.OnEnable(); }     \
    template <class U>                                                                                  \
    static void _caerulus_try_enable(U&, ...) {}                                                        \
                                                                                                        \
    template <class U>                                                                                  \
    static auto _caerulus_try_disable(U& u, int) -> decltype(u.OnDisable(), void()) { u.OnDisable(); }  \
    template <class U>                                                                                  \
    static void _caerulus_try_disable(U&, ...) {}                                                       \
                                                                                                        \
public:                                                                                                 \
    void SetEnabled(bool enabled)                                                                       \
    {                                                                                                   \
        if (m_entity.IsEnabled())                                                                       \
        {                                                                                               \
            if (m_isEnabled != enabled)                                                                 \
            {                                                                                           \
                m_isEnabled = enabled;                                                                  \
                if (enabled)                                                                            \
                {                                                                                       \
                    _caerulus_try_enable(*this, 0);                                                     \
                }                                                                                       \
                else                                                                                    \
                {                                                                                       \
                    _caerulus_try_disable(*this, 0);                                                    \
                }                                                                                       \
            }                                                                                           \
        }                                                                                               \
        else if (m_isEnabled != false)                                                                  \
        {                                                                                               \
            m_isEnabled = false;                                                                        \
            _caerulus_try_disable(*this, 0);                                                            \
        }                                                                                               \
    }                                                                                                   \
                                                                                                        \
    Core::ECS::Entity& GetEntity()                                                                      \
    {                                                                                                   \
        return m_entity;                                                                                \
    }                                                                                                   \
                                                                                                        \
    const Core::ECS::Entity& GetEntity() const                                                          \
    {                                                                                                   \
        return m_entity;                                                                                \
    }                                                                                                   \
                                                                                                        \
    bool IsEnabled() const                                                                              \
    {                                                                                                   \
        return m_isEnabled && m_entity.IsEnabled();                                                     \
    }                                                                                                   \
