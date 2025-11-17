#pragma once
#include "cbox/core/core.hpp"
#include "cbox/math/math.hpp"

namespace cc {

enum class ProjectionType : u8 {
    Perspective,
    Orthographic
};

class Camera {
public:
    class Builder {
    public:
        Builder() = default;

        Builder& SetPerspective(f32 fov, f32 aspect, f32 near_plane, f32 far_plane);
        Builder& SetOrthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near_plane, f32 far_plane);
        Builder& SetPosition(const vec3f& pos);
        Builder& SetTarget(const vec3f& target);
        Builder& SetUp(const vec3f& up);

        ref<Camera> Build();

    private:
        ProjectionType type_{ProjectionType::Perspective};
        f32 fov_{45.0f};
        f32 aspect_{16.0f / 9.0f};
        f32 near_{0.1f};
        f32 far_{1000.0f};
        f32 left_{-10.0f};
        f32 right_{10.0f};
        f32 bottom_{-10.0f};
        f32 top_{10.0f};
        vec3f position_{0.0f, 0.0f, 3.0f};
        vec3f target_{0.0f, 0.0f, 0.0f};
        vec3f up_{0.0f, 1.0f, 0.0f};
    };

    static Builder Create() { return Builder(); }

    const mat4f& GetViewMatrix() const noexcept {
        if (view_dirty_) UpdateView();
        return view_;
    }

    const mat4f& GetProjectionMatrix() const noexcept {
        if (proj_dirty_) UpdateProjection();
        return projection_;
    }

    mat4f GetViewProjectionMatrix() const noexcept {
        return GetProjectionMatrix() * GetViewMatrix();
    }

    void SetPosition(const vec3f& pos) noexcept { 
        position_ = pos; 
        view_dirty_ = true; 
    }

    void SetTarget(const vec3f& target) noexcept { 
        target_ = target; 
        view_dirty_ = true; 
    }

    void SetUp(const vec3f& up) noexcept { 
        up_ = up; 
        view_dirty_ = true; 
    }

    void SetPerspective(f32 fov, f32 aspect, f32 near_plane, f32 far_plane) noexcept {
        type_ = ProjectionType::Perspective;
        fov_ = fov;
        aspect_ = aspect;
        near_ = near_plane;
        far_ = far_plane;
        proj_dirty_ = true;
    }

    void SetOrthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near_plane, f32 far_plane) noexcept {
        type_ = ProjectionType::Orthographic;
        left_ = left;
        right_ = right;
        bottom_ = bottom;
        top_ = top;
        near_ = near_plane;
        far_ = far_plane;
        proj_dirty_ = true;
    }

    const vec3f& GetPosition() const noexcept { return position_; }
    const vec3f& GetTarget() const noexcept { return target_; }
    const vec3f& GetUp() const noexcept { return up_; }

    vec3f GetForward() const noexcept { 
        return (target_ - position_).norm(); 
    }

    vec3f GetRight() const noexcept { 
        return GetForward().cross(up_).norm(); 
    }

    void Move(const vec3f& delta) noexcept {
        position_ += delta;
        target_ += delta;
        view_dirty_ = true;
    }

    void Orbit(f32 yaw, f32 pitch) noexcept {
        vec3f offset = position_ - target_;
        f32 radius = offset.len();

        mat4f rotation = rotate_y(yaw) * rotate_x(pitch);
        vec3f rotated = vec3f{
            rotation[0][0] * offset.x + rotation[0][1] * offset.y + rotation[0][2] * offset.z,
            rotation[1][0] * offset.x + rotation[1][1] * offset.y + rotation[1][2] * offset.z,
            rotation[2][0] * offset.x + rotation[2][1] * offset.y + rotation[2][2] * offset.z
        };

        position_ = target_ + rotated.norm() * radius;
        view_dirty_ = true;
    }

private:
    Camera() = default;

    void UpdateView() const noexcept {
        view_ = look_at(position_, target_, up_);
        view_dirty_ = false;
    }

    void UpdateProjection() const noexcept {
        if (type_ == ProjectionType::Perspective) {
            projection_ = perspective(fov_, aspect_, near_, far_);
        } else {
            projection_ = ortho(left_, right_, bottom_, top_, near_, far_);
        }
        proj_dirty_ = false;
    }

    ProjectionType type_{ProjectionType::Perspective};
    f32 fov_{45.0f};
    f32 aspect_{16.0f / 9.0f};
    f32 near_{0.1f};
    f32 far_{1000.0f};
    f32 left_{-10.0f};
    f32 right_{10.0f};
    f32 bottom_{-10.0f};
    f32 top_{10.0f};

    vec3f position_{0.0f, 0.0f, 3.0f};
    vec3f target_{0.0f, 0.0f, 0.0f};
    vec3f up_{0.0f, 1.0f, 0.0f};

    mutable mat4f view_{mat4f::identity()};
    mutable mat4f projection_{mat4f::identity()};
    mutable bool view_dirty_{true};
    mutable bool proj_dirty_{true};

    friend class Builder;
};

}
