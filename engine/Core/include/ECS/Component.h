#pragma once

#include "ECS/Entity.h"

/// Each Component must pass Entity& through as a param and init Entity
/// Example(Core::ECS::Entity& e) : m_entity(e)
/// 
/// Must implement the following functions:
/// 
/// CAERULUS_COMPONENT_INIT
/// void OnEnable() {};
/// void OnDisable() {};
/// 
/// TODO - constexpr the above functions still similair to the ComponentManager.h

#define CAERULUS_COMPONENT_INIT                        \
private:                                               \
    bool m_isEnabled = true;                           \
    Core::ECS::Entity& m_entity;                       \
public:                                                \
    void SetEnabled(bool enabled)                      \
    {                                                  \
        if (m_entity.IsEnabled())                      \
        {                                              \
            if (m_isEnabled != enabled)                \
            {                                          \
                m_isEnabled = enabled;                 \
                enabled ? OnEnable() : OnDisable();    \
            }                                          \
        }                                              \
        else if (m_isEnabled != false)                 \
        {                                              \
            m_isEnabled = false;                       \
            OnDisable();                               \
        }                                              \
    }                                                  \
    Core::ECS::Entity& GetEntity()                     \
    {                                                  \
        return m_entity;                               \
    }                                                  \
    const Core::ECS::Entity& GetEntity() const         \
    {                                                  \
        return m_entity;                               \
    }                                                  \
    bool IsEnabled() const                             \
    {                                                  \
        return m_isEnabled && m_entity.IsEnabled();    \
    }