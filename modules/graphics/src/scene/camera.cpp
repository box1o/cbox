#include "cbox/graphics/scene/camera.hpp"

namespace cc {

auto Camera::Builder::SetPerspective(f32 fov, f32 aspect, f32 near_plane, f32 far_plane) -> Builder& {
    type_ = ProjectionType::Perspective;
    fov_ = fov;
    aspect_ = aspect;
    near_ = near_plane;
    far_ = far_plane;
    return *this;
}

auto Camera::Builder::SetOrthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near_plane, f32 far_plane) -> Builder& {
    type_ = ProjectionType::Orthographic;
    left_ = left;
    right_ = right;
    bottom_ = bottom;
    top_ = top;
    near_ = near_plane;
    far_ = far_plane;
    return *this;
}

auto Camera::Builder::SetPosition(const vec3f& pos) -> Builder& {
    position_ = pos;
    return *this;
}

auto Camera::Builder::SetTarget(const vec3f& target) -> Builder& {
    target_ = target;
    return *this;
}

auto Camera::Builder::SetUp(const vec3f& up) -> Builder& {
    up_ = up;
    return *this;
}

auto Camera::Builder::Build() -> ref<Camera> {
    auto camera = ref<Camera>(new Camera());
    camera->type_ = type_;
    camera->fov_ = fov_;
    camera->aspect_ = aspect_;
    camera->near_ = near_;
    camera->far_ = far_;
    camera->left_ = left_;
    camera->right_ = right_;
    camera->bottom_ = bottom_;
    camera->top_ = top_;
    camera->position_ = position_;
    camera->target_ = target_;
    camera->up_ = up_;
    camera->view_dirty_ = true;
    camera->proj_dirty_ = true;
    return camera;
}

}
