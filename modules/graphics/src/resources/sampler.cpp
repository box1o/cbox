#include "cbox/graphics/resources/sampler.hpp"
#include "../../backends/gl/resources/sampler.hpp"

namespace cc {

auto Sampler::Builder::SetFilter(Filter min, Filter mag) -> Builder& {
    min_filter_ = min;
    mag_filter_ = mag;
    return *this;
}

auto Sampler::Builder::SetMipmap(MipmapMode mode) -> Builder& {
    mipmap_mode_ = mode;
    return *this;
}

auto Sampler::Builder::SetWrap(WrapMode mode) -> Builder& {
    wrap_u_ = wrap_v_ = wrap_w_ = mode;
    return *this;
}

auto Sampler::Builder::SetWrapU(WrapMode mode) -> Builder& {
    wrap_u_ = mode;
    return *this;
}

auto Sampler::Builder::SetWrapV(WrapMode mode) -> Builder& {
    wrap_v_ = mode;
    return *this;
}

auto Sampler::Builder::SetWrapW(WrapMode mode) -> Builder& {
    wrap_w_ = mode;
    return *this;
}

auto Sampler::Builder::SetAnisotropy(f32 level) -> Builder& {
    anisotropy_ = level;
    return *this;
}

auto Sampler::Builder::SetBorderColor(const vec4f& color) -> Builder& {
    border_color_ = color;
    return *this;
}

auto Sampler::Builder::Build() -> ref<Sampler> {
    return GLSampler::Create(min_filter_, mag_filter_, mipmap_mode_,
                             wrap_u_, wrap_v_, wrap_w_,
                             anisotropy_, border_color_);
}

}
