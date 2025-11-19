#pragma once
#include "cbox/core/core.hpp"

namespace cc {

enum class CullMode : u8 { None, Front, Back, FrontAndBack };

enum class FillMode : u8 { Solid, Wireframe, Point };

enum class FrontFace : u8 { Clockwise, CounterClockwise };

struct RasterizerState {
    CullMode cull_mode{CullMode::Back};
    FillMode fill_mode{FillMode::Solid};
    FrontFace front_face{FrontFace::CounterClockwise};
    f32 depth_bias{0.0f};
    f32 depth_bias_slope{0.0f};
    bool depth_clamp{false};
    bool scissor_test{false};

    static RasterizerState Default() {
        return RasterizerState{};
    }

    static RasterizerState NoCull() {
        RasterizerState state;
        state.cull_mode = CullMode::None;
        return state;
    }

    static RasterizerState Wireframe() {
        RasterizerState state;
        state.fill_mode = FillMode::Wireframe;
        return state;
    }

    void Apply() const;
};

} // namespace cc
