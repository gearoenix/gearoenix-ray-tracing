#include "gx-gl-engine.hpp"
#ifdef GX_RENDER_OPENGL_ENABLED
#include "../platform/gx-plt-application.hpp"
#include "gx-gl-camera.hpp"
#include "gx-gl-constants.hpp"
#include "gx-gl-loader.hpp"
#include "gx-gl-mesh.hpp"
#include "gx-gl-model.hpp"
#include "gx-gl-submission.hpp"
#include "gx-gl-texture.hpp"

gearoenix::gl::Engine::Engine(platform::Application& platform_application) noexcept
    : render::engine::Engine(render::engine::Type::OpenGL, platform_application)
    , submission_manager(new SubmissionManager(*this))
{
    frames_count = GEAROENIX_GL_FRAMES_COUNT;
    camera_manager = std::make_unique<CameraManager>(*this);
    mesh_manager = std::make_unique<MeshManager>(*this);
    model_manager = std::make_unique<ModelManager>(*this);
    texture_manager = std::make_unique<TextureManager>(*this);
}

gearoenix::gl::Engine::~Engine() noexcept = default;

void gearoenix::gl::Engine::start_frame() noexcept
{
    todos.unload();
    render::engine::Engine::start_frame();
    todos.unload();
}

void gearoenix::gl::Engine::end_frame() noexcept
{
    todos.unload();
    render::engine::Engine::end_frame();
    todos.unload();
    submission_manager->update();
    todos.unload();
}

void gearoenix::gl::Engine::window_resized() noexcept
{
}

void gearoenix::gl::Engine::upload_imgui_fonts() noexcept
{
}

bool gearoenix::gl::Engine::is_supported() noexcept
{
    return load_library();
}

std::unique_ptr<gearoenix::gl::Engine> gearoenix::gl::Engine::construct(platform::Application& platform_application) noexcept
{
    if (!is_supported())
        return std::unique_ptr<gearoenix::gl::Engine>();
    return std::make_unique<gearoenix::gl::Engine>(platform_application);
}

#endif