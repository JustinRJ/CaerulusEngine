#include "stdafx.h"

#include "Mapping.h"
#include "../Parser/StringHelper.h"

#include <iostream>

namespace Core
{
    namespace Input
    {
        Mapping::Mapping(const std::string& id) :
            m_ID(id),
            m_Binding(nullptr)
        {
        }

        Mapping::~Mapping()
        {
            m_Controls.clear();
            delete m_Binding;
        }

        void Mapping::AddControl(Control* control)
        {
            if (control == nullptr)
            {
                std::cerr << "Failed to add control!" << std::endl;
                return;
            }
            for (auto c : m_Controls)
            {
                if (c == control)
                {
                    // Already added
                    return;
                }
            }
            m_Controls.push_back(control);
        }

        void Mapping::RemoveControl(Control* control)
        {
            for (std::vector<Control*>::const_iterator it = m_Controls.cbegin(); it < m_Controls.cend(); ++it)
            {
                if (*it == control)
                {
                    m_Controls.erase(it);
                    return;
                }
            }
            std::cerr << "Failed to remove control!" << std::endl;
        }

        void Mapping::SetBinding(Binding* binding)
        {
            delete m_Binding;
            m_Binding = binding;
        }

        void Mapping::Update()
        {
            if (m_Binding != nullptr)
            {
                m_Binding->Update(m_Controls);
                m_Binding->Invoke();
            }
        }

        std::string Mapping::ToString()
        {
            std::string str = Parser::StringHelper::Format("[%-0s]", m_ID.c_str());
            for (Control* control : m_Controls)
            {
                Parser::StringHelper::FormatAppend(str, " %-0s", control->Name.c_str());
            }
            return str;
        }
    }
}