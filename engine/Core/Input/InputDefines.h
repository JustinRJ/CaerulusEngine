#pragma once

namespace Core
{
    namespace Input
    {
        enum class Modifier
        {
            None = NULL,
            Shift = GLFW_MOD_SHIFT,
            Alt = GLFW_MOD_ALT,
            Control = GLFW_MOD_CONTROL,
            Super = GLFW_MOD_SUPER
        };

        enum class Action
        {
            Unknown = GLFW_KEY_UNKNOWN,
            Press = GLFW_PRESS,
            Release = GLFW_RELEASE,
            Hold = GLFW_REPEAT
        };
    }
}