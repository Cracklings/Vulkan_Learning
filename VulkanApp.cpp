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
    device_.freeCommandBuffers(command_pool_, command_buffer_);
    device_.destroyCommandPool(command_pool_);
    device_.destroy();
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
    const auto required_extensions{glfwGetRequiredInstanceExtensions(&required_extension_count)};
    ici.enabledExtensionCount = required_extension_count;
    ici.ppEnabledExtensionNames = required_extensions;
    auto layer_props{vk::enumerateInstanceLayerProperties()};
    auto layer{"VK_LAYER_LUNARG_standard_validation"};
    ici.enabledLayerCount = 1;
    ici.ppEnabledLayerNames = &layer;
    instance_ = vk::createInstance(ici, nullptr);
}

void VulkanApp::enumeratePhysicalDevices()
{
    auto physical_devices{instance_.enumeratePhysicalDevices()};
    for (const auto& physical_device : physical_devices)
    {
        std::string dev{physical_device.getProperties().deviceName};
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
    vk::DeviceCreateInfo ci{};
    std::vector<vk::DeviceQueueCreateInfo> q_ci{};
    auto queue_families{physical_device_.getQueueFamilyProperties()};
    for (const auto& family : queue_families)
    {
        std::cout << to_string(family.queueFlags) << std::endl;
        std::cout << family.queueCount << std::endl;
    }
    vk::DeviceQueueCreateInfo q_render{};
    float_t q_priority{1};
    q_render.pQueuePriorities = &q_priority;
    q_render.queueCount = 1;
    auto q_prop = std::find_if(queue_families.begin(), queue_families.end(),
                               [](vk::QueueFamilyProperties prop)
                               {
                                   return prop.queueFlags & vk::QueueFlagBits::eGraphics;
                               });
    render_q_index_ = std::distance(q_prop, queue_families.begin());
    q_render.queueFamilyIndex = gsl::narrow<int32_t>(render_q_index_);
    q_ci.push_back(q_render);
    //	Just do more of this to create multiple queues.
    ci.pQueueCreateInfos = q_ci.data();
    ci.queueCreateInfoCount = q_ci.size();
    device_ = physical_device_.createDevice(ci);
}

void VulkanApp::createCommandBuffers()
{
    vk::CommandPoolCreateInfo ci{};
    ci.queueFamilyIndex = render_q_index_;
    command_pool_ = device_.createCommandPool(ci);

    vk::CommandBufferAllocateInfo ai{};
    ai.commandPool = command_pool_;
    ai.commandBufferCount = 1;
    ai.level = vk::CommandBufferLevel::ePrimary;
    command_buffer_ = device_.allocateCommandBuffers(ai);
}
