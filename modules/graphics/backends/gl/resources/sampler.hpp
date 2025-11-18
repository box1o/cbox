#pragma once
#include "cbox/graphics/resources/sampler.hpp"

namespace cc {

class GLSampler : public Sampler {
public:
    ~GLSampler() override;

    static auto Create(Filter min, Filter mag, MipmapMode mipmap, 
                       WrapMode wrap_u, WrapMode wrap_v, WrapMode wrap_w,
                       f32 anisotropy, const vec4f& border_color) 
        -> ref<GLSampler>;

    void Bind(u32 slot) const override;

    u32 GetSamplerID() const noexcept { return sampler_id_; }

private:
    GLSampler() = default;

    u32 sampler_id_{0};
};

}
