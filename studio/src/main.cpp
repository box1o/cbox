// #include "../../modules/graphics/backends/gl/pipeline/varray.hpp"
// #include "cbox/core/core.hpp"
// #include "cbox/graphics/context/context.hpp"
// #include "cbox/graphics/graphics.hpp"
// #include "cbox/graphics/pipeline/vlayout.hpp"
// #include "cbox/graphics/resources/buffer.hpp"
// #include "cbox/graphics/resources/texture.hpp"
// #include "cbox/graphics/shader/module.hpp"
// #include "cbox/graphics/swapchain/swapchain.hpp"
// #include "cbox/graphics/types/types.hpp"
// #include "cbox/math/interop/transform.hpp"
// #include <glad/glad.h>
//
// struct Vertex {
//   cc::vec3f position;
//   cc::vec2f texcoord;
// };
//
// int main() {
//   cc::log::Init("studio");
//
//   auto window = cc::Window::Create({.name = "studio", .width = 720, .height = 480});
//
//   auto ctx = cc::RendererContext::Create(cc::RenderAPI::OpenGL)
//     .SetVSync(window->GetVSync())
//     .Build();
//
//   auto swapchain = cc::Swapchain::Create(window);
//
//   auto shader =
//     cc::ShaderModule::Create()
//     .AddStage(cc::ShaderStage::Vertex, "resources/shaders/texture.vert")
//     .AddStage(cc::ShaderStage::Fragment, "resources/shaders/texture.frag")
//     .Reflect()
//     .Build();
//
//   auto texture = cc::Texture2D::FromFile("resources/textures/test.png")
//     .GenerateMipmaps()
//     .Build();
//
//   Vertex vertices[] = {
//     {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
//     {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
//     {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
//     {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
//
//     {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
//     {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
//     {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
//     {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},
//
//     {{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
//     {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
//     {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
//     {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
//
//     {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
//     {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
//     {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
//     {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
//
//     {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
//     {{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
//     {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
//     {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
//
//     {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
//     {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
//     {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
//     {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
//   };
//
//   cc::u32 indices[] = {
//     0,  1,  2,  2,  3,  0,
//     4,  5,  6,  6,  7,  4,
//     8,  9, 10, 10, 11,  8,
//     12, 13, 14, 14, 15, 12,
//     16, 17, 18, 18, 19, 16,
//     20, 21, 22, 22, 23, 20
//   };
//
//   auto vbo = cc::Buffer::Create(cc::BufferType::Vertex, cc::BufferUsage::Static,
//                                 sizeof(vertices), vertices);
//
//   auto ibo = cc::Buffer::Create(cc::BufferType::Index, cc::BufferUsage::Static,
//                                 sizeof(indices), indices);
//
//   auto layout =
//     cc::VertexLayout::Create()
//     .Attribute(0, cc::VertexFormat::Float3, offsetof(Vertex, position))
//     .Attribute(1, cc::VertexFormat::Float2, offsetof(Vertex, texcoord))
//     .Stride(sizeof(Vertex))
//     .Build();
//
//   auto vao = cc::GLVertexArray::Create().value();
//   vao->Bind();
//   vao->SetVertexBuffer(vbo, layout);
//   vao->SetIndexBuffer(ibo);
//
//   struct MatData {
//     cc::mat4f model;
//     cc::mat4f view;
//     cc::mat4f projection;
//   };
//
//   auto ubo = cc::Buffer::Create(cc::BufferType::Uniform, cc::BufferUsage::Dynamic, sizeof(MatData));
//
//   glEnable(GL_DEPTH_TEST);
//   glDepthFunc(GL_LESS);
//   glCullFace(GL_BACK);
//   glEnable(GL_CULL_FACE);
//   glFrontFace(GL_CCW);
//
//
//   float rotation = 0.0f;
//
//   while (!window->ShouldClose()) {
//     swapchain->Clear({0.1f, 0.1f, 0.1f, 1.0f}, 1.0f);
//
//     rotation += 0.01f;
//
//     MatData matrices;
//     matrices.model =  cc::rotate(rotation, cc::vec3f(0.5f, 1.0f, 0.0f));
//     matrices.view = cc::look_at(
//       cc::vec3f(0.0f, 0.0f, 3.0f),
//       cc::vec3f(0.0f, 0.0f, 0.0f),
//       cc::vec3f(0.0f, 1.0f, 0.0f)
//     );
//     matrices.projection = cc::perspective(
//       cc::rad(45.0f),
//       720.0f / 480.0f,
//       0.1f,
//       100.0f
//     );
//
//     ubo->Bind();
//     ubo->SetData(&matrices, sizeof(MatData));
//     glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo->GetBufferID());
//
//     shader->Bind();
//     texture->Bind(0);
//     vao->Bind();
//
//     glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
//
//     swapchain->Present();
//     window->PollEvents();
//   }
//
//   cc::RendererContext::Shutdown();
//   return 0;
// }

