project(oma)

add_subdirectory(internal)
add_subdirectory(ui)
add_subdirectory(core)

add_executable(${PROJECT_NAME} main.cpp)

if(CMAKE_BUILD_TYPE MATCHES "Release")
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

target_link_libraries(${PROJECT_NAME} PRIVATE ui)
