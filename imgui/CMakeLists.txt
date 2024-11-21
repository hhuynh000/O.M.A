project(imgui)

find_package(glfw3 CONFIG REQUIRED)

add_library(${PROJECT_NAME}
    imgui.cpp
    imgui_widgets.cpp
    imgui_tables.cpp
    imgui_draw.cpp
    imgui_impl_glfw.cpp
    imgui_impl_opengl3.cpp
    imnodes.cpp
)

if (CMAKE_BUILD_TYPE MATCHES "Release")
    set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
    target_compile_options(${PROJECT_NAME} PRIVATE
        /std:c++20
        /DUNICODE
        /D_UNICODE
    )
else()
    set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
    target_compile_options(${PROJECT_NAME} PRIVATE 
        /fsanitize=address
        /Zi
        /std:c++20
        /W4
        /DUNICODE
        /D_UNICODE
    )
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE opengl32.lib gdi32.lib shell32.lib)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
