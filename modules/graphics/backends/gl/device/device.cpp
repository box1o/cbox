#include "device.hpp"
#include "cbox/core/core.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace cc {

static void GLAPIENTRY OpenGLDebugCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam
) {
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;

    const char* type_str = "UNKNOWN";
    switch (type) {
        case GL_DEBUG_TYPE_ERROR: type_str = "ERROR"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_str = "DEPRECATED"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: type_str = "UNDEFINED"; break;
        case GL_DEBUG_TYPE_PORTABILITY: type_str = "PORTABILITY"; break;
        case GL_DEBUG_TYPE_PERFORMANCE: type_str = "PERFORMANCE"; break;
        case GL_DEBUG_TYPE_OTHER: type_str = "OTHER"; break;
    }

    if (severity == GL_DEBUG_SEVERITY_HIGH) {
        log::Error("OpenGL {}: {}", type_str, message);
    } else if (severity == GL_DEBUG_SEVERITY_MEDIUM) {
        log::Warn("OpenGL {}: {}", type_str, message);
    } else {
        log::Debug("OpenGL {}: {}", type_str, message);
    }
}

GLDevice::~GLDevice() {
}

auto GLDevice::Create() -> result<ref<GLDevice>> {
    auto device = ref<GLDevice>(new GLDevice());

    if (auto res = device->Initialize(); !res) {
        return err(res.error());
    }

    return ok(device);
}

auto GLDevice::Initialize() -> result<void> {
    QueryCapabilities();
    initialized_ = true;
    return ok();
}

void GLDevice::QueryCapabilities() {
    GLint max_texture_size = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);
    capabilities_.max_texture_size = static_cast<u32>(max_texture_size);

    GLint max_texture_units = 0;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_texture_units);
    capabilities_.max_texture_units = static_cast<u32>(max_texture_units);

    GLint max_vertex_attribs = 0;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_vertex_attribs);
    capabilities_.max_vertex_attributes = static_cast<u32>(max_vertex_attribs);

    GLint max_uniform_locations = 0;
    glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &max_uniform_locations);
    capabilities_.max_uniform_locations = static_cast<u32>(max_uniform_locations);

    GLint max_color_attachments = 0;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &max_color_attachments);
    capabilities_.max_color_attachments = static_cast<u32>(max_color_attachments);

    if (GLAD_GL_EXT_texture_filter_anisotropic) {
        GLfloat max_anisotropy = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max_anisotropy);
        capabilities_.max_anisotropy = max_anisotropy;
    } else {
        capabilities_.max_anisotropy = 1.0f;
    }

    capabilities_.supports_compute = (GLAD_GL_VERSION_4_3 != 0);
    capabilities_.supports_geometry_shader = (GLAD_GL_VERSION_3_2 != 0);
    capabilities_.supports_tessellation = (GLAD_GL_VERSION_4_0 != 0);

    log::Info("Device Capabilities:");
    log::Info("  Max Texture Size: {}", capabilities_.max_texture_size);
    log::Info("  Max Texture Units: {}", capabilities_.max_texture_units);
    log::Info("  Max Vertex Attributes: {}", capabilities_.max_vertex_attributes);
    log::Info("  Max Anisotropy: {}", capabilities_.max_anisotropy);
    log::Info("  Compute Shaders: {}", capabilities_.supports_compute ? "Yes" : "No");
}

void GLDevice::Clear(const ClearColor& color, f32 depth, u8 stencil) {
    glClearColor(color[0], color[1], color[2], color[3]);
    glClearDepth(depth);
    glClearStencil(stencil);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GLDevice::SetViewport(u32 x, u32 y, u32 width, u32 height) {
    glViewport(static_cast<GLint>(x), static_cast<GLint>(y), 
               static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void GLDevice::SetVSync(bool enabled) {
    glfwSwapInterval(enabled ? 1 : 0);
}

}
