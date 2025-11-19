#include "cbox/graphics/pipeline/blend.hpp"
#include <glad/glad.h>

namespace cc {

static GLenum ToGLBlendFactor(BlendFactor factor) {
    switch (factor) {
    case BlendFactor::Zero:
        return GL_ZERO;
    case BlendFactor::One:
        return GL_ONE;
    case BlendFactor::SrcColor:
        return GL_SRC_COLOR;
    case BlendFactor::OneMinusSrcColor:
        return GL_ONE_MINUS_SRC_COLOR;
    case BlendFactor::DstColor:
        return GL_DST_COLOR;
    case BlendFactor::OneMinusDstColor:
        return GL_ONE_MINUS_DST_COLOR;
    case BlendFactor::SrcAlpha:
        return GL_SRC_ALPHA;
    case BlendFactor::OneMinusSrcAlpha:
        return GL_ONE_MINUS_SRC_ALPHA;
    case BlendFactor::DstAlpha:
        return GL_DST_ALPHA;
    case BlendFactor::OneMinusDstAlpha:
        return GL_ONE_MINUS_DST_ALPHA;
    case BlendFactor::ConstantColor:
        return GL_CONSTANT_COLOR;
    case BlendFactor::OneMinusConstantColor:
        return GL_ONE_MINUS_CONSTANT_COLOR;
    case BlendFactor::ConstantAlpha:
        return GL_CONSTANT_ALPHA;
    case BlendFactor::OneMinusConstantAlpha:
        return GL_ONE_MINUS_CONSTANT_ALPHA;
    }
    return GL_ONE;
}

static GLenum ToGLBlendOp(BlendOp op) {
    switch (op) {
    case BlendOp::Add:
        return GL_FUNC_ADD;
    case BlendOp::Subtract:
        return GL_FUNC_SUBTRACT;
    case BlendOp::ReverseSubtract:
        return GL_FUNC_REVERSE_SUBTRACT;
    case BlendOp::Min:
        return GL_MIN;
    case BlendOp::Max:
        return GL_MAX;
    }
    return GL_FUNC_ADD;
}

void BlendState::Apply(u32 attachment_count) const {
    glBlendColor(blend_constants.x, blend_constants.y, blend_constants.z, blend_constants.w);

    for (u32 i = 0; i < attachment_count && i < 8; ++i) {
        const auto& att = attachments[i];

        if (att.enabled) {
            glEnablei(GL_BLEND, i);
            glBlendFuncSeparatei(i, ToGLBlendFactor(att.src_color), ToGLBlendFactor(att.dst_color),
                                 ToGLBlendFactor(att.src_alpha), ToGLBlendFactor(att.dst_alpha));
            glBlendEquationSeparatei(i, ToGLBlendOp(att.color_op), ToGLBlendOp(att.alpha_op));
        } else {
            glDisablei(GL_BLEND, i);
        }

        glColorMaski(i, att.color_write_r ? GL_TRUE : GL_FALSE,
                     att.color_write_g ? GL_TRUE : GL_FALSE, att.color_write_b ? GL_TRUE : GL_FALSE,
                     att.color_write_a ? GL_TRUE : GL_FALSE);
    }
}

} // namespace cc
