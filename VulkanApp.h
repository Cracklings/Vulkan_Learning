#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

class VulkanApp
{
public:
	VulkanApp();
	~VulkanApp();
	void run();

private:
	void loop();
	void initializeVulkan();
	void terminateVulkan();

	void initializeInstance();
	void enumeratePhysicalDevices();
	void createSurface();
	void createDevice();
	void createCommandBuffers();

	GLFWwindow* window_                              = nullptr;

	VkSurfaceKHR surface_;
	vk::Instance instance_                           = nullptr;
	vk::PhysicalDevice physical_device_              = nullptr;
	vk::Device device_                               = nullptr;

	ptrdiff_t render_q_index_                        = -1;
	std::vector<vk::CommandBuffer> command_buffer_   = {};
	vk::CommandPool command_pool_                    = nullptr;
};
