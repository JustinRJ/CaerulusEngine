#pragma once

#include "Interface/INode.h"

namespace Core
{
    using namespace Interface;
    class CAERULUS_CORE Node : public INode
    {
    public:

        Node();
        virtual ~Node();

        unsigned int GetID() const final;
        unsigned int GetNodeCount() const final;

        void SwapID(INode& e) final;

    private:
        void SetID(unsigned int id) final;
        unsigned int m_ID;
    };

    bool operator == (const INode &left, const INode &right)
    {
        return left.GetID() == right.GetID();
    }

    bool operator < (const INode &left, const INode &right)
    {
        return left.GetID() < right.GetID();
    }

    bool operator > (const INode &left, const INode &right)
    {
        return right < left;
    }

    bool operator <= (const INode &left, const INode &right)
    {
        return !(left > right);
    }

    bool operator >= (const INode &left, const INode &right)
    {
        return !(left < right);
    }
}
