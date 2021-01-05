#include "gx-rnd-eng-engine.hpp"
//#include "../../core/asset/gx-cr-asset-manager.hpp"
//#include "../../core/gx-cr-function-loader.hpp"
//#include "../../core/sync/gx-cr-sync-kernel-workers.hpp"
//#include "../../core/sync/gx-cr-sync-update-manager.hpp"
//#include "../../physics/gx-phs-engine.hpp"
#include "../../platform/gx-plt-application.hpp"
#include "../../platform/gx-plt-log.hpp"
//#include "../buffer/gx-rnd-buf-manager.hpp"
//#include "../command/gx-rnd-cmd-manager.hpp"
//#include "../graph/tree/gx-rnd-gr-tr-pbr.hpp"
//#include "../pipeline/gx-rnd-pip-manager.hpp"
//#include "../scene/gx-rnd-scn-manager.hpp"
#include "../../vulkan/engine/gx-vk-eng-engine.hpp"
//#include "../../direct3dx/engine/gx-d3d-eng-engine.hpp"
//#include "../../metal/engine/gx-mtl-eng-engine.hpp"
//#include "../../opengl/engine/gx-gl-eng-engine.hpp"
#include "../../core/macro/gx-cr-mcr-assert.hpp"

//void gearoenix::render::engine::Engine::do_late_delete() noexcept
//{
//    ++late_delete_index;
//    late_delete_index %= late_delete_assets.size();
//    late_delete_assets[late_delete_index].clear();
//}

gearoenix::render::engine::Engine::Engine(
    const RuntimeConfiguration& configuration,
    std::shared_ptr<platform::Application> platform_application,
    const Type engine_type) noexcept
    : engine_type(engine_type)
    , configuration(configuration)
    , platform_application(std::move(platform_application))
//    , function_loader(new core::FunctionLoader())
//    , kernels(new core::sync::KernelWorkers())
//    , update_manager(new core::sync::UpdateManager(kernels.get()))
//    , physics_engine(new physics::Engine(system_application, kernels.get()))
//    , late_delete_assets(10)
{
    //    kernels->add_step(
    //        [this] {
    //            function_loader->unload();
    //            render_tree->update();
    //            function_loader->unload();
    //        },
    //        [this](const unsigned int kernel_index) {
    //            render_tree->record(kernel_index);
    //        },
    //        [] {},
    //        [this] {
    //            function_loader->unload();
    //            render_tree->submit();
    //        });
}

std::shared_ptr<gearoenix::render::engine::Engine> gearoenix::render::engine::Engine::construct(
    const RuntimeConfiguration& configuration,
    std::shared_ptr<platform::Application> platform_application) noexcept
{
    std::shared_ptr<Engine> result;
#ifdef GX_RENDER_VULKAN_ENABLED
    if (configuration.get_vulkan_render_backend_enabled() && vulkan::engine::Engine::is_supported()) {
        result = vulkan::engine::Engine::construct(configuration, std::move(platform_application));
    }
#endif
#ifdef GX_RENDER_DIRECT3DX_ENABLED
    if (result == nullptr && configuration.get_direct3dx_render_backend_enabled() && direct3dx::engine::Engine::is_supported()) {
        result = direct3dx::engine::Engine::construct(configuration, std::move(platform_application));
    }
#endif
#ifdef GX_RENDER_METAL_ENABLED
    if (result == nullptr && configuration.get_metal_render_backend_enabled() && metal::engine::Engine::is_supported()) {
        result = metal::engine::Engine::construct(configuration, std::move(platform_application));
    }
#endif
#ifdef GX_RENDER_OPENGL_ENABLED
    if (result == nullptr && configuration.get_opengl_render_backend_enabled() && opengl::engine::Engine::is_supported()) {
        result = opengl::engine::Engine::construct(configuration, std::move(platform_application));
    }
#endif
    GX_CHECK_NOT_EQUAL(result, nullptr)
    result->self = result;
    return result;
}

gearoenix::render::engine::Engine::~Engine() noexcept = default;

//void gearoenix::render::engine::Engine::late_delete(std::shared_ptr<core::asset::Asset> asset) noexcept
//{
//    GX_GUARD_LOCK(late_delete_assets)
//    late_delete_assets[late_delete_index].push_back(std::move(asset));
//}

void gearoenix::render::engine::Engine::update() noexcept
{
    const std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> delta_time_duration = now - last_frame_time;
    delta_time = delta_time_duration.count();
    last_frame_time = now;
    ++frame_number_from_start;
    frame_number = frame_number_from_start % frames_count;
    //    function_loader->unload();
    //    physics_engine->update();
    //    kernels->do_steps();
    //    do_late_delete();
}