#pragma once

#include "../PipeLine/Shader.h"

namespace Graphics
{
    namespace PipeLine
    {
        class PipeLineItem
        {
        public:
            PipeLineItem(int ID, const Shader& shader) :
                m_ItemID(ID),
                m_Shader(shader)
            {
            }

            virtual ~PipeLineItem() = default;
            virtual void SendParamsToShader() = 0;

        private:
            int m_ItemID = -1;
            const Shader& m_Shader;
        };
    }
}