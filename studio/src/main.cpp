#include "cbox/core/core.hpp"
#include "cbox/graphics/graphics.hpp"
#include "../modules/graphics/backends/gl/pipeline/varray.hpp"
#include "../modules/graphics/backends/gl/pipeline/pipeline.hpp"
#include <glad/glad.h>

int main() {
    cc::log::Init("renderer");
    
    auto window_result = cc::Window::Create({
        .name = "Textured Cube Test",
        .width = 1280,
        .height = 720
    });
    
    if (!window_result) {
        window_result.error().log();
        return -1;
    }
    auto window = window_result.value();
    
    auto ctx_result = cc::RendererContext::Create(cc::RenderAPI::OpenGL)
        .SetVSync(true)
        .Build();
    
    if (!ctx_result) {
        ctx_result.error().log();
        return -1;
    }
    
    auto swapchain_result = cc::Swapchain::Create(window);
    if (!swapchain_result) {
        swapchain_result.error().log();
        return -1;
    }
    auto swapchain = swapchain_result.value();
    
    auto quad_mesh = cc::Mesh::CreateQuad();
    
    auto texture_result = cc::Texture2D::FromFile("resources/textures/test.png")
        .SetSRGB(true)
        .GenerateMipmaps()
        .Build();
    
    if (!texture_result) {
        texture_result.error().log();
        return -1;
    }
    auto texture = texture_result.value();
    
    auto sampler_result = cc::Sampler::Create()
        .SetFilter(cc::Filter::Linear, cc::Filter::Linear)
        .SetWrap(cc::WrapMode::Repeat)
        .SetAnisotropy(16.0f)
        .Build();
    
    if (!sampler_result) {
        sampler_result.error().log();
        return -1;
    }
    auto sampler = sampler_result.value();
    
    auto shader_result = cc::ShaderModule::Create()
        .AddStage(cc::ShaderStage::Vertex, "resources/shaders/textured.vert")
        .AddStage(cc::ShaderStage::Fragment, "resources/shaders/textured.frag")
        .Build();
    
    if (!shader_result) {
        shader_result.error().log();
        return -1;
    }
    auto shader = shader_result.value();
    
    auto material = cc::Material::Create()
        .SetShader(shader)
        .SetTexture("u_Texture", 0, texture)
        .SetSampler(0, sampler)
        .Build();
    
    auto depth_stencil = cc::DepthStencilState::Default();
    // depth_stencil.depth_test_enable = false;
    
    auto pipeline_result = cc::PipelineState::Create()
        .SetShader(shader)
        .SetVertexLayout(quad_mesh->GetLayout())
        .SetTopology(cc::PrimitiveTopology::Triangles)
        .SetRasterizer(cc::RasterizerState::Default())
        .SetDepthStencil(depth_stencil)
        .SetBlend(cc::BlendState::Disabled())
        .Build();
    
    if (!pipeline_result) {
        pipeline_result.error().log();
        return -1;
    }
    auto pipeline = pipeline_result.value();
    
    auto gl_pipeline = std::static_pointer_cast<cc::GLPipelineState>(pipeline);
    gl_pipeline->GetVAO()->SetVertexBuffer(quad_mesh->GetVertexBuffer(), quad_mesh->GetLayout());
    gl_pipeline->GetVAO()->SetIndexBuffer(quad_mesh->GetIndexBuffer());
    
    auto cmd_result = cc::CommandBuffer::Create();
    if (!cmd_result) {
        cmd_result.error().log();
        return -1;
    }
    auto cmd = cmd_result.value();
    
    auto render_pass = cc::RenderPass::Create()
        .SetFramebuffer(swapchain->GetFramebuffer())
        .SetClearColor(0, {0.1f, 0.1f, 0.1f, 1.0f})
        .SetClearDepth(1.0f)
        .Build();
    
    cc::log::Info("Initialization complete, starting render loop");
    
    while (!window->ShouldClose()) {
        cmd->Begin();
        cmd->BeginRenderPass(render_pass);
        
        cmd->SetPipeline(pipeline);
        material->Apply();
        
        cmd->DrawIndexed(quad_mesh->GetIndexCount());
        
        cmd->EndRenderPass();
        cmd->End();
        
        swapchain->Present();
        window->PollEvents();
    }
    
    cc::RendererContext::Shutdown();
    
    return 0;
}
