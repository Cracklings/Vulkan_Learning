#include "VulkanApp.h"
#include <gsl/gsl>
#include <iostream>
#include <vulkan/vk_platform.h>

VulkanApp::VulkanApp()
{
	Ensures(glfwInit() == GLFW_TRUE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window_ = glfwCreateWindow(800, 600, "Vulkan App 1", nullptr, nullptr);
}


VulkanApp::~VulkanApp()
{
	glfwDestroyWindow(window_);
	glfwTerminate();
}

void VulkanApp::run()
{
	initializeVulkan();
	loop();
}

void VulkanApp::loop()
{
	while (!glfwWindowShouldClose(window_))
	{
		glfwPollEvents();
	}

}

void VulkanApp::initializeVulkan()
{
	initializeInstance();
	enumeratePhysicalDevices();
	createSurface();
	createDevice();
}

void VulkanApp::terminateVulkan()
{
	instance_.destroy();
}

void VulkanApp::initializeInstance()
{
	vk::ApplicationInfo appinfo;
	appinfo.apiVersion = VK_API_VERSION_1_0;
	appinfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
	appinfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
	appinfo.pApplicationName = "Crack learning vulkan";
	appinfo.pEngineName = "CrackEngine";

	vk::InstanceCreateInfo ici;
	ici.pApplicationInfo = &appinfo;
	uint32_t required_extension_count;
	const auto required_extensions{ glfwGetRequiredInstanceExtensions(&required_extension_count) };
	ici.enabledExtensionCount = required_extension_count;
	ici.ppEnabledExtensionNames = required_extensions;
	auto layer_props{ vk::enumerateInstanceLayerProperties() };
	auto layer{ "VK_LAYER_LUNARG_standard_validation" };
	ici.enabledLayerCount = 1;
	ici.ppEnabledLayerNames = &layer;
	instance_ = vk::createInstance(ici, nullptr);
}

void VulkanApp::enumeratePhysicalDevices()
{
	auto physical_devices { instance_.enumeratePhysicalDevices() };
	for (const auto& physical_device : physical_devices)
	{
		std::string dev{ physical_device.getProperties().deviceName };
		if (dev.find("Radeon") != std::string::npos ||
			dev.find("GTX") != std::string::npos)
		{
			physical_device_ = physical_device;
			break;
		}
	}
	Ensures(physical_device_);
}

void VulkanApp::createSurface()
{
	if (glfwCreateWindowSurface(instance_, window_, nullptr, &surface_))
	{
		throw std::runtime_error("Window surface initialization failed.");
	}
}

void VulkanApp::createDevice()
{

}

