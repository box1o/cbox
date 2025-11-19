#include "cbox/graphics/pipeline/depth_stencil.hpp"
#include <glad/glad.h>

namespace cc {

static GLenum ToGLCompareOp(CompareOp op) {
    switch (op) {
    case CompareOp::Never:
        return GL_NEVER;
    case CompareOp::Less:
        return GL_LESS;
    case CompareOp::Equal:
        return GL_EQUAL;
    case CompareOp::LessOrEqual:
        return GL_LEQUAL;
    case CompareOp::Greater:
        return GL_GREATER;
    case CompareOp::NotEqual:
        return GL_NOTEQUAL;
    case CompareOp::GreaterOrEqual:
        return GL_GEQUAL;
    case CompareOp::Always:
        return GL_ALWAYS;
    }
    return GL_LESS;
}

static GLenum ToGLStencilOp(StencilOp op) {
    switch (op) {
    case StencilOp::Keep:
        return GL_KEEP;
    case StencilOp::Zero:
        return GL_ZERO;
    case StencilOp::Replace:
        return GL_REPLACE;
    case StencilOp::IncrementClamp:
        return GL_INCR;
    case StencilOp::DecrementClamp:
        return GL_DECR;
    case StencilOp::Invert:
        return GL_INVERT;
    case StencilOp::IncrementWrap:
        return GL_INCR_WRAP;
    case StencilOp::DecrementWrap:
        return GL_DECR_WRAP;
    }
    return GL_KEEP;
}

void DepthStencilState::Apply() const {
    if (depth_test) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(ToGLCompareOp(depth_compare));
    } else {
        glDisable(GL_DEPTH_TEST);
    }

    glDepthMask(depth_write ? GL_TRUE : GL_FALSE);

    if (stencil_test) {
        glEnable(GL_STENCIL_TEST);

        glStencilFuncSeparate(GL_FRONT, ToGLCompareOp(stencil_front.compare),
                              stencil_front.reference, stencil_front.compare_mask);
        glStencilOpSeparate(GL_FRONT, ToGLStencilOp(stencil_front.fail),
                            ToGLStencilOp(stencil_front.depth_fail),
                            ToGLStencilOp(stencil_front.pass));
        glStencilMaskSeparate(GL_FRONT, stencil_front.write_mask);

        glStencilFuncSeparate(GL_BACK, ToGLCompareOp(stencil_back.compare), stencil_back.reference,
                              stencil_back.compare_mask);
        glStencilOpSeparate(GL_BACK, ToGLStencilOp(stencil_back.fail),
                            ToGLStencilOp(stencil_back.depth_fail),
                            ToGLStencilOp(stencil_back.pass));
        glStencilMaskSeparate(GL_BACK, stencil_back.write_mask);
    } else {
        glDisable(GL_STENCIL_TEST);
    }
}

} // namespace cc
