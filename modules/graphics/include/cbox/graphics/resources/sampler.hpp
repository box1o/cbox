#pragma once
#include "cbox/core/core.hpp"
#include "cbox/math/math.hpp"

namespace cc {

enum class Filter : u8 { Nearest, Linear };

enum class MipmapMode : u8 { Nearest, Linear };

enum class WrapMode : u8 { Repeat, MirroredRepeat, ClampToEdge, ClampToBorder };

class Sampler {
  public:
    class Builder {
      public:
        Builder() = default;

        Builder& SetFilter(Filter min, Filter mag);
        Builder& SetMipmap(MipmapMode mode);
        Builder& SetWrap(WrapMode mode);
        Builder& SetWrapU(WrapMode mode);
        Builder& SetWrapV(WrapMode mode);
        Builder& SetWrapW(WrapMode mode);
        Builder& SetAnisotropy(f32 level);
        Builder& SetBorderColor(const vec4f& color);

        ref<Sampler> Build();

      private:
        Filter min_filter_{Filter::Linear};
        Filter mag_filter_{Filter::Linear};
        MipmapMode mipmap_mode_{MipmapMode::Linear};
        WrapMode wrap_u_{WrapMode::Repeat};
        WrapMode wrap_v_{WrapMode::Repeat};
        WrapMode wrap_w_{WrapMode::Repeat};
        f32 anisotropy_{1.0f};
        vec4f border_color_{0.0f, 0.0f, 0.0f, 1.0f};
    };

    virtual ~Sampler() = default;

    static Builder Create() {
        return Builder();
    }

    virtual void Bind(u32 slot) const = 0;

  protected:
    Sampler() = default;
};

} // namespace cc
