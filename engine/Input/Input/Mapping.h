#pragma once

#include "Binding.hpp"

namespace Input
{
    class Mapping
    {
    public:

        Mapping(const std::string& id);
        virtual ~Mapping();

        void AddControl(std::shared_ptr<Control> control);
        void RemoveControl(std::shared_ptr<Control> control);

        void SetBinding(std::unique_ptr<Binding> binding);

        void Update();
        bool Invoke();

        std::string ToString();

    private:

        std::string m_ID;
        std::vector<std::shared_ptr<Control>> m_Controls;
        std::unique_ptr<Binding> m_Binding;
    };
}
