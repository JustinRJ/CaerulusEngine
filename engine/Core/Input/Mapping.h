#pragma once

#include "Binding.hpp"

namespace Core
{
    namespace Input
    {
        class Mapping
        {
        public:

            Mapping(const std::string& id);
            virtual ~Mapping();

            void AddControl(Control* control);

            void RemoveControl(Control* control);

            void SetBinding(Binding* binding);

            void Update();

            std::string ToString();

        private:

            std::string m_ID;
            std::vector<Control*> m_Controls;
            Binding* m_Binding;
        };
    }
}