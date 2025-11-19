#pragma once
#include "cbox/core/core.hpp"
#include "cbox/graphics/types/types.hpp"

namespace cc {

class Window;
class Framebuffer;
class CommandBuffer;

class Swapchain {
  public:
    ~Swapchain();

    static auto Create(const ref<Window>& window) -> ref<Swapchain>;

    void Clear(const ClearColor& color, f32 depth = 1.0f);
    void Present();
    void Present(const ref<CommandBuffer>& cmd);

    u32 GetWidth() const noexcept;
    u32 GetHeight() const noexcept;

    ref<Framebuffer> GetFramebuffer() const noexcept {
        return framebuffer_;
    }

  private:
    Swapchain() = default;

    ref<Window> window_;
    ref<Framebuffer> framebuffer_;
    bool initialized_{false};
};

} // namespace cc
