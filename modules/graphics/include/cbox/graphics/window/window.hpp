#pragma once
#include "cbox/core/core.hpp"
#include <string>

struct GLFWwindow;

namespace cc {

struct WindowDesc {
    std::string name{"studio"};
    u32 width{1920};
    u32 height{1080};
    bool resizable{true};
    bool vsync{true};
};

class Window {
public:
    ~Window();

    static auto Create(const WindowDesc& desc) -> result<ref<Window>>;

    bool ShouldClose() const noexcept;
    void PollEvents() noexcept;
    void Close() noexcept { should_close_ = true; }

    u32 GetWidth() const noexcept { return width_; }
    u32 GetHeight() const noexcept { return height_; }
    f32 GetAspectRatio() const noexcept { return static_cast<f32>(width_) / static_cast<f32>(height_); }
    bool GetVSync() const noexcept { return vsync_; }

    GLFWwindow* GetNativeWindow() const noexcept { return window_; }

private:
    Window() = default;

    GLFWwindow* window_{nullptr};
    u32 width_{0};
    u32 height_{0};
    bool vsync_{true};
    bool should_close_{false};

    static inline u32 window_count_{0};
};

}
