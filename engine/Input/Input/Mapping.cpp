#include "stdafx.h"

#include "Mapping.h"
#include "../../Core/Parser/StringHelper.h"

#include <iostream>

namespace Input
{
    Mapping::Mapping(const std::string& id) :
        m_ID(id)
    {
    }

    Mapping::~Mapping()
    {
        m_Controls.clear();
        m_Binding.release();
    }

    void Mapping::AddControl(std::shared_ptr<Control> control)
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

    void Mapping::RemoveControl(std::shared_ptr<Control> control)
    {
        for (std::vector<std::shared_ptr<Control>>::const_iterator it = m_Controls.cbegin(); it < m_Controls.cend(); ++it)
        {
            if (*it == control)
            {
                m_Controls.erase(it);
                return;
            }
        }
        std::cerr << "Failed to remove control!" << std::endl;
    }

    void Mapping::SetBinding(std::unique_ptr<Binding> binding)
    {
        m_Binding.reset(binding.release());
    }

    void Mapping::Update()
    {
        if (m_Binding)
        {
            m_Binding->Update(m_Controls);
            m_Binding->Invoke();
        }
    }

    bool Mapping::Invoke()
    {
        if (m_Binding)
        {
            m_Binding->InvokeAll();
            return true;
        }
        return false;
    }

    std::string Mapping::ToString()
    {
        std::string str = Core::Parser::StringHelper::Format("[%-0s]", m_ID.c_str());
        for (std::shared_ptr<Control> control : m_Controls)
        {
            Core::Parser::StringHelper::FormatAppend(str, " %-0s", control->Name.c_str());
        }
        return str;
    }
}
