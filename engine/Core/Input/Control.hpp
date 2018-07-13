#pragma once

#include "../Math/MathHelper.h"

#include <string>

namespace Core
{
    namespace Input
    {
        struct Control
        {
            Control(const std::string& name) :
                Name(name)
            {
            }

            virtual ~Control()
            {
            }

            virtual operator bool() = 0;
            virtual operator float() = 0;
            virtual operator glm::vec2() = 0;

            std::string Name;
        };

        template <typename T>
        class ControlT : public Control
        {
        public:

            ControlT(const std::string& name) :
                Control(name),
                Value()
            {
            }

            virtual ~ControlT()
            {
            }

            virtual operator bool() override        { return AsBool(Value); }
            virtual operator float() override       { return AsFloat(Value); }
            virtual operator glm::vec2() override   { return AsVec2(Value); }

            T Value;

        private:

            static bool AsBool(bool val)        { return val; }
            static bool AsBool(float val)       { return val > 0; }
            static bool AsBool(glm::vec2 val)   { return val.length() > 0; }

            static float AsFloat(float val)     { return val; }
            static float AsFloat(bool val)      { return val ? 1.0f : 0.0f; }
            static float AsFloat(glm::vec2 val) { return static_cast<float>(val.length()); }

            static glm::vec2 AsVec2(bool val)       { return glm::vec2(static_cast<float>(val), static_cast<float>(val)); }
            static glm::vec2 AsVec2(float val)      { return glm::vec2(val, val); }
            static glm::vec2 AsVec2(glm::vec2 val)  { return val; }
        };
    }
}