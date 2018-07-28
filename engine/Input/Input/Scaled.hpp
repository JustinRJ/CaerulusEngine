#pragma once

#include "Control.hpp"

#include <vector>

namespace Core
{
    namespace Input
    {
        struct Scaled
        {
            Scaled() :
                State(0)
            {
            }

            virtual ~Scaled()
            {
            }

            virtual void Update(std::vector<std::shared_ptr<Control>>& list) = 0;

            int State;
        };

        template <typename T>
        class ScaledT : public Scaled
        {
        public:

            ScaledT(T val = T()) :
                DeadZone(0.0f),
                Value(val)
            {
            }

            virtual ~ScaledT()
            {
            }

            virtual void Update(std::vector<std::shared_ptr<Control>>& list) override
            {
                Pressed(SetT<T>(list));
            }

            operator T()
            {
                return Value;
            }

            T Value;
            float DeadZone;

        private:

            int Pressed(bool down)
            {
                return State = ((State & 1) << 1) | static_cast<int>(down);
            }

            template <typename U>
            bool SetT(std::vector<std::shared_ptr<Control>>& list)
            {
                return false;
            }

            template <>
            bool SetT<bool>(std::vector<std::shared_ptr<Control>>& list)
            {
                for (size_t i = 0; i < list.size(); ++i)
                {
                    if (static_cast<bool>(*list[i]))
                    {
                        Value = true;
                        return Value;
                    }
                }
                Value = false;
                return Value;
            }

            template <>
            bool SetT<float>(std::vector<std::shared_ptr<Control>>& list)
            {
                Value = 0.0f;
                for (size_t i = 0; i < list.size(); ++i)
                {
                    Value += static_cast<float>(*list[i]);
                }
                return (Value < 0 ? -Value : Value) > DeadZone;
            }

            template <>
            bool SetT<glm::vec2>(std::vector<std::shared_ptr<Control>>& list)
            {
                Value = glm::vec2(0, 0);
                for (size_t i = 0; i < list.size(); ++i)
                {
                    Value += (glm::vec2) * list[i];
                }
                return Value.length() > DeadZone;
            }
        };
    }
}