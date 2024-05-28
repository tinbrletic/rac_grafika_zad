# komentar
add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)


set(CMAKE_CXX_STANDARD 17) # ako javlja gresku, stavite 14
set(CMAKE_CXX_STANDARD_REQUIRED ON)
configure_file (
                "${PROJECT_SOURCE_DIR}/studenti/brletic/pathconfig.h.in"
                "${PROJECT_BINARY_DIR}/brletic/pathconfig.h"
)

##########################################
## ovdje navedite svoje datoteke
add_executable(brletic 
            # ovdje navesti svoje datoteke
            shader.cpp shader.h
            vertexbuffer.cpp vertexbuffer.h
            indexbuffer.cpp indexbuffer.h
            vertexbufferlayout.cpp vertexbufferlayout.h
            vertexarray.cpp vertexarray.h
            renderer.cpp renderer.h 
            texture.cpp texture.h
            box.h box_renderer.h
            brletic.cpp
             )

target_include_directories(brletic PRIVATE
            ${PROJECT_SOURCE_DIR}/third_party/glfw-3.3.2/include/
            ${PROJECT_SOURCE_DIR}/third_party/glm/
            ${PROJECT_SOURCE_DIR}/third_party/glew-2.1.0/include/
            ${PROJECT_SOURCE_DIR}/third_party/stb
            ${PROJECT_BINARY_DIR}
            ${PROJECT_BINARY_DIR}/brletic/
)

target_link_libraries(brletic
        ${OPENGL_LIBRARY}
        glfw
        GLEW_2100
        assimp
)

set_property(TARGET brletic PROPERTY CXX_STANDARD 17)
set_property(TARGET brletic PROPERTY CXX_STANDARD_REQUIRED ON)
