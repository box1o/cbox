#pragma once
#include "cbox/core/core.hpp"
#include "cbox/math/math.hpp"

namespace cc {

enum class BlendFactor : u8 {
    Zero,
    One,
    SrcColor,
    OneMinusSrcColor,
    DstColor,
    OneMinusDstColor,
    SrcAlpha,
    OneMinusSrcAlpha,
    DstAlpha,
    OneMinusDstAlpha,
    ConstantColor,
    OneMinusConstantColor,
    ConstantAlpha,
    OneMinusConstantAlpha
};

enum class BlendOp : u8 { Add, Subtract, ReverseSubtract, Min, Max };

struct BlendAttachment {
    bool enabled{false};
    BlendFactor src_color{BlendFactor::One};
    BlendFactor dst_color{BlendFactor::Zero};
    BlendOp color_op{BlendOp::Add};
    BlendFactor src_alpha{BlendFactor::One};
    BlendFactor dst_alpha{BlendFactor::Zero};
    BlendOp alpha_op{BlendOp::Add};
    bool color_write_r{true};
    bool color_write_g{true};
    bool color_write_b{true};
    bool color_write_a{true};
};

struct BlendState {
    BlendAttachment attachments[8];
    vec4f blend_constants{0.0f, 0.0f, 0.0f, 0.0f};

    static BlendState Disabled() {
        return BlendState{};
    }

    static BlendState AlphaBlend() {
        BlendState state;
        state.attachments[0].enabled = true;
        state.attachments[0].src_color = BlendFactor::SrcAlpha;
        state.attachments[0].dst_color = BlendFactor::OneMinusSrcAlpha;
        state.attachments[0].src_alpha = BlendFactor::One;
        state.attachments[0].dst_alpha = BlendFactor::Zero;
        return state;
    }

    static BlendState Additive() {
        BlendState state;
        state.attachments[0].enabled = true;
        state.attachments[0].src_color = BlendFactor::One;
        state.attachments[0].dst_color = BlendFactor::One;
        state.attachments[0].src_alpha = BlendFactor::One;
        state.attachments[0].dst_alpha = BlendFactor::One;
        return state;
    }

    void Apply(u32 attachment_count = 1) const;
};

} // namespace cc
