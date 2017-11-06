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

	GLFWwindow* window_;

	vk::Instance instance_;
	vk::PhysicalDevice physical_device_;
	vk::Device device_;
	vk::Queue queue_;
};

