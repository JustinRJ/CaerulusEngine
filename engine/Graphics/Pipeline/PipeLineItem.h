#pragma once

#include "../PipeLine/Shader.h"

namespace Graphics
{
    namespace PipeLine
    {
        class PipeLineItem
        {
        public:
            PipeLineItem(unsigned int ID, const Shader& shader) :
                m_ItemID(ID),
                m_Shader(shader)
            {
            }

            virtual ~PipeLineItem() {};
            virtual void SendParamsToShader() = 0;

        private:
            unsigned int m_ItemID = -1;
            const Shader& m_Shader;
        };
    }
}