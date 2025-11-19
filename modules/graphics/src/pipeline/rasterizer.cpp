#include "cbox/graphics/pipeline/rasterizer.hpp"
#include <glad/glad.h>

namespace cc {

void RasterizerState::Apply() const {
    if (cull_mode == CullMode::None) {
        glDisable(GL_CULL_FACE);
    } else {
        glEnable(GL_CULL_FACE);

        switch (cull_mode) {
        case CullMode::Front:
            glCullFace(GL_FRONT);
            break;
        case CullMode::Back:
            glCullFace(GL_BACK);
            break;
        case CullMode::FrontAndBack:
            glCullFace(GL_FRONT_AND_BACK);
            break;
        default:
            break;
        }
    }

    glFrontFace(front_face == FrontFace::Clockwise ? GL_CW : GL_CCW);

    switch (fill_mode) {
    case FillMode::Solid:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    case FillMode::Wireframe:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    case FillMode::Point:
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        break;
    }

    if (depth_bias != 0.0f || depth_bias_slope != 0.0f) {
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(depth_bias_slope, depth_bias);
    } else {
        glDisable(GL_POLYGON_OFFSET_FILL);
    }

    if (depth_clamp) {
        glEnable(GL_DEPTH_CLAMP);
    } else {
        glDisable(GL_DEPTH_CLAMP);
    }

    if (scissor_test) {
        glEnable(GL_SCISSOR_TEST);
    } else {
        glDisable(GL_SCISSOR_TEST);
    }
}

} // namespace cc
