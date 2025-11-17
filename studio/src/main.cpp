#include "cbox/core/core.hpp"
#include "cbox/graphics/context/context.hpp"
#include "cbox/graphics/graphics.hpp"
#include "cbox/graphics/swapchain/swapchain.hpp"
#include "cbox/graphics/types/types.hpp"

int main() {
    cc::log::Init("studio");

    auto window = cc::Window::Create({
        .name = "studio",
        .width = 1280,
        .height = 720
    });

    auto ctx = cc::RendererContext::Create(cc::RenderAPI::OpenGL)
        .SetVSync(window->GetVSync())
        .SetMSAA(4)
        .Build();


    auto swapchain = cc::Swapchain::Create(window);







    while (!window->ShouldClose()) {

        swapchain->Clear({0.1f, 0.1f, 0.1f, 1.0f}, 1.0f);


        swapchain->Present();


        window->PollEvents();
    }


    cc::RendererContext::Shutdown();

    return 0;
}
