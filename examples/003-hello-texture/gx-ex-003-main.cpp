#include <gearoenix/core/gx-cr-application.hpp>
#include <gearoenix/physics/gx-phs-transformation.hpp>
#include <gearoenix/platform/gx-plt-log.hpp>
#include <gearoenix/render/camera/gx-rnd-cmr-manager.hpp>
#include <gearoenix/render/engine/gx-rnd-eng-engine.hpp>
#include <gearoenix/render/gx-rnd-vertex.hpp>
#include <gearoenix/render/material/gx-rnd-mat-pbr.hpp>
#include <gearoenix/render/mesh/gx-rnd-msh-builder.hpp>
#include <gearoenix/render/mesh/gx-rnd-msh-manager.hpp>
#include <gearoenix/render/scene/gx-rnd-scn-builder.hpp>
#include <gearoenix/render/scene/gx-rnd-scn-manager.hpp>
#include <gearoenix/render/texture/gx-rnd-txt-manager.hpp>

struct GameApp final : public gearoenix::core::Application {
    explicit GameApp(gearoenix::platform::Application& plt_app) noexcept
        : Application(plt_app)
    {
        std::vector<gearoenix::render::PbrVertex> vertices(3);
        vertices[0].set_position(1.0f, -1.0f, 0.0f);
        vertices[0].set_uv(1.0f, -1.0f);
        vertices[1].set_position(0.0f, 1.0f, 0.0f);
        vertices[1].set_uv(0.0f, 1.0f);
        vertices[2].set_position(-1.0f, -1.0f, 0.0f);
        vertices[2].set_uv(-1.0f, -1.0f);

        std::vector<std::uint32_t> indices = { 0, 2, 1 };

        auto mesh_builder = render_engine.get_mesh_manager()->build("triangle", vertices, indices);
        gearoenix::render::material::Pbr material(render_engine);
        material.set_albedo(render_engine.get_texture_manager()->create_2d_from_file(
            "gearoenix-logo",
            gearoenix::platform::AbsolutePath("../../../../assets/gearoenix-logo.png")));
        mesh_builder->set_material(material);

        auto camera_builder = render_engine.get_camera_manager()->build("camera");
        camera_builder->get_transformation().set_location(0.0f, 0.0f, 5.0f);
        camera_builder->set(gearoenix::render::camera::Projection::Perspective);

        const auto scene_builder = render_engine.get_scene_manager()->build("scene");
        scene_builder->add(std::move(mesh_builder));
        scene_builder->add(std::move(camera_builder));
    }

    void update() noexcept final
    {
        Application::update();
    }
};

GEAROENIX_START(GameApp)