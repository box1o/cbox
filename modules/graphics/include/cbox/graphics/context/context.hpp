#pragma once
#include "cbox/core/core.hpp"
#include "cbox/graphics/types/types.hpp"

namespace cc {

class RenderDevice;
class Window;

class RendererContext {
  public:
    class Builder {
      public:
        explicit Builder(RenderAPI api) : api_(api) {}

        Builder& SetVSync(bool enabled) {
            vsync_ = enabled;
            return *this;
        }
        Builder& SetMSAA(u32 samples) {
            msaa_samples_ = samples;
            return *this;
        }

        ref<RendererContext> Build();

      private:
        RenderAPI api_;
        bool vsync_{true};
        u32 msaa_samples_{0};
    };

    ~RendererContext();

    static Builder Create(RenderAPI api) {
        return Builder(api);
    }
    static ref<RendererContext> Get() {
        return instance_;
    }
    static void Shutdown();

    RenderAPI GetAPI() const noexcept {
        return api_;
    }
    ref<RenderDevice> GetDevice() const noexcept {
        return device_;
    }

    void InitializeDevice();

  private:
    RendererContext() = default;

    RenderAPI api_{RenderAPI::None};
    ref<RenderDevice> device_;
    bool vsync_{true};
    u32 msaa_samples_{0};

    static inline ref<RendererContext> instance_{nullptr};

    friend class Builder;
    friend class Swapchain;
};

} // namespace cc
