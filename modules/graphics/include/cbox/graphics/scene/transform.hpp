#pragma once
#include "cbox/core/core.hpp"
#include "cbox/math/math.hpp"

namespace cc {

class Transform {
public:
    Transform() = default;

    static Transform Identity() {
        return Transform();
    }

    void SetPosition(const vec3f& pos) noexcept { 
        position_ = pos; 
        dirty_ = true; 
    }

    void SetRotation(const vec3f& euler) noexcept { 
        rotation_ = euler; 
        dirty_ = true; 
    }

    void SetScale(const vec3f& scale) noexcept { 
        scale_ = scale; 
        dirty_ = true; 
    }

    void Translate(const vec3f& delta) noexcept { 
        position_ += delta; 
        dirty_ = true; 
    }

    void Rotate(const vec3f& delta) noexcept { 
        rotation_ += delta; 
        dirty_ = true; 
    }

    void Scale(const vec3f& delta) noexcept { 
        scale_ *= delta; 
        dirty_ = true; 
    }

    const vec3f& GetPosition() const noexcept { return position_; }
    const vec3f& GetRotation() const noexcept { return rotation_; }
    const vec3f& GetScale() const noexcept { return scale_; }

    const mat4f& GetMatrix() const noexcept {
        if (dirty_) {
            UpdateMatrix();
        }
        return matrix_;
    }

    vec3f Forward() const noexcept {
        if (dirty_) UpdateMatrix();
        return vec3f{-matrix_[2][0], -matrix_[2][1], -matrix_[2][2]}.norm();
    }

    vec3f Right() const noexcept {
        if (dirty_) UpdateMatrix();
        return vec3f{matrix_[0][0], matrix_[0][1], matrix_[0][2]}.norm();
    }

    vec3f Up() const noexcept {
        if (dirty_) UpdateMatrix();
        return vec3f{matrix_[1][0], matrix_[1][1], matrix_[1][2]}.norm();
    }

private:
    void UpdateMatrix() const noexcept {
        mat4f translation = translate(position_);
        mat4f rotation = rotate(rotation_);
        mat4f scaling = scale(scale_);
        matrix_ = translation * rotation * scaling;
        dirty_ = false;
    }

    vec3f position_{0.0f, 0.0f, 0.0f};
    vec3f rotation_{0.0f, 0.0f, 0.0f};
    vec3f scale_{1.0f, 1.0f, 1.0f};
    mutable mat4f matrix_{mat4f::identity()};
    mutable bool dirty_{true};
};

}
