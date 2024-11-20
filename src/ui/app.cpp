#include "app.hpp"

using namespace ui;

static void glfw_error_callback(int error, const char* description)
{
    std::cout << std::format("GLFW Error {}: {}\n", error, description);
}

App::~App()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

bool App::init(const char* title, const int width, const int height)
{
    // Setup GLFW context
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        return false;
    }

    // Declare GL + GLSL versions
    constexpr const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create GLFW window and graphics context
    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (m_window == nullptr)
    {
        std::cout << "Failed to initialize GLFW window!\n";
        return false;
    }

    glfwSetWindowSizeLimits(m_window, width, height, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    // Setup Dear Imgui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Set Dear ImGui configuration and style
    set_imgui_config();
    set_imgui_style();

    // Setup platform/renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return true;
}

void App::run()
{
    // Main loop
    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();

        // Initialize Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Initialize dock
        ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGuiID main_dock_id = ImGui::DockSpaceOverViewport
                                (
                                    ImGui::GetID(main_viewport),
                                    main_viewport,
                                    ImGuiDockNodeFlags_NoDockingSplit
                                );
                              
        if (m_init_dock)
        {
            ImGui::DockBuilderRemoveNode(main_dock_id);
            ImGui::DockBuilderAddNode(main_dock_id, ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(main_dock_id, ImGui::GetMainViewport()->Size);
            ImGui::DockBuilderDockWindow("main", main_dock_id);
            ImGui::DockBuilderFinish(main_dock_id);
            m_init_dock = false;
        }

        ImGuiWindowClass window_class;
        window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoDockingOverMe |
                                                ImGuiDockNodeFlags_NoTabBar;
        ImGui::SetNextWindowClass(&window_class);
        if (ImGui::Begin("main", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar))
        {
            // Main UI
        }
        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_width = 0;
        int display_height = 0;
        glfwGetFramebufferSize(m_window, &display_width, &display_height);
        glViewport(0, 0, display_width, display_height);
        glClearColor(m_clear_color.x, m_clear_color.y, m_clear_color.z, m_clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and render additional platform windows
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(m_window);
    }
}

void App::set_imgui_config()
{
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.IniFilename = NULL;
    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF(ARIAL_TTF, FONT_SIZE);
    io.Fonts->AddFontDefault();
}

void App::set_imgui_style()
{
    ImGui::StyleColorsLight();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.FrameRounding = ROUNDING_SIZE;
    style.GrabRounding = ROUNDING_SIZE;
    style.ChildRounding = ROUNDING_SIZE;
    style.ChildBorderSize = CHILD_BORDER_SIZE;
    style.Colors[ImGuiCol_WindowBg] = WINDOW_BG_COLOR;
    style.Colors[ImGuiCol_PopupBg] = WINDOW_BG_COLOR;
}
