#ifndef GEAROENIX_RENDER_CAMERA_ORTHOGRAPHIC_CONTROLLER_HPP
#define GEAROENIX_RENDER_CAMERA_ORTHOGRAPHIC_CONTROLLER_HPP
#include "gx-rnd-cmr-camera.hpp"

namespace gearoenix::render::camera {
struct OrthographicController final {
    // private:
    //     double aspects_size = 1.0;
    //     void update_aspects_size() noexcept;
    //     void update_cascades() noexcept;
    //
    // public:
    //     OrthographicController(core::Id my_id, std::string name, platform::stream::Stream* f, engine::Engine* e) noexcept;
    //     OrthographicController(core::Id my_id, std::string name, engine::Engine* e) noexcept;
    //     [[nodiscard]] math::Ray3 create_ray3(double x, double y) const noexcept final;
    //     [[nodiscard]] double get_distance(const math::Vec3<double>& model_location) const noexcept final;
    //     void set_aspects_size(double aspects_size) noexcept;
    //     void set_aspects(unsigned int w, unsigned int h) noexcept final;
};
}
#endif
