#include "stdafx.h"

#include "Entity.h"
#include <list>

namespace
{
    unsigned int s_NumEntities = 0;
    unsigned int s_MaxDiscardedEntityIDs = 64;
    std::list<unsigned int> s_ReusableEntityIDs = std::list<unsigned int>(s_MaxDiscardedEntityIDs);
}

namespace Core
{
    namespace ECS
    {

        Entity::Entity()
        {
            if (s_MaxDiscardedEntityIDs < s_ReusableEntityIDs.size())
            {
                m_ID = s_NumEntities++;
            }
            else
            {
                m_ID = s_ReusableEntityIDs.front();
                s_ReusableEntityIDs.erase(s_ReusableEntityIDs.begin());
            }
        }

        void Entity::SwapID(IEntity& e)
        {
            unsigned int temp = m_ID;
            m_ID = e.GetID();
            dynamic_cast<Entity&>(e).SetID(temp);
        }

        Entity::~Entity()
        {
            s_NumEntities--;
            s_ReusableEntityIDs.push_back(m_ID);
        }

        unsigned int Entity::GetID() const
        {
            return m_ID;
        }

        unsigned int Entity::GetEntityCount() const
        {
            return s_NumEntities;
        }

        void Entity::SetID(unsigned int id)
        {
            m_ID = id;
        }
    }
}