#include "cbox/core/core.hpp"
#include "cbox/graphics/context/context.hpp"
#include "cbox/graphics/resources/texture.hpp"
#include "cbox/graphics/resources/buffer.hpp"
#include "cbox/graphics/shader/module.hpp"
#include "cbox/graphics/pipeline/vlayout.hpp"
#include "cbox/math/interop/transform.hpp"
#include "../../modules/graphics/backends/gl/pipeline/varray.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Vertex {
  cc::vec3f position;
  cc::vec2f texcoord;
};

int main() {
  cc::log::Init("studio");

  if (!glfwInit()) {
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(720, 480, "studio", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    glfwTerminate();
    return -1;
  }

  auto ctx = cc::RendererContext::Create(cc::RenderAPI::OpenGL)
    .SetVSync(true)
    .Build();

  auto shader = cc::ShaderModule::Create()
    .AddStage(cc::ShaderStage::Vertex, "resources/shaders/texture.vert")
    .AddStage(cc::ShaderStage::Fragment, "resources/shaders/texture.frag")
    .Reflect()
    .Build();

  auto texture = cc::Texture2D::FromFile("resources/textures/test.png")
    .GenerateMipmaps()
    .Build();

  Vertex vertices[] = {
    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
    {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
    {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
    {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},

    {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
    {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
    {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
    {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},

    {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
    {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
    {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
    {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},

    {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
    {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
    {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},

    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
    {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
    {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}},
    {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},

    {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f}},
    {{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f}},
    {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
    {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},
  };

  cc::u32 indices[] = {
    0,  1,  2,  2,  3,  0,
    4,  5,  6,  6,  7,  4,
    8,  9, 10, 10, 11,  8,
    12, 13, 14, 14, 15, 12,
    16, 17, 18, 18, 19, 16,
    20, 21, 22, 22, 23, 20
  };

  auto vbo = cc::Buffer::Create(cc::BufferType::Vertex, cc::BufferUsage::Static,
                                sizeof(vertices), vertices);
  auto ibo = cc::Buffer::Create(cc::BufferType::Index, cc::BufferUsage::Static,
                                sizeof(indices), indices);

  auto layout = cc::VertexLayout::Create()
    .Attribute(0, cc::VertexFormat::Float3, offsetof(Vertex, position))
    .Attribute(1, cc::VertexFormat::Float2, offsetof(Vertex, texcoord))
    .Stride(sizeof(Vertex))
    .Build();

  auto vao = cc::GLVertexArray::Create().value();
  vao->Bind();
  vao->SetVertexBuffer(vbo, layout);
  vao->SetIndexBuffer(ibo);

  struct MatData {
    cc::mat4f model;
    cc::mat4f view;
    cc::mat4f projection;
  };

  auto ubo = cc::Buffer::Create(cc::BufferType::Uniform, cc::BufferUsage::Dynamic, sizeof(MatData));

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  float rotation = 0.0f;

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    rotation += 0.01f;

    MatData matrices;
    matrices.model = cc::rotate(rotation, cc::vec3f(0.5f, 1.0f, 0.0f));
    matrices.view = cc::look_at(
      cc::vec3f(0.0f, 0.0f, 3.0f),
      cc::vec3f(0.0f, 0.0f, 0.0f),
      cc::vec3f(0.0f, 1.0f, 0.0f)
    );
    matrices.projection = cc::perspective(
      cc::rad(45.0f),
      720.0f / 480.0f,
      0.1f,
      100.0f
    );

    ubo->Bind();
    ubo->SetData(&matrices, sizeof(MatData));
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo->GetBufferID());

    shader->Bind();
    texture->Bind(0);
    vao->Bind();

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  cc::RendererContext::Shutdown();
  return 0;
}
