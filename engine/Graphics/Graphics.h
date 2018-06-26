#pragma once

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_INCLUDE_VULKAN
#include "Vulkan/vulkan.h"
#include <glfw3.h>
#include <glfw3native.h>

void* Hwnd()
{
    if (glfwVulkanSupported())
    {
        // Vulkan is available, at least for compute
    }

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
    return glfwGetWin32Window(window);
}