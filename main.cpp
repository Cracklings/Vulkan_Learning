#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.hpp>
#include "VulkanApp.h"

int main(int argc, char* argv[])
{
	VulkanApp app;
	app.run();
	return 0;
}
