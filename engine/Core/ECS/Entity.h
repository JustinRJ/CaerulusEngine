#pragma once

#include "../Interface/IEntity.h"

namespace Core
{
    namespace ECS
    {
        using namespace Core::Interface;
        class CAERULUS_CORE Entity : public IEntity
        {
        public:

            Entity();
            virtual ~Entity();

            unsigned int GetID() const final;
            unsigned int GetEntityCount() const final;

            void SwapID(IEntity& e) final;

        private:
            void SetID(unsigned int id) final;
            unsigned int m_ID;
        };

        bool operator == (const IEntity &left, const IEntity &right)
        {
            return left.GetID() == right.GetID();
        }

        bool operator < (const IEntity &left, const IEntity &right)
        {
            return left.GetID() < right.GetID();
        }

        bool operator > (const IEntity &left, const IEntity &right)
        {
            return right < left;
        }

        bool operator <= (const IEntity &left, const IEntity &right)
        {
            return !(left > right);
        }

        bool operator >= (const IEntity &left, const IEntity &right)
        {
            return !(left < right);
        }
    }
}
