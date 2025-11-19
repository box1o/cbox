#pragma once
#include "cbox/core/core.hpp"

namespace cc {

enum class CompareOp : u8 {
    Never,
    Less,
    Equal,
    LessOrEqual,
    Greater,
    NotEqual,
    GreaterOrEqual,
    Always
};

enum class StencilOp : u8 {
    Keep,
    Zero,
    Replace,
    IncrementClamp,
    DecrementClamp,
    Invert,
    IncrementWrap,
    DecrementWrap
};

struct StencilOpState {
    StencilOp fail{StencilOp::Keep};
    StencilOp depth_fail{StencilOp::Keep};
    StencilOp pass{StencilOp::Keep};
    CompareOp compare{CompareOp::Always};
    u32 compare_mask{0xFF};
    u32 write_mask{0xFF};
    u32 reference{0};
};

struct DepthStencilState {
    bool depth_test{true};
    bool depth_write{true};
    CompareOp depth_compare{CompareOp::Less};

    bool stencil_test{false};
    StencilOpState stencil_front;
    StencilOpState stencil_back;

    static DepthStencilState Default() {
        return DepthStencilState{};
    }

    static DepthStencilState NoDepth() {
        DepthStencilState state;
        state.depth_test = false;
        state.depth_write = false;
        return state;
    }

    static DepthStencilState ReadOnly() {
        DepthStencilState state;
        state.depth_write = false;
        return state;
    }

    void Apply() const;
};

} // namespace cc
