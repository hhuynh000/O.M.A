#pragma once

#include <string>
#include <format>
#include <iostream>

#include "GLFW/glfw3.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace ui
{
    static constexpr const char* ARIAL_TTF = "c:\\Windows\\Fonts\\Arial.ttf";
    static constexpr float FONT_SIZE = 18.0f;
    static constexpr float ROUNDING_SIZE = 4.0f;
    static constexpr float CHILD_BORDER_SIZE = 2.0f;
    static constexpr ImVec4 WINDOW_BG_COLOR{0.9f, 0.9f, 0.9f, 1.0f};

    class App
    {
    public:
        App() = default;
        ~App();
        bool init(const char* title, int width, int height);
        void run();
    
    private:
        GLFWwindow* m_window = nullptr;
        ImVec4 m_clear_color{0.8f, 0.82f, 0.85f, 1.0f};
        bool m_init_dock = true;

        void set_imgui_config();
        void set_imgui_style();
    };
} // namespace ui